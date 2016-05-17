/*
   Copyright (c) 2016, The CyanogenMod Project. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   File Name : init_sec.c
   Create Date : 2016.04.13
 */

#include <stdlib.h>
#include <stdio.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

void make_me_dual()
{
	property_set("rild.libpath2", "/system/lib/libsec-ril-dsds.so");
	property_set("persist.radio.multisim.config", "dsds");
	property_set("ro.multisim.simslotcount", "2");
}

void vendor_load_properties()
{
    char platform[PROP_VALUE_MAX];
    char bootloader[PROP_VALUE_MAX];
    char device[PROP_VALUE_MAX];
    char devicename[PROP_VALUE_MAX];

    property_get("ro.bootloader", bootloader);

    if (strstr(bootloader, "G903F")) {
        property_set("ro.build.fingerprint", "samsung/s5neoltexx/s5neolte:5.1.1/LMY47X/G903FXXS2APD1:user/release-keys");
        property_set("ro.build.description", "s5neoltexx-user 5.1.1 LMY47X G903FXXS2APD1 release-keys");
        property_set("ro.product.model", "SM-G903F");
        property_set("ro.product.device", "s5neolte");
	make_me_dual();
    } else if (strstr(bootloader, "G903M")) {
        property_set("ro.build.fingerprint", "samsung/s5neolteub/s5neolte:6.0.1/MMB29K/G903MUBU1BPD3:user/release-keys");
        property_set("ro.build.description", "s5neolteub-user 6.0.1 MMB29K G903MUBU1BPD3 release-keys");
        property_set("ro.product.model", "SM-G903M");
        property_set("ro.product.device", "s5neolte");
	property_set("ro.product.name", "s5neolteub");
	make_me_dual();
    } else {
        property_set("ro.product.model", "SM-G903W");
        property_set("ro.product.device", "s5neolte");
    }

    property_get("ro.product.device", device);
    strlcpy(devicename, device, sizeof(devicename));
    ERROR("Found bootloader id %s setting build properties for %s device\n", bootloader, devicename);
}
