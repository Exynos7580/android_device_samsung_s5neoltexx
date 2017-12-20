# Kernel
BOARD_MKBOOTIMG_ARGS := --kernel_offset 0x00008000 --ramdisk_offset 0x01000000 --tags_offset 0x00000100 --board SYSMAGIC000KU
BOARD_KERNEL_CMDLINE := androidboot.selinux=permissive console=tty0 androidboot.console=tty0 androidboot.hardware=samsungexynos7580 fbcon=font:VGA8x8,rotate:1 mem=471M no_console_suspend=1
TARGET_KERNEL_CONFIG := lineageos_s5neolte_defconfig
TARGET_KERNEL_SOURCE := kernel/samsung/exynos7580-common
BOARD_CUSTOM_BOOTIMG_MK := hardware/samsung/mkbootimg.mk
BOARD_KERNEL_IMAGE_NAME := Image-dtb
TARGET_CUSTOM_DTBTOOL := dtbhtoolExynos
