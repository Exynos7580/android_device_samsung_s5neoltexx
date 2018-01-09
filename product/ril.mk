# Ril
PRODUCT_PACKAGES += \
	libprotobuf-cpp-full \
	libsecril-client \
	modemloader \
	rild \
	libreference-ril \
	libsecril-client-sap \
	libril

# cpboot-daemon for modem
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/ril/sbin/cbd:system/bin/cbd \
    $(LOCAL_PATH)/rootdir/etc/vendor/rild.rc:system/vendor/etc/init/rild.rc \
    $(LOCAL_PATH)/rootdir/etc/vendor/rild-dsds.rc:system/vendor/etc/init/rild-dsds.rc
