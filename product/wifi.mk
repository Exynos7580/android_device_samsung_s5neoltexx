# Wi-fi
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/wifi/cred.conf:system/etc/wifi/cred.conf \
    $(LOCAL_PATH)/configs/wifi/wpa_supplicant_overlay.conf:system/etc/wifi/wpa_supplicant_overlay.conf \
    $(LOCAL_PATH)/configs/wifi/p2p_supplicant_overlay.conf:system/etc/wifi/p2p_supplicant_overlay.conf \
	$(LOCAL_PATH)/configs/wifi/filter_ie:system/etc/wifi/filter_ie

# Wifi-Packages
PRODUCT_PACKAGES += \
    android.hardware.wifi@1.0 \
    android.hardware.wifi@1.0-impl \
    android.hardware.wifi@1.0-service \
    libnetcmdiface \
    libwpa_client \
    hostapd \
    macloader \
    wificond \
    wifiloader \
    wifilogd \
    wlutil \
    wpa_supplicant \
    wpa_supplicant.conf