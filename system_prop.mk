# Properties
# Default.prop overrides to get adb working at boot
PRODUCT_PROPERTY_OVERRIDES += \
	ro.secure=0 \
	ro.adb.secure=0 \
	ro.hardware=universal7580 \
	persist.service.adb.enable=1 \
	persist.service.debuggable=1 \
	persist.sys.usb.config=adb \
	ro.securestorage.support=false \
	persist.media.treble_omx=false

# Dalvik/Art
PRODUCT_PROPERTY_OVERRIDES += \
	ro.sys.fw.dex2oat_thread_count=4 \
	dalvik.vm.heapstartsize=8m \
	dalvik.vm.heapgrowthlimit=256m \
	dalvik.vm.heapsize=512m \
	dalvik.vm.heaptargetutilization=0.75 \
	dalvik.vm.heapminfree=2m \
	dalvik.vm.heapmaxfree=8m

# Hwui
PRODUCT_PROPERTY_OVERRIDES += \
	ro.hwui.texture_cache_size=88 \
	ro.hwui.layer_cache_size=58 \
	ro.hwui.path_cache_size=16 \
	ro.hwui.texture_cache_flushrate=0.4 \
	ro.hwui.shape_cache_size=4 \
	ro.hwui.gradient_cache_size=2 \
	ro.hwui.drop_shadow_cache_size=6 \
	ro.hwui.r_buffer_cache_size=8 \
	ro.hwui.text_small_cache_width=1024 \
	ro.hwui.text_small_cache_height=1024 \
	ro.hwui.text_large_cache_width=4096 \
	ro.hwui.text_large_cache_height=2048 \
	ro.hwui.fbo_cache_size=16

# Network
# Define default initial receive window size in segments.
PRODUCT_PROPERTY_OVERRIDES += \
	net.tcp.default_init_rwnd=60

# sdcardfs
PRODUCT_PROPERTY_OVERRIDES += \
	ro.sys.sdcardfs=false

# Allows healthd to boot directly from charger mode rather than initiating a reboot.
PRODUCT_PROPERTY_OVERRIDES += \
    ro.enable_boot_charger_mode=1

# Radio Properties
PRODUCT_PROPERTY_OVERRIDES += \
    persist.radio.add_power_save=1 \
    persist.radio.apm_sim_not_pwdn=1 \
    telephony.lteOnGsmDevice=1 \
    telephony.lteOnCdmaDevice=0 \
    ro.telephony.default_network=9 \
    ro.use_data_netmgrd=false \
    persist.data.netmgrd.qos.enable=false \
    ro.ril.hsxpa=1 \
    ro.ril.telephony.mqanelements=6 \
    ro.ril.gprsclass=10 \
    ro.telephony.get_imsi_from_sim=true \
    ro.ril.force_eri_from_xml=true \
    net.tethering.noprovisioning=true

ADDITIONAL_DEFAULT_PROPERTIES += $(PRODUCT_PROPERTY_OVERRIDES)
