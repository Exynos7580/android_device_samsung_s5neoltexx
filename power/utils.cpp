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

#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#include "utils.h"

static string curr_cpugov_path[8] = { "" };

/***********************************
 * Common
 */
bool update_current_cpugov_path(const int core) {
	string cpugov;
	ostringstream path;
	ostringstream cpugov_path;

	path << "/sys/devices/system/cpu/cpu" << core << "/cpufreq";
	cpugov_path << path.str() << "/scaling_governor";

	if (!read(cpugov_path.str(), cpugov)) {
		return false;
	}

	path << "/" << cpugov;
	curr_cpugov_path[core] = path.str();
	return true;
}

bool assert_cpugov(const int core, const string asserted_cpugov) {
	string cpugov;
	ostringstream path;
	ostringstream cpugov_path;

	path << "/sys/devices/system/cpu/cpu" << core << "/cpufreq";
	cpugov_path << path.str() << "/scaling_governor";

	read(cpugov_path.str(), cpugov);
	return (cpugov == asserted_cpugov);
}

bool assert_cpugov_file(const int core, const string file) {
	ostringstream path;

	path << curr_cpugov_path[core] << "/" << file;
	return is_file(path.str());
}

static int __stat_path(const string path) {
	struct stat fstat;
	const char *cpath = path.c_str();

	if (stat(cpath, &fstat) == -1) {
		return 0;
	}

	return fstat.st_mode;
}

bool is_file(const string path) {
	return (__stat_path(path) & S_IFREG) == S_IFREG;
}

bool is_dir(const string path) {
	return (__stat_path(path) & S_IFDIR) == S_IFDIR;
}


/***********************************
 * Writing
 */
bool write(const string path, const string str) {
	ofstream file;

	file.open(path);
	if (!file.is_open()) {
		ALOGE("%s: failed to open \"%s\"", __func__, path.c_str());
		return false;
	}

	ALOGDD("%s: store \"%s\" to \"%s\"", __func__, str.c_str(), path.c_str());

	file << str;
	file.close();

	return true;
}

bool write(const string path, const bool flag) {
	return write(path, flag ? 1 : 0);
}

bool write(const string path, const int value) {
	return write(path, to_string(value));
}

bool write(const string path, const unsigned int value) {
	return write(path, to_string(value));
}


/***********************************
 * Writing cpugovs
 */
bool write_cpugov(const int core, const string file, const string str) {
	ostringstream path;

	path << curr_cpugov_path[core] << "/" << file;
	if (!is_file(path.str())) {
		ALOGE("%s: failed to open \"%s\"", __func__, path.str().c_str());
		return false;
	}

	return write(path.str(), str);
}

bool write_cpugov(const int core, const string file, const bool flag) {
	return write_cpugov(core, file, flag ? 1 : 0);
}

bool write_cpugov(const int core, const string file, const int value) {
	return write_cpugov(core, file, to_string(value));
}

bool write_cpugov(const int core, const string file, const unsigned int value) {
	return write_cpugov(core, file, to_string(value));
}


/***********************************
 * Reading
 */
bool read(string path, string &str) {
	ifstream file(path);

	if (!file.is_open()) {
		ALOGE("%s: failed to open \"%s\"", __func__, path.c_str());
		return false;
	}

	if (!getline(file, str)) {
		ALOGE("%s: failed to read from \"%s\"", __func__, path.c_str());
		return false;
	}

	ALOGDD("%s: read \"%s\" from \"%s\"", __func__, str.c_str(), path.c_str());

	file.close();
	return true;
}

bool read(string path, bool *f) {
	int out = 0;

	if (!read(path, &out)) {
		return false;
	}

	*f = (out ? true : false);
	return true;
}

bool read(string path, int *v) {
	string line;

	if (!read(path, line)) {
		return false;
	}

	*v = stoi(line);
	return true;
}
