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

	struct {
		bool cpu0;
		bool cpu1;
		bool cpu2;
		bool cpu3;
	} cores;
};

struct power_profile {
	struct {
		struct power_profile_cpucluster apollo;
		struct power_profile_cpucluster atlas;
	} cpu;

	struct {
		struct {
			unsigned int freq_min;
			unsigned int freq_max;
		} dvfs;

		struct {
			unsigned int freq;
			unsigned int load;
		} highspeed;
	} gpu;

	struct {
		bool power_efficient_workqueue;
	} kernel;
} power_profiles_data[PROFILE_MAX_USABLE + 1] = {

	/***********
	 * PROFILE_SCREEN_OFF
	 */
	{
		.cpu = {
			.apollo = {
				.freq_min = 200000,
				.freq_max = 400000,
				.cores = {
					.cpu0 = true,
					.cpu1 = true,
					.cpu2 = true,
					.cpu3 = true,
				},
			},
			.atlas = {
				.freq_min = 200000,
				.freq_max = 400000,
				.cores = {
					.cpu0 = false,
					.cpu1 = false,
					.cpu2 = false,
					.cpu3 = false,
				},
			},
		},
		.gpu = {
			.dvfs = {
				.freq_min = 100,
				.freq_max = 100,
			},
			.highspeed = {
				.freq = 100,
				.load = 100,
			},
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
			.apollo = {
				.freq_min = 200000,
				.freq_max = 1100000,
				.cores = {
					.cpu0 = true,
					.cpu1 = true,
					.cpu2 = true,
					.cpu3 = true,
				},
			},
			.atlas = {
				.freq_min = 200000,
				.freq_max = 1300000,
				.cores = {
					.cpu0 = false,
					.cpu1 = false,
					.cpu2 = false,
					.cpu3 = false,
				},
			},
		},
		.gpu = {
			.dvfs = {
				.freq_min = 100,
				.freq_max = 544,
			},
			.highspeed = {
				.freq = 266,
				.load = 99,
			},
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
			.apollo = {
				.freq_min = 400000,
				.freq_max = 1600000,
				.cores = {
					.cpu0 = true,
					.cpu1 = true,
					.cpu2 = true,
					.cpu3 = true,
				},
			},
			.atlas = {
				.freq_min = 400000,
				.freq_max = 1600000,
				.cores = {
					.cpu0 = true,
					.cpu1 = true,
					.cpu2 = true,
					.cpu3 = true,
				},
			},
		},
		.gpu = {
			.dvfs = {
				.freq_min = 100,
				.freq_max = 600,
			},
			.highspeed = {
				.freq = 700,
				.load = 80,
			},
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
			.apollo = {
				.freq_min = 800000,
				.freq_max = 1600000,
				.cores = {
					.cpu0 = true,
					.cpu1 = true,
					.cpu2 = true,
					.cpu3 = true,
				},
			},
			.atlas = {
				.freq_min = 800000,
				.freq_max = 1600000,
				.cores = {
					.cpu0 = true,
					.cpu1 = true,
					.cpu2 = true,
					.cpu3 = true,
				},
			},
		},
		.gpu = {
			.dvfs = {
				.freq_min = 400,
				.freq_max = 80,
			},
			.highspeed = {
				.freq = 772,
				.load = 60,
			},
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
			.apollo = {
				.freq_min = 600000,
				.freq_max = 1100000,
				.cores = {
					.cpu0 = true,
					.cpu1 = true,
					.cpu2 = true,
					.cpu3 = true,
				},
			},
			.atlas = {
				.freq_min = 400000,
				.freq_max = 1000000,
				.cores = {
					.cpu0 = false,
					.cpu1 = false,
					.cpu2 = false,
					.cpu3 = false,
				},
			},
		},
		.gpu = {
			.dvfs = {
				.freq_min = 100,
				.freq_max = 600,
			},
			.highspeed = {
				.freq = 420,
				.load = 90,
			},
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
			.apollo = {
				.freq_min = 1000000,
				.freq_max = 1600000,
				.cores = {
					.cpu0 = true,
					.cpu1 = true,
					.cpu2 = true,
					.cpu3 = true,
				},
			},
			.atlas = {
				.freq_min = 600000,
				.freq_max = 1600000,
				.cores = {
					.cpu0 = true,
					.cpu1 = true,
					.cpu2 = true,
					.cpu3 = true,
				},
			},
		},
		.gpu = {
			.dvfs = {
				.freq_min = 266,
				.freq_max = 90,
			},
			.highspeed = {
				.freq = 772,
				.load = 70,
			},
		},
		.kernel = {
			.power_efficient_workqueue = false,
		},
	},

};

#endif // EXYNOS5_POWER_HAL_PROFILES_INCLUDED
