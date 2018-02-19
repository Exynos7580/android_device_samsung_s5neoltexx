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
			power->profile.current = PROFILE_INVALID;
			power->profile.requested = PROFILE_INVALID;
			power->input.touchkeys_enabled = true;
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

	// set to normal power profile
	power->profile.requested = PROFILE_BALANCED;
	power_set_profile(power, PROFILE_BALANCED);

	// initialize all input-devices
	power_input_device_state(power, true);

#ifdef POWER_DT2W_ENABLED
	power_dt2w_state(power, false);
#endif

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
#ifdef POWER_HAS_LINEAGE_HINTS
		case POWER_HINT_SET_PROFILE:
			ALOGI("%s: hint(POWER_HINT_SET_PROFILE, %d, %llu)", __func__, value, (unsigned long long)data);
			power->profile.requested = value;
			power_set_profile(power, value);
			break;
#endif

		case POWER_HINT_LOW_POWER:
			ALOGI("%s: hint(POWER_HINT_LOW_POWER, %d, %llu)", __func__, value, (unsigned long long)data);

			if (value) {
				power_set_profile(power, PROFILE_POWER_SAVE);
			} else {
				// reset to requested- or fallback-profile
				power_reset_profile(power);
			}
			break;

		case POWER_HINT_SUSTAINED_PERFORMANCE:
			ALOGI("%s: hint(POWER_HINT_SUSTAINED_PERFORMANCE, %d, %llu)", __func__, value, (unsigned long long)data);
			if (value) {
				power_set_profile(power, PROFILE_HIGH_PERFORMANCE);
			} else {
				// reset to requested- or fallback-profile
				power_reset_profile(power);
			}
			break;

        case POWER_HINT_VR_MODE:
			ALOGI("%s: hint(POWER_HINT_VR_MODE, %d, %llu)", __func__, value, (unsigned long long)data);
			if (value) {
				power_set_profile(power, PROFILE_HIGH_PERFORMANCE);
			} else {
				// reset to requested- or fallback-profile
				power_reset_profile(power);
			}
			break;

		/***********************************
		 * Profiles
		 */
		case POWER_HINT_VSYNC:
			ALOGDD("%s: hint(POWER_HINT_VSYNC, %d, %llu)", __func__, value, (unsigned long long)data);
			break;

		case POWER_HINT_INTERACTION:
			ALOGDD("%s: hint(POWER_HINT_INTERACTION, %d, %llu)", __func__, value, (unsigned long long)data);
			break;

		case POWER_HINT_LAUNCH:
			ALOGDD("%s: hint(POWER_HINT_LAUNCH, %d, %llu)", __func__, value, (unsigned long long)data);
 			break;

		/***********************************
		 * Inputs
		 */
		case POWER_HINT_DISABLE_TOUCH:
			ALOGI("%s: hint(POWER_HINT_DISABLE_TOUCH, %d, %llu)", __func__, value, (unsigned long long)data);
			if (!power->screen_on) {
				power_input_device_state(power, value ? false : true);
			}
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
	// settings
	write_cpugov(0, "freq_min",     data.cpu.apollo.freq_min);
	write_cpugov(0, "freq_max",     data.cpu.apollo.freq_max);
	write_cpugov(0, "hispeed_freq", data.cpu.apollo.freq_max);
	// Cores
	write("/sys/devices/system/cpu/cpu0/online", data.cpu.apollo.cores.cpu0);
	write("/sys/devices/system/cpu/cpu1/online", data.cpu.apollo.cores.cpu1);
	write("/sys/devices/system/cpu/cpu2/online", data.cpu.apollo.cores.cpu2);
	write("/sys/devices/system/cpu/cpu3/online", data.cpu.apollo.cores.cpu3);

	/*********************
	 * CPU Cluster1
	 */
	// settings
	write_cpugov(4, "freq_min",     data.cpu.atlas.freq_min);
	write_cpugov(4, "freq_max",     data.cpu.atlas.freq_max);
	write_cpugov(4, "hispeed_freq", data.cpu.atlas.freq_max);
	// Cores
	write("/sys/devices/system/cpu/cpu4/online", data.cpu.atlas.cores.cpu0);
	write("/sys/devices/system/cpu/cpu5/online", data.cpu.atlas.cores.cpu1);
	write("/sys/devices/system/cpu/cpu6/online", data.cpu.atlas.cores.cpu2);
	write("/sys/devices/system/cpu/cpu7/online", data.cpu.atlas.cores.cpu3);

	/*********************
	 * GPU
	 */
	write("/sys/devices/14ac0000.mali/dvfs_min_lock",   data.gpu.dvfs.freq_min);
	write("/sys/devices/14ac0000.mali/dvfs_max_lock",   data.gpu.dvfs.freq_max);
	write("/sys/devices/14ac0000.mali/highspeed_clock", data.gpu.highspeed.freq);
	write("/sys/devices/14ac0000.mali/highspeed_load",  data.gpu.highspeed.load);

	/*********************
	 * Kernel
	 */
	write("/sys/module/workqueue/parameters/power_efficient", data.kernel.power_efficient_workqueue);
}

static void power_reset_profile(struct sec_power_module *power) {
	if (power->profile.requested != PROFILE_INVALID) {
		power_set_profile(power, power->profile.requested);
	} else {
		// fall back to BALANCED
		power_set_profile(power, PROFILE_BALANCED);
	}
}

/***********************************
 * Inputs
 */
#ifdef POWER_DT2W_ENABLED
static void power_dt2w_state(struct sec_power_module *power, bool state) {
	power->input.dt2w = !!state;
	write(POWER_DT2W_ENABLED, state);
}
#endif

static void power_input_device_state(struct sec_power_module *power, bool state) {
	ALOGDD("%s: state = %d", __func__, state);

#ifdef POWER_DT2W_ENABLED
	if ((state == false && power->input.dt2w == false) || state == true)
#endif
		write(POWER_TOUCHSCREEN_ENABLED, state);

	write(POWER_TOUCHKEYS_ENABLED, state);

#ifdef POWER_DT2W_ENABLED
	power_dt2w_state(power, power->input.dt2w);
#endif
}

static void power_set_interactive(struct power_module* module, int on) {
	struct sec_power_module *power = container_of(module, struct sec_power_module, base);
	bool screen_is_on = !!on;

	ALOGDD("%s: enter; on=%d", __func__, on);
	POWER_LOCK();

	if (!power->initialized) {
		ALOGDD("%s: exit; (not yet initialized)", __func__);
		return;
	}

	power->screen_on = screen_is_on;

	if (!screen_is_on) {
		power_set_profile(power, PROFILE_SCREEN_OFF);
	} else {
		// reset to requested- or fallback-profile
		power_reset_profile(power);
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

#ifdef POWER_DT2W_ENABLED
		case POWER_FEATURE_DOUBLE_TAP_TO_WAKE:
			ALOGDD("%s: request for POWER_FEATURE_DOUBLE_TAP_TO_WAKE = 1", __func__);
			retval = is_file(POWER_DT2W_ENABLED);
			break;
#endif

		default:
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
#ifdef POWER_DT2W_ENABLED
		case POWER_FEATURE_DOUBLE_TAP_TO_WAKE:
			ALOGDD("%s: set POWER_FEATURE_DOUBLE_TAP_TO_WAKE to \"%d\"", __func__, state);
			power_dt2w_state(power, state);
			break;
#endif
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
