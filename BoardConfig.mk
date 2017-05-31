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

TARGET_OTA_ASSERT_DEVICE := s5neo3g,s5neo3gxx,s5neolte,s5neoltexx

DEVICE_PATH := device/samsung/s5neoltexx

# Include path
TARGET_SPECIFIC_HEADER_PATH += $(DEVICE_PATH)/include

# Bluetooth
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := $(DEVICE_PATH)/bluetooth

# Charger
BOARD_CHARGING_MODE_BOOTING_LPM := /sys/class/power_supply/battery/batt_lp_charging
BOARD_CHARGER_ENABLE_SUSPEND := true
BOARD_CHARGER_SHOW_PERCENTAGE := true
CHARGING_ENABLED_PATH := /sys/class/power_supply/battery/batt_lp_charging

# Hardware
BOARD_HARDWARE_CLASS += $(DEVICE_PATH)/cmhw

# Init
TARGET_INIT_VENDOR_LIB := libinit_s5neolte
TARGET_UNIFIED_DEVICE := true

# Kernel
BOARD_MKBOOTIMG_ARGS := --kernel_offset 0x00008000 --ramdisk_offset 0x01000000 --tags_offset 0x00000100 --dt $(DEVICE_PATH)/dt.img --board SYSMAGIC000KU
BOARD_KERNEL_CMDLINE := androidboot.selinux=permissive console=tty0 androidboot.console=tty0 androidboot.hardware=samsungexynos7580 fbcon=font:VGA8x8,rotate:1 mem=471M no_console_suspend=1
TARGET_KERNEL_CONFIG := lineageos_s5neoltexx_defconfig
TARGET_KERNEL_SOURCE := kernel/samsung/a3xelte
BOARD_CUSTOM_BOOTIMG_MK := hardware/samsung/mkbootimg.mk

# LED
RED_LED_PATH := "/sys/class/leds/led_r/brightness"
GREEN_LED_PATH := "/sys/class/leds/led_g/brightness"
BLUE_LED_PATH := "/sys/class/leds/led_b/brightness"
BACKLIGHT_PATH := "/sys/class/backlight/panel/brightness"

# NFC
BOARD_NFC_HAL_SUFFIX := universal7580
BOARD_HAVE_NFC := true

# Partitions
BOARD_HAS_NO_MISC_PARTITION:= false
TARGET_USERIMAGES_USE_F2FS := true
BOARD_BOOTIMAGE_PARTITION_SIZE := 33554432
BOARD_CACHEIMAGE_PARTITION_SIZE := 209715200
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 39845888
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 3145728000
BOARD_USERDATAIMAGE_PARTITION_SIZE := 12096372736
BOARD_FLASH_BLOCK_SIZE := 131072

# Properties
TARGET_SYSTEM_PROP += $(DEVICE_PATH)/system.prop

# Recovery
TARGET_RECOVERY_FSTAB := $(DEVICE_PATH)/rootdir/etc/fstab.samsungexynos7580

# Radio
BOARD_RIL_CLASS := ../../../$(DEVICE_PATH)/ril
BOARD_MODEM_TYPE := tss310

# inherit from the proprietary version
-include vendor/samsung/s5neoltexx/BoardConfigVendor.mk

# twrp
ifeq ($(WITH_TWRP),true)
-include device/samsung/s5neoltexx/twrp.mk
endif
