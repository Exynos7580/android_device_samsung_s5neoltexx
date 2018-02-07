/*
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

#include <string>

#ifndef EXYNOS5_POWER_HAL_PROFILES_INCLUDED
#define EXYNOS5_POWER_HAL_PROFILES_INCLUDED

using namespace std;

struct power_profile_cpucluster {

	unsigned int freq_min;
	unsigned int freq_max;

};

struct power_profile {

	struct {

		struct power_profile_cpucluster cl0;

		struct power_profile_cpucluster cl1;

		// we use same settings for both cl0 and cl1
		struct {

			int down_load;
			int down_step;
			int down_lts_ratio;
			int down_lts_elev;

			int up_load;
			int up_step;
			int up_lts_ratio;
			int up_lts_elev;

		} nexus;

	} cpu;

	struct {

		bool boost;
		bool semiboost;

		unsigned int sb_down_thres;
		unsigned int sb_up_thres;

		bool active_down_migration;
		bool aggressive_up_migration;

	} hmp;

	struct {

		unsigned int min_lock;
		unsigned int max_lock;

		unsigned int highspeed_clock;
		unsigned int highspeed_load;

	} gpu;

	struct {

		bool booster;
		string booster_table;

	} input;

	struct {

		bool hotplugging;
		int ipa_control_temp;

	} thermal;

	struct {

		bool power_efficient_workqueue;

	} kernel;

} power_profiles_data[PROFILE_MAX_USABLE + 1] = {

	/***********
	 * PROFILE_SCREEN_OFF
	 */
	{
		.cpu = {
			.cl0 = {
				.freq_max = 400000,
				.freq_min = 200000,
			},
			.cl1 = {
				.freq_max = 400000,
				.freq_min = 200000,
			},
			.nexus = {
				.down_load = 50,
				.down_step = 2,
				.down_lts_ratio = 0,
				.down_lts_elev = 0,

				.up_load = 60,
				.up_step = 1,
				.up_lts_ratio = 0,
				.up_lts_elev = 0,
			},
		},
		.hmp = {
			.boost = false,
			.semiboost = false,
			.sb_down_thres = 150,
			.sb_up_thres = 400,
			.active_down_migration = true,
			.aggressive_up_migration = false,
		},
		.gpu = {
			.min_lock = 100,
			.max_lock = 100,

			.highspeed_clock = 100,
			.highspeed_load = 100,
		},
		.input = {
			.booster = false,
			.booster_table = "0 0 0 0 0 0"
		},
		.thermal = {
			.hotplugging = false,
			.ipa_control_temp = 35,
		},
		.kernel = {
			.power_efficient_workqueue = true,
		},
	},

	/***********
	 * PROFILE_POWER_SAVE
	 */
	{
		.cpu = {
			.cl0 = {
				.freq_max = 1300000,
				.freq_min = 400000,
			},
			.cl1 = {
				.freq_max = 1900000,
				.freq_min = 200000,
			},
			.nexus = {
				.down_load = 40,
				.down_step = 2,
				.down_lts_ratio = 150,
				.down_lts_elev = 1,

				.up_load = 50,
				.up_step = 1,
				.up_lts_ratio = 150,
				.up_lts_elev = 1,
			},
		},
		.hmp = {
			.boost = false,
			.semiboost = false,
			.sb_down_thres = 150,
			.sb_up_thres = 400,
			.active_down_migration = true,
			.aggressive_up_migration = false,
		},
		.gpu = {
			.min_lock = 100,
			.max_lock = 544,

			.highspeed_clock = 266,
			.highspeed_load = 99,
		},
		.input = {
			.booster = true,
			.booster_table = "300 0 400000 0 0 0"
		},
		.thermal = {
			.hotplugging = false,
			.ipa_control_temp = 45,
		},
		.kernel = {
			.power_efficient_workqueue = true,
		},
	},

	/***********
	 * PROFILE_BALANCED
	 */
	{
		.cpu = {
			.cl0 = {
				.freq_max = 1500000,
				.freq_min = 600000,
			},
			.cl1 = {
				.freq_max = 2100000,
				.freq_min = 400000,
			},
			.nexus = {
				.down_load = 40,
				.down_step = 1,
				.down_lts_ratio = 125,
				.down_lts_elev = 1,

				.up_load = 50,
				.up_step = 1,
				.up_lts_ratio = 125,
				.up_lts_elev = 2,
			},
		},
		.hmp = {
			.boost = false,
			.semiboost = true,
			.sb_down_thres = 125,
			.sb_up_thres = 175,
			.active_down_migration = true,
			.aggressive_up_migration = false,
		},
		.gpu = {
			.min_lock = 350,
			.max_lock = 772,

			.highspeed_clock = 700,
			.highspeed_load = 80,
		},
		.input = {
			.booster = true,
			.booster_table = "300 0 800000 0 0 0"
		},
		.thermal = {
			.hotplugging = false,
			.ipa_control_temp = 65,
		},
		.kernel = {
			.power_efficient_workqueue = true,
		},
	},

	/***********
	 * PROFILE_HIGH_PERFORMANCE
	 */
	{
		.cpu = {
			.cl0 = {
				.freq_max = 1704000,
				.freq_min = 1000000,
			},
			.cl1 = {
				.freq_max = 2304000,
				.freq_min = 800000,
			},
			.nexus = {
				.down_load = 20,
				.down_step = 1,
				.down_lts_ratio = 100,
				.down_lts_elev = 2,

				.up_load = 30,
				.up_step = 3,
				.up_lts_ratio = 100,
				.up_lts_elev = 4,
			},
		},
		.hmp = {
			.boost = true,
			.semiboost = false,
			.sb_down_thres = 75,
			.sb_up_thres = 125,
			.active_down_migration = false,
			.aggressive_up_migration = true,
		},
		.gpu = {
			.min_lock = 600,
			.max_lock = 772,

			.highspeed_clock = 772,
			.highspeed_load = 60,
		},
		.input = {
			.booster = true,
			.booster_table = "300 0 1200000 0 0 0"
		},
		.thermal = {
			.hotplugging = false,
			.ipa_control_temp = 75,
		},
		.kernel = {
			.power_efficient_workqueue = false,
		},
	},

	/***********
	 * PROFILE_BIAS_POWER_SAVE
	 */
	{
		.cpu = {
			.cl0 = {
				.freq_max = 1400000,
				.freq_min = 400000,
			},
			.cl1 = {
				.freq_max = 2000000,
				.freq_min = 200000,
			},
			.nexus = {
				.down_load = 35,
				.down_step = 1,
				.down_lts_ratio = 135,
				.down_lts_elev = 1,

				.up_load = 45,
				.up_step = 1,
				.up_lts_ratio = 135,
				.up_lts_elev = 1,
			},
		},
		.hmp = {
			.boost = false,
			.semiboost = false,
			.sb_down_thres = 150,
			.sb_up_thres = 400,
			.active_down_migration = true,
			.aggressive_up_migration = false,
		},
		.gpu = {
			.min_lock = 266,
			.max_lock = 600,

			.highspeed_clock = 420,
			.highspeed_load = 90,
		},
		.input = {
			.booster = true,
			.booster_table = "300 0 600000 0 0 0"
		},
		.thermal = {
			.hotplugging = false,
			.ipa_control_temp = 55,
		},
		.kernel = {
			.power_efficient_workqueue = true,
		},
	},

	/***********
	 * PROFILE_BIAS_PERFORMANCE
	 */
	{
		.cpu = {
			.cl0 = {
				.freq_max = 1600000,
				.freq_min = 800000,
			},
			.cl1 = {
				.freq_max = 2200000,
				.freq_min = 600000,
			},
			.nexus = {
				.down_load = 30,
				.down_step = 1,
				.down_lts_ratio = 115,
				.down_lts_elev = 1,

				.up_load = 40,
				.up_step = 2,
				.up_lts_ratio = 115,
				.up_lts_elev = 3,
			},
		},
		.hmp = {
			.boost = false,
			.semiboost = true,
			.sb_down_thres = 75,
			.sb_up_thres = 125,
			.active_down_migration = false,
			.aggressive_up_migration = true,
		},
		.gpu = {
			.min_lock = 544,
			.max_lock = 772,

			.highspeed_clock = 772,
			.highspeed_load = 70,
		},
		.input = {
			.booster = true,
			.booster_table = "300 0 1000000 0 0 0"
		},
		.thermal = {
			.hotplugging = false,
			.ipa_control_temp = 70,
		},
		.kernel = {
			.power_efficient_workqueue = false,
		},
	},

};

#endif // EXYNOS5_POWER_HAL_PROFILES_INCLUDED
