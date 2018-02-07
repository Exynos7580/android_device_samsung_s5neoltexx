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

#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#ifndef EXYNOS5_POWER_HAL_UTILS_H_INCLUDED
#define EXYNOS5_POWER_HAL_UTILS_H_INCLUDED

/** Common */
bool update_current_cpugov_path(const int core);
bool assert_cpugov(const int core, const string asserted_cpugov);
bool is_file(const string path);
bool is_dir(const string path);

/** Writing */
bool write(const string path, const string str);
bool write(const string path, const bool flag);
bool write(const string path, const int value);
bool write(const string path, const unsigned int value);

/** Writing cpugovs */
bool write_cpugov(const int core, const string file, const string str);
bool write_cpugov(const int core, const string file, const bool flag);
bool write_cpugov(const int core, const string file, const int value);
bool write_cpugov(const int core, const string file, const unsigned int value);

/** Reading */
bool read(const string path, string &str);
bool read(const string path, bool *f);
bool read(const string path, int *v);

#endif // EXYNOS5_POWER_HAL_UTILS_H_INCLUDED
