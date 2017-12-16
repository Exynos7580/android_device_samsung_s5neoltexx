#
# Copyright (C) 2017 The LineageOS Project
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

# Inherit from Exynos7580-common
include device/samsung/exynos7580-common/BoardConfigCommon.mk

# Define device codename we support
TARGET_OTA_ASSERT_DEVICE := s5neo3g,s5neo3gxx,s5neolte,s5neoltexx

# Define device-tree path
DEVICE_PATH := device/samsung/s5neoltexx

# Include path
TARGET_SPECIFIC_HEADER_PATH += $(DEVICE_PATH)/include

# Bluetooth
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := $(DEVICE_PATH)/bluetooth

# Hardware
BOARD_HARDWARE_CLASS += $(DEVICE_PATH)/cmhw

# Init
TARGET_INIT_VENDOR_LIB := libinit_sec

# Kernel
TARGET_KERNEL_CONFIG := lineageos_s5neolte_defconfig
TARGET_KERNEL_SOURCE := kernel/samsung/exynos7580-common
TARGET_KERNEL_ARCH := arm64
TARGET_KERNEL_HEADER_ARCH := arm64

# Extracted with libbootimg
BOARD_CUSTOM_BOOTIMG_MK := hardware/samsung/mkbootimg.mk
#BOARD_KERNEL_SEPARATED_DT := true
BOARD_MKBOOTIMG_ARGS := --kernel_offset 0x00008000 --ramdisk_offset 0x01000000 --tags_offset 0x00000100 --dt $(DEVICE_PATH)/dt.img --board SYSMAGIC000KU
#TARGET_CUSTOM_DTBTOOL := dtbhtoolExynos

# Partitions
BOARD_HAS_NO_MISC_PARTITION:= false
TARGET_USERIMAGES_USE_F2FS := true
BOARD_BOOTIMAGE_PARTITION_SIZE := 33554432         # 32MB
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 39845888     # 38MB
BOARD_CACHEIMAGE_PARTITION_SIZE := 209715200       # 200MB
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 3145728000     # 3GB
BOARD_USERDATAIMAGE_PARTITION_SIZE := 12096372736  # 11GB
BOARD_FLASH_BLOCK_SIZE := 131072

# .dat compression
BLOCK_BASED_OTA := true

# PowerHAL
TARGET_POWERHAL_VARIANT := samsung

# Properties
TARGET_SYSTEM_PROP += $(DEVICE_PATH)/system.prop

# Charger
BACKLIGHT_PATH := /sys/devices/14800000.dsim/backlight/panel/brightness

# Recovery
TARGET_RECOVERY_FSTAB := $(DEVICE_PATH)/ramdisk/fstab.samsungexynos7580

# Radio
BOARD_RIL_CLASS := ../../../$(DEVICE_PATH)/ril
BOARD_MODEM_TYPE := tss310

# NFC
BOARD_NFC_CHIPSET := pn547

# inherit from the proprietary version
-include vendor/samsung/s5neoltexx/BoardConfigVendor.mk
