# Ril
PRODUCT_PACKAGES += \
    libprotobuf-cpp-fl26 \
    libprotobuf-cpp-full \
    libsecril-client \
    modemloader \
    libxml2 \
    rild \
    libril \
    libreference-ril \
    libsecril-client-sap \
    android.hardware.radio@1.1 \
    android.hardware.radio.deprecated@1.0

# cpboot-daemon for modem
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/ril/sbin/cbd:system/bin/cbd \
    device/samsung/exynos7580-common/configs/init/rild.rc:system/vendor/etc/init/rild.rc
