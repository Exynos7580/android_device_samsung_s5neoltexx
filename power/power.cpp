/*
 * Copyright (C) 2013 The Android Open Source Project
 * Copyright (C) 2017 Jesse Chan <cjx123@outlook.com>
 * Copyright (C) 2018 Lukas Berger <mail@lukasberger.at>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "log.h"

#include <atomic>
#include <cutils/properties.h>
#include <future>
#include <hardware/hardware.h>
#include <hardware/power.h>
#include <linux/stat.h>
#include <math.h>
#include <pwd.h>
#include <stdlib.h>
#include <string>

#include "config.h"
#include "power.h"
#include "profiles.h"
#include "utils.h"

using namespace std;

#define container_of(addr, struct_name, field_name) \
	((struct_name *)((char *)(addr) - offsetof(struct_name, field_name)))

/***********************************
 * Initializing
 */
static int power_open(const hw_module_t *module, const char *name, hw_device_t **device) {
	int retval = 0; // 0 is ok; -1 is error

	ALOGDD("%s: enter; name=%s", __func__, name);

	if (strcmp(name, POWER_HARDWARE_MODULE_ID) == 0) {
		power_module_t *dev = (power_module_t *)calloc(1, sizeof(power_module_t));

		if (dev) {
			// Common hw_device_t fields
			dev->common.tag = HARDWARE_DEVICE_TAG;
			dev->common.module_api_version = POWER_MODULE_API_VERSION_0_5;
			dev->common.hal_api_version = HARDWARE_HAL_API_VERSION;

			dev->init = power_init;
			dev->powerHint = power_hint;
#ifdef POWER_HAS_LINEAGE_HINTS
			dev->getFeature = power_get_feature;
#endif
			dev->setFeature = power_set_feature;
			dev->setInteractive = power_set_interactive;

			*device = (hw_device_t *)dev;

			struct sec_power_module *power = container_of(dev, struct sec_power_module, base);
			pthread_mutex_init(&power->lock, nullptr);

			power->initialized = false;
			power->dozing = false;
			power->profile.current = PROFILE_INVALID;
			power->profile.requested = PROFILE_INVALID;
			power->input.touchkeys_enabled = true;
			power->input.touchscreen_control_path = "";
		} else {
			retval = -ENOMEM;
		}
	} else {
		retval = -EINVAL;
	}

	ALOGDD("%s: exit; rc=%d", __func__, retval);
	return retval;
}

static void power_init(struct power_module *module) {
	struct sec_power_module *power = container_of(module, struct sec_power_module, base);

	ALOGDD("%s: enter;", __func__);
	POWER_LOCK();

	if (power->initialized) {
		ALOGDD("%s: exit; (already initialized)", __func__);
		return;
	}

	// initialize power-config
	power_config_initialize();

	// get correct touchkeys/enabled-file
	// reads from input1/name:
	//   - flat will return sec_touchscreen
	//   - edge won't
	string touchscreen_input_name;
	read(POWER_TOUCHSCREEN_NAME, touchscreen_input_name);
	ALOGDD("%s: '%s' == '%s'", __func__, touchscreen_input_name.c_str(), POWER_TOUCHSCREEN_NAME_EXPECT);
	if (touchscreen_input_name == POWER_TOUCHSCREEN_NAME_EXPECT) {
		power->variant = FLAT;
		power->input.touchscreen_control_path = POWER_TOUCHSCREEN_ENABLED_FLAT;
	} else {
		power->variant = EDGE;
		power->input.touchscreen_control_path = POWER_TOUCHSCREEN_ENABLED_EDGE;
	}

	// set to normal power profile
	power->profile.requested = PROFILE_BALANCED;
	power_set_profile(power, PROFILE_BALANCED);

	// initialize all input-devices
	power_input_device_state(power, true);

	// disable dt2w by default
	power_dt2w_state(power, false);

	// enable fingerprint by default
	power_fingerprint_state(true);

	power->initialized = true;
	POWER_UNLOCK();
	ALOGDD("%s: exit;", __func__);
}

/***********************************
 * Hinting
 */
static void power_hint(struct power_module *module, power_hint_t hint, void *data) {
	struct sec_power_module *power = container_of(module, struct sec_power_module, base);
	int value = (data ? *((intptr_t *)data) : 0);

	ALOGDD("%s: enter; hint=%d", __func__, hint);
	POWER_LOCK();

	if (!power->initialized) {
		ALOGDD("%s: exit; (not yet initialized)", __func__);
		return;
	}

	switch (hint) {

		/***********************************
		 * Profiles
		 */
		case POWER_HINT_LOW_POWER:
			if (power_profiles_automated()) {
				ALOGI("%s: hint(POWER_HINT_LOW_POWER, %d, %llu)", __func__, value, (unsigned long long)data);

				if (value) {
					power_set_profile(power, PROFILE_POWER_SAVE);
				} else {
					if (PROFILE_INVALID < power->profile.requested) {
						power_set_profile(power, power->profile.requested);
					} else {
						// fall back to BALANCED
						power_set_profile(power, PROFILE_BALANCED);
					}
				}
			}
			break;

#ifdef POWER_HAS_LINEAGE_HINTS
		case POWER_HINT_SET_PROFILE:
			ALOGI("%s: hint(POWER_HINT_SET_PROFILE, %d, %llu)", __func__, value, (unsigned long long)data);
			power->profile.requested = value;
			power_set_profile(power, value);
			break;
#endif

		case POWER_HINT_SUSTAINED_PERFORMANCE:
		case POWER_HINT_VR_MODE:
			if (power_profiles_automated()) {
				if (hint == POWER_HINT_SUSTAINED_PERFORMANCE)
					ALOGI("%s: hint(POWER_HINT_SUSTAINED_PERFORMANCE, %d, %llu)", __func__, value, (unsigned long long)data);
				else if (hint == POWER_HINT_VR_MODE)
					ALOGI("%s: hint(POWER_HINT_VR_MODE, %d, %llu)", __func__, value, (unsigned long long)data);

				if (value) {
					power_set_profile(power, PROFILE_HIGH_PERFORMANCE);
				} else {
					if (PROFILE_INVALID < power->profile.requested) {
						power_set_profile(power, power->profile.requested);
					} else {
						// fall back to BALANCED
						power_set_profile(power, PROFILE_BALANCED);
					}
				}
			}
			break;

		/*case POWER_HINT_DOZING:
			if (power_profiles_automated() && power_profiles_dozing()) {
				ALOGI("%s: hint(POWER_HINT_DOZING, %d, %llu)", __func__, value, (unsigned long long)data);
				if (value) {
					power->dozing = true;

					// update input devices
					power_input_device_state(power, false);

					// We don't want create another profile, and
					// SCREEN_OFF is the best-matching one
					power_set_profile(power, PROFILE_SCREEN_OFF);
				} else {
					power->dozing = false;

					// update input devices
					power_input_device_state(power, true);

					if (PROFILE_INVALID < power->profile.requested) {
						power_set_profile(power, power->profile.requested);
					} else {
						// fall back to BALANCED
						power_set_profile(power, PROFILE_BALANCED);
					}
				}
			}
			break;*/

		/***********************************
		 * Boosting
		 */
		case POWER_HINT_INTERACTION:
			if (power_boost_interaction()) {
				ALOGI("%s: hint(POWER_HINT_INTERACTION, %d, %llu)", __func__, value, (unsigned long long)data);

				power_boostpulse(value ? value : 50000);
				power_boostpulse(value ? value : 50000);
			}

			break;

#ifdef POWER_HAS_LINEAGE_HINTS
        case POWER_HINT_CPU_BOOST:
			if (power_boost_cpu()) {
				ALOGI("%s: hint(POWER_HINT_CPU_BOOST, %d, %llu)", __func__, value, (unsigned long long)data);

				power_boostpulse(value);
				power_boostpulse(value);
			}

			break;
#endif // POWER_HAS_LINEAGE_HINTS

		/***********************************
		 * Inputs
		 */
		case POWER_HINT_DISABLE_TOUCH:
			ALOGI("%s: hint(POWER_HINT_DISABLE_TOUCH, %d, %llu)", __func__, value, (unsigned long long)data);
			power_input_device_state(power, value ? false : true);
			break;

		default: break;
	}

	POWER_UNLOCK();
	ALOGDD("%s: exit;", __func__);
}

/***********************************
 * Profiles
 */
static void power_set_profile(struct sec_power_module *power, int profile) {
 	ALOGI("%s: apply profile %d", __func__, profile);

	// check if profiles are enabled
	if (!power_profiles()) {
		return;
	}

	// store it
	power->profile.current = profile;

	// apply settings
	power_profile data = power_profiles_data[power->profile.current + 1];

	// read current configuration
	if (!update_current_cpugov_path(0) ||
		!update_current_cpugov_path(4)) {
		ALOGW("Failed to load current cpugov-configuration");
		return;
	}

	/*********************
	 * CPU Cluster0
	 */
	if (power_subprofile("cluster0")) {
		write_cpugov(0, "freq_min",     data.cpu.cl0.freq_min);
		write_cpugov(0, "freq_max",     data.cpu.cl0.freq_max);
		write_cpugov(0, "hispeed_freq", data.cpu.cl0.freq_max);

		if (assert_cpugov(0, "nexus")) {
			write_cpugov(0, "down_load",                   data.cpu.nexus.down_load);
			write_cpugov(0, "down_step",                   data.cpu.nexus.down_step);
			write_cpugov(0, "down_load_to_step_ratio",     data.cpu.nexus.down_lts_ratio);
			write_cpugov(0, "down_load_to_step_elevation", data.cpu.nexus.down_lts_elev);
			write_cpugov(0, "up_load",                     data.cpu.nexus.up_load);
			write_cpugov(0, "up_step",                     data.cpu.nexus.up_step);
			write_cpugov(0, "up_load_to_step_ratio",       data.cpu.nexus.up_lts_ratio);
			write_cpugov(0, "up_load_to_step_elevation",   data.cpu.nexus.up_lts_elev);
		}
	}

	/*********************
	 * CPU Cluster1
	 */
	if (power_subprofile("cluster1")) {
		write_cpugov(4, "freq_min",     data.cpu.cl1.freq_min);
		write_cpugov(4, "freq_max",     data.cpu.cl1.freq_max);
		write_cpugov(4, "hispeed_freq", data.cpu.cl1.freq_max);

		if (assert_cpugov(4, "nexus")) {
			write_cpugov(4, "down_load",                   data.cpu.nexus.down_load);
			write_cpugov(4, "down_step",                   data.cpu.nexus.down_step);
			write_cpugov(4, "down_load_to_step_ratio",     data.cpu.nexus.down_lts_ratio);
			write_cpugov(4, "down_load_to_step_elevation", data.cpu.nexus.down_lts_elev);
			write_cpugov(4, "up_load",                     data.cpu.nexus.up_load);
			write_cpugov(4, "up_step",                     data.cpu.nexus.up_step);
			write_cpugov(4, "up_load_to_step_ratio",       data.cpu.nexus.up_lts_ratio);
			write_cpugov(4, "up_load_to_step_elevation",   data.cpu.nexus.up_lts_elev);
		}
	}

	/*********************
	 * HMP
	 */
	if (power_subprofile("hmp")) {
		write("/sys/kernel/hmp/boost",                   data.hmp.boost);
		write("/sys/kernel/hmp/semiboost",               data.hmp.semiboost);
		write("/sys/kernel/hmp/sb_down_threshold",       data.hmp.sb_down_thres);
		write("/sys/kernel/hmp/sb_up_threshold",         data.hmp.sb_up_thres);
		write("/sys/kernel/hmp/active_down_migration",   data.hmp.active_down_migration);
		write("/sys/kernel/hmp/aggressive_up_migration", data.hmp.aggressive_up_migration);
	}

	/*********************
	 * GPU
	 */
	if (power_subprofile("gpu")) {
		write("/sys/devices/14ac0000.mali/dvfs_min_lock",   data.gpu.min_lock);
		write("/sys/devices/14ac0000.mali/dvfs_max_lock",   data.gpu.max_lock);
		write("/sys/devices/14ac0000.mali/highspeed_clock", data.gpu.highspeed_clock);
		write("/sys/devices/14ac0000.mali/highspeed_load",  data.gpu.highspeed_load);
	}

	/*********************
	 * Input
	 */
	if (power_subprofile("input")) {
		write("/sys/class/input_booster/level", (data.input.booster ? 2 : 0));
		write("/sys/class/input_booster/head", data.input.booster_table);
		write("/sys/class/input_booster/tail", data.input.booster_table);
	}

	/*********************
	 * Thermal
	 */
	if (power_subprofile("thermal")) {
		write("/sys/power/enable_dm_hotplug", data.thermal.hotplugging);
		write("/sys/power/ipa/control_temp", data.thermal.ipa_control_temp);
	}

	/*********************
	 * Kernel
	 */
	if (power_subprofile("kernel")) {
		write("/sys/module/workqueue/parameters/power_efficient", data.kernel.power_efficient_workqueue);
	}
}

/***********************************
 * Boost
 */
static void power_boostpulse(int duration) {
	ALOGDD("%s: duration     = %d", __func__, duration);

	if (duration > 0) {
		write_cpugov(0, "boostpulse_duration", duration);
		write_cpugov(1, "boostpulse_duration", duration);
	}

	write_cpugov(0, "boostpulse", true);
	write_cpugov(1, "boostpulse", true);
}

/***********************************
 * Inputs
 */
static void power_fingerprint_state(bool state) {
	/*
	 * Ordered power toggling:
	 *    Turn on:   +Wakelocks  ->  +Regulator
	 *    Turn off:  -Regulator  ->  -Wakelocks
	 */
	if (state) {
		write(POWER_FINGERPRINT_WAKELOCKS, true);
		write(POWER_FINGERPRINT_REGULATOR, true);
	} else {
		write(POWER_FINGERPRINT_REGULATOR, false);

		if (power_fingerprint_wakelocks())
			write(POWER_FINGERPRINT_WAKELOCKS, true);
		else
			write(POWER_FINGERPRINT_WAKELOCKS, false);
	}
}

static void power_dt2w_state(struct sec_power_module *power, bool state) {
	power->input.dt2w = !!state;
	if (state) {
		write(POWER_DT2W_ENABLED, true);
	} else {
		write(POWER_DT2W_ENABLED, false);
	}
}
 
static void power_input_device_state(struct sec_power_module *power, bool state) {
	ALOGDD("%s: state = %d", __func__, state);

	if (state) {
		if (power->input.touchscreen_control_path != "") {
			write(power->input.touchscreen_control_path, true);
		}

		if (power->input.touchkeys_enabled) {
			if (power->variant != EDGE) {
				write(POWER_TOUCHKEYS_ENABLED, true);
			}

			write(POWER_TOUCHKEYS_BRIGHTNESS, 255);
		}

		power_fingerprint_state(true);
		power_dt2w_state(power, power->input.dt2w);
	} else {
		if (power->variant != EDGE && !power->dozing) {
			// save to current state to prevent enabling
			read(POWER_TOUCHKEYS_ENABLED, &power->input.touchkeys_enabled);
		}

		if (power->input.touchscreen_control_path != "" && !power->input.dt2w && !power->dozing) {
			write(power->input.touchscreen_control_path, false);
		}

		write(POWER_TOUCHKEYS_ENABLED, false);
		write(POWER_TOUCHKEYS_BRIGHTNESS, 0);

		if (!power_fingerprint_always_on()) {
			power_fingerprint_state(false);
		}

		power_dt2w_state(power, power->input.dt2w);
	}
}

static void power_set_interactive(struct power_module* module, int on) {
	struct sec_power_module *power = container_of(module, struct sec_power_module, base);
	bool screen_is_on = (on != 0);

	ALOGDD("%s: enter; on=%d", __func__, on);
	POWER_LOCK();

	if (!power->initialized) {
		ALOGDD("%s: exit; (not yet initialized)", __func__);
		return;
	}

	// Android seems to have the habit to go into interactive-state
	// after it went dozing/dreaming. This overwrites the previously
	// ran DOZING-hint. Thus, skip switching into interactive-state
	// if we are dozing right now
	if (power->dozing) {
		return;
	}

	if (power_profiles_automated()) {
		if (!screen_is_on) {
			power_set_profile(power, PROFILE_SCREEN_OFF);
		} else {
			if (PROFILE_INVALID < power->profile.requested) {
				power_set_profile(power, power->profile.requested);
			} else {
				// fall back to BALANCED
				power_set_profile(power, PROFILE_BALANCED);
			}
		}
	}

	power_input_device_state(power, screen_is_on);

	POWER_UNLOCK();
	ALOGDD("%s: exit;", __func__);
}

/***********************************
 * Features
 */
#ifdef POWER_HAS_LINEAGE_HINTS
static int power_get_feature(struct power_module *module, feature_t feature) {
	struct sec_power_module *power = container_of(module, struct sec_power_module, base);
	int retval = -EINVAL;

	ALOGDD("%s: enter; feature=%d", __func__, feature);
	POWER_LOCK();

	if (!power->initialized) {
		ALOGDD("%s: exit; (not yet initialized)", __func__);
		retval = -EAGAIN;
		goto exit;
	}

	switch (feature) {
		case POWER_FEATURE_SUPPORTED_PROFILES:
			ALOGDD("%s: request for POWER_FEATURE_SUPPORTED_PROFILES = %d", __func__, PROFILE_MAX_USABLE);
			retval = PROFILE_MAX_USABLE;
			break;

		case POWER_FEATURE_DOUBLE_TAP_TO_WAKE:
			ALOGDD("%s: request for POWER_FEATURE_DOUBLE_TAP_TO_WAKE = 1", __func__);
			retval = is_file(POWER_DT2W_ENABLED);
			break;
	}

exit:
	POWER_UNLOCK();
	ALOGDD("%s: exit; rc=%d", __func__, retval);

	return retval;
}
#endif // POWER_HAS_LINEAGE_HINTS

static void power_set_feature(struct power_module *module, feature_t feature, int state) {
	struct sec_power_module *power = container_of(module, struct sec_power_module, base);
	ALOGDD("%s: enter; feature=%d, state=%d", __func__, feature, state);
	POWER_LOCK();

	if (!power->initialized) {
		ALOGDD("%s: exit; (not yet initialized)", __func__);
		return;
	}

	switch (feature) {
		case POWER_FEATURE_DOUBLE_TAP_TO_WAKE:
			ALOGDD("%s: set POWER_FEATURE_DOUBLE_TAP_TO_WAKE to \"%d\"", __func__, state);
			power_dt2w_state(power, state);
			break;

		default:
			ALOGW("Error setting the feature %d and state %d, it doesn't exist\n",
				  feature, state);
			break;
	}

	POWER_UNLOCK();
	ALOGDD("%s: exit", __func__);
}

static struct hw_module_methods_t power_module_methods = {
	.open = power_open,
};

struct sec_power_module HAL_MODULE_INFO_SYM = {

	.base = {
		.common = {
			.tag = HARDWARE_MODULE_TAG,
			.module_api_version = POWER_MODULE_API_VERSION_0_5,
			.hal_api_version = HARDWARE_HAL_API_VERSION,
			.id = POWER_HARDWARE_MODULE_ID,
			.name = "Power HAL for Exynos 7420 SoCs",
			.author = "Lukas Berger <mail@lukasberger.at>",
			.methods = &power_module_methods,
		},

		.init = power_init,
		.powerHint = power_hint,
#ifdef POWER_HAS_LINEAGE_HINTS
		.getFeature = power_get_feature,
#endif
		.setFeature = power_set_feature,
		.setInteractive = power_set_interactive,
	}

};
