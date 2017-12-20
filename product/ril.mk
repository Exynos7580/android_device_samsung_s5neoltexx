# Ril
PRODUCT_PACKAGES += \
    libprotobuf-cpp-full \
    modemloader \
	libsec-ril-wrapper

# cpboot-daemon for modem
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/ril/sbin/cbd:root/sbin/cbd \
    $(LOCAL_PATH)/rootdir/etc/vendor/rild.rc:system/vendor/etc/init/rild.rc \
    $(LOCAL_PATH)/rootdir/etc/vendor/rild-dsds.rc:system/vendor/etc/init/rild-dsds.rc
