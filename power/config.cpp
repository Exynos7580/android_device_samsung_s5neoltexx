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

#include "log.h"

#include <string>
#include <sys/types.h>
#include <sys/stat.h>

#include "utils.h"
#include "config.h"

void power_config_initialize() {
	if (!is_dir(BASE_DIRECTORY)) {
		// create directory
		mkdir(BASE_DIRECTORY, S_IRWXU | S_IRWXG | S_IROTH);
	}

	/**
	 * defaults
	 */
	// version 1: 03-02-2018_1
	if (!is_file(PATH_POWER_PROFILES))
		power_profiles_set(POWER_PROFILES_DEFAULT);
	if (!is_file(PATH_POWER_PROFILES_AUTOMATED))
		power_profiles_automated_set(POWER_PROFILES_AUTOMATED_DEFAULT);
	if (!is_file(PATH_POWER_BOOST_INTERACTION))
		power_boost_interaction_set(POWER_BOOST_INTERACTION_DEFAULT);
	if (!is_file(PATH_POWER_BOOST_CPU))
		power_boost_cpu_set(POWER_BOOST_CPU_DEFAULT);
	if (!is_file(PATH_POWER_FINGERPRINT_ALWAYS_ON))
		power_fingerprint_always_on_set(POWER_FINGERPRINT_ALWAYS_ON_DEFAULT);
	if (!is_file(PATH_POWER_SUBPROFILE_BASE "cluster0"))
		power_subprofile_set("cluster0", POWER_SUBPROFILE_DEFAULT);
	if (!is_file(PATH_POWER_SUBPROFILE_BASE "cluster1"))
		power_subprofile_set("cluster1", POWER_SUBPROFILE_DEFAULT);
	if (!is_file(PATH_POWER_SUBPROFILE_BASE "hmp"))
		power_subprofile_set("hmp", POWER_SUBPROFILE_DEFAULT);
	if (!is_file(PATH_POWER_SUBPROFILE_BASE "gpu"))
		power_subprofile_set("gpu", POWER_SUBPROFILE_DEFAULT);
	if (!is_file(PATH_POWER_SUBPROFILE_BASE "input"))
		power_subprofile_set("input", POWER_SUBPROFILE_DEFAULT);
	if (!is_file(PATH_POWER_SUBPROFILE_BASE "thermal"))
		power_subprofile_set("thermal", POWER_SUBPROFILE_DEFAULT);
	if (!is_file(PATH_POWER_SUBPROFILE_BASE "kernel"))
		power_subprofile_set("kernel", POWER_SUBPROFILE_DEFAULT);

	// version 2: 04-02-2018_1
	if (!is_file(PATH_POWER_FINGERPRINT_WAKELOCKS))
		power_fingerprint_wakelocks_set(POWER_FINGERPRINT_WAKELOCKS_DEFAULT);

	// version 3: 06-02-2018_1
	if (!is_file(PATH_POWER_PROFILES_DOZING))
		power_profiles_dozing_set(POWER_PROFILES_DOZING_DEFAULT);
}

/**
 * profiles
 */
bool power_profiles() {
	bool flag = POWER_PROFILES_DEFAULT;
	read(PATH_POWER_PROFILES, &flag);
	return flag;
}

void power_profiles_set(bool state) {
	write(PATH_POWER_PROFILES, state);
}

/**
 * profiles_automated
 */
bool power_profiles_automated() {
	bool flag = POWER_PROFILES_AUTOMATED_DEFAULT;
	read(PATH_POWER_PROFILES_AUTOMATED, &flag);
	return flag;
}

void power_profiles_automated_set(bool state) {
	write(PATH_POWER_PROFILES_AUTOMATED, state);
}

/**
 * profiles_dozing
 */
bool power_profiles_dozing() {
	bool flag = POWER_PROFILES_DOZING_DEFAULT;
	read(PATH_POWER_PROFILES_DOZING, &flag);
	return flag;
}

void power_profiles_dozing_set(bool state) {
	write(PATH_POWER_PROFILES_DOZING, state);
}

/**
 * boost_interaction
 */
bool power_boost_interaction() {
	bool flag = POWER_BOOST_INTERACTION_DEFAULT;
	read(PATH_POWER_BOOST_INTERACTION, &flag);
	return flag;
}

void power_boost_interaction_set(bool state) {
	write(PATH_POWER_BOOST_INTERACTION, state);
}

/**
 * boost_cpu
 */
bool power_boost_cpu() {
	bool flag = POWER_BOOST_CPU_DEFAULT;
	read(PATH_POWER_BOOST_CPU, &flag);
	return flag;
}

void power_boost_cpu_set(bool state) {
	write(PATH_POWER_BOOST_CPU, state);
}

/**
 * fingerprint_always_on
 */
bool power_fingerprint_always_on() {
	bool flag = POWER_FINGERPRINT_ALWAYS_ON_DEFAULT;
	read(PATH_POWER_FINGERPRINT_ALWAYS_ON, &flag);
	return flag;
}

void power_fingerprint_always_on_set(bool state) {
	write(PATH_POWER_FINGERPRINT_ALWAYS_ON, state);
}

/**
 * fingerprint_wakelocks
 */
bool power_fingerprint_wakelocks() {
	bool flag = POWER_FINGERPRINT_WAKELOCKS_DEFAULT;
	read(PATH_POWER_FINGERPRINT_WAKELOCKS, &flag);
	return flag;
}

void power_fingerprint_wakelocks_set(bool state) {
	write(PATH_POWER_FINGERPRINT_WAKELOCKS, state);
}

/**
 * subprofile
 */
bool power_subprofile(string subprofile) {
	bool flag = POWER_SUBPROFILE_DEFAULT;
	read(PATH_POWER_SUBPROFILE_BASE + subprofile, &flag);
	return flag;
}

void power_subprofile_set(string subprofile, bool state) {
	write(PATH_POWER_SUBPROFILE_BASE + subprofile, state);
}
