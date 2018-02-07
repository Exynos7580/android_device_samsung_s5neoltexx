/*
 * Copyright (C) 2010 The Android Open Source Project
 * Copyright (C) 2018 TeamNexus
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "ZeroPowerSettings-JNI"

#include "JNIHelp.h"
#include "jni.h"

#include <ScopedUtfChars.h>

#include <utils/Log.h>

/* zero power configuration */
#include "config.h"

extern "C" {

JNIEXPORT void JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerConfigInitialize(JNIEnv* /* env */, jclass /* clazz */) {
	power_config_initialize();
}

/**
 * profiles
 */
JNIEXPORT jboolean JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerProfiles(JNIEnv* /* env */, jclass /* clazz */) {
	return power_profiles();
}

JNIEXPORT void JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerProfilesSet(JNIEnv* /* env */, jclass /* clazz */, jboolean state) {
	power_profiles_set(state);
}

/**
 * profiles_automated
 */
JNIEXPORT jboolean JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerProfilesAutomated(JNIEnv* /* env */, jclass /* clazz */) {
	return power_profiles_automated();
}

JNIEXPORT void JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerProfilesAutomatedSet(JNIEnv* /* env */, jclass /* clazz */, jboolean state) {
	power_profiles_automated_set(state);
}

/**
 * profiles_dozing
 */
JNIEXPORT jboolean JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerProfilesDozing(JNIEnv* /* env */, jclass /* clazz */) {
	return power_profiles_dozing();
}

JNIEXPORT void JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerProfilesDozingSet(JNIEnv* /* env */, jclass /* clazz */, jboolean state) {
	power_profiles_dozing_set(state);
}

/**
 * boost_nteraction
 */
JNIEXPORT jboolean JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerBoostInteraction(JNIEnv* /* env */, jclass /* clazz */) {
	return power_boost_interaction();
}

JNIEXPORT void JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerBoostInteractionSet(JNIEnv* /* env */, jclass /* clazz */, jboolean state) {
	power_boost_interaction_set(state);
}

/**
 * boost_cpu
 */
JNIEXPORT jboolean JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerBoostCpu(JNIEnv* /* env */, jclass /* clazz */) {
	return power_boost_cpu();
}

JNIEXPORT void JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerBoostCpuSet(JNIEnv* /* env */, jclass /* clazz */, jboolean state) {
	power_boost_cpu_set(state);
}

/**
 * fingerprint_always_on
 */
JNIEXPORT jboolean JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerFingerprintAlwaysOn(JNIEnv* /* env */, jclass /* clazz */) {
	return power_fingerprint_always_on();
}

JNIEXPORT void JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerFingerprintAlwaysOnSet(JNIEnv* /* env */, jclass /* clazz */, jboolean state) {
	power_fingerprint_always_on_set(state);
}

/**
 * fingerprint_wakelocks
 */
JNIEXPORT jboolean JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerFingerprintWakelocks(JNIEnv* /* env */, jclass /* clazz */) {
	return power_fingerprint_wakelocks();
}

JNIEXPORT void JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerFingerprintWakelocksSet(JNIEnv* /* env */, jclass /* clazz */, jboolean state) {
	power_fingerprint_wakelocks_set(state);
}

/**
 * subprofile
 */
JNIEXPORT jboolean JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerSubprofile(JNIEnv* env, jclass /* clazz */, jstring subprofile) {
    ScopedUtfChars subprofile_string(env, subprofile);
	return power_subprofile(subprofile_string.c_str());
}

JNIEXPORT void JNICALL
Java_org_teamnexus_power_settings_ZeroPowerSettingsFragment_nativePowerSubprofileSet(JNIEnv* env, jclass /* clazz */, jstring subprofile, jboolean state) {
    ScopedUtfChars subprofile_string(env, subprofile);
	power_subprofile_set(subprofile_string.c_str(), state);
}

} // extern "C"
