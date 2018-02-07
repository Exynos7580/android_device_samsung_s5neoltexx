# Power
PRODUCT_PACKAGES += \
    power.exynos5 \
    android.hardware.power@1.0-service \
    android.hardware.power@1.0-impl \
    vendor.lineage.power@1.0-impl

# power control interface
PRODUCT_PACKAGES += \
	libpower-config \
	libzeropowersettings_jni \
	ZeroPowerSettings

# permission-updater
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/power/android.hardware.power@1.0-service.rc:system/vendor/etc/init/android.hardware.power@1.0-service.rc
