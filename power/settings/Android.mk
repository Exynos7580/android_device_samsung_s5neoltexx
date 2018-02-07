#
# Copyright (C) 2017 The LineageOS Project
# Copyright (C) 2018 TeamNexus
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH:= $(call my-dir)

#
# ZeroPowerSettings user-interface
#
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := $(call all-java-files-under, src)

LOCAL_PACKAGE_NAME := ZeroPowerSettings
LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_JNI_SHARED_LIBRARIES := libzeropowersettings_jni

LOCAL_STATIC_JAVA_LIBRARIES := \
	android-support-v4 \
	android-support-v7-appcompat \
	android-support-v7-preference \
	android-support-v7-recyclerview \
	android-support-v13 \
	android-support-v14-preference

LOCAL_PROGUARD_FLAG_FILES := proguard.flags

LOCAL_RESOURCE_DIR := \
	$(LOCAL_PATH)/res

ifneq (,$(wildcard lineage-sdk/))
  LOCAL_RESOURCE_DIR += $(LOCAL_PATH)/res-lineage
else
  LOCAL_RESOURCE_DIR += $(LOCAL_PATH)/res-non-lineage
endif

LOCAL_RESOURCE_DIR += \
	frameworks/support/v14/preference/res \
	frameworks/support/v7/appcompat/res \
	frameworks/support/v7/preference/res \
	frameworks/support/v7/recyclerview/res

LOCAL_AAPT_FLAGS := \
	--auto-add-overlay \
	--extra-packages android.support.v14.preference \
	--extra-packages android.support.v7.appcompat \
	--extra-packages android.support.v7.preference \
	--extra-packages android.support.v7.recyclerview

include $(BUILD_PACKAGE)

#
# ZeroPowerSettings JNI-interface
#
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	$(call all-cpp-files-under, jni)

LOCAL_MODULE := libzeropowersettings_jni
LOCAL_CFLAGS := -Wall -Werror -Wno-unused-parameter -Wno-unused-function

LOCAL_SHARED_LIBRARIES := \
	liblog \
	libnativehelper \
	libpower-config

LOCAL_C_INCLUDES += \
	$(JNI_H_INCLUDE) \
	device/samsung/zero-common/power/

include $(BUILD_SHARED_LIBRARY)
