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

#ifndef EXYNOS5_POWER_HAL_CONFIG_H_INCLUDED
#define EXYNOS5_POWER_HAL_CONFIG_H_INCLUDED

using namespace std;

#define POWER_PROFILES_DEFAULT                 true
#define POWER_PROFILES_AUTOMATED_DEFAULT       true
#define POWER_PROFILES_DOZING_DEFAULT          true
#define POWER_BOOST_INTERACTION_DEFAULT        false
#define POWER_BOOST_CPU_DEFAULT                false
#define POWER_FINGERPRINT_ALWAYS_ON_DEFAULT    false
#define POWER_FINGERPRINT_WAKELOCKS_DEFAULT    true
#define POWER_SUBPROFILE_DEFAULT               true

#define BASE_DIRECTORY    "/data/misc/power/"

#define PATH_POWER_PROFILES                 BASE_DIRECTORY "sys.power.profiles"
#define PATH_POWER_PROFILES_AUTOMATED       BASE_DIRECTORY "sys.power.profiles.automated"
#define PATH_POWER_PROFILES_DOZING          BASE_DIRECTORY "sys.power.profiles.dozing"
#define PATH_POWER_BOOST_INTERACTION        BASE_DIRECTORY "sys.power.boost.interaction"
#define PATH_POWER_BOOST_CPU                BASE_DIRECTORY "sys.power.boost.cpu"
#define PATH_POWER_FINGERPRINT_ALWAYS_ON    BASE_DIRECTORY "sys.power.fingerprint.always-on"
#define PATH_POWER_FINGERPRINT_WAKELOCKS    BASE_DIRECTORY "sys.power.fingerprint.wakelocks"
#define PATH_POWER_SUBPROFILE_BASE          BASE_DIRECTORY "sys.power.subprofile."

void power_config_initialize();

/** profiles */
bool power_profiles();
void power_profiles_set(bool state);

/** profiles_automated */
bool power_profiles_automated();
void power_profiles_automated_set(bool state);

/** profiles_dozing */
bool power_profiles_dozing();
void power_profiles_dozing_set(bool state);

/** boost_interaction */
bool power_boost_interaction();
void power_boost_interaction_set(bool state);

/** boost_cpu */
bool power_boost_cpu();
void power_boost_cpu_set(bool state);

/** fingerprint_always_on */
bool power_fingerprint_always_on();
void power_fingerprint_always_on_set(bool state);

/** fingerprint_wakelocks */
bool power_fingerprint_wakelocks();
void power_fingerprint_wakelocks_set(bool state);

/** subprofile */
bool power_subprofile(string subprofile);
void power_subprofile_set(string subprofile, bool state);

#endif // EXYNOS5_POWER_HAL_CONFIG_H_INCLUDED
