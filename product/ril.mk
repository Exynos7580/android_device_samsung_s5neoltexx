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
    android.hardware.radio@1.0 \
    android.hardware.radio.deprecated@1.0

# cpboot-daemon for modem
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/ril/sbin/cbd:$(TARGET_COPY_OUT_VENDOR)/bin/cbd \
    $(LOCAL_PATH)/rootdir/etc/vendor/rild.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/rild.rc
