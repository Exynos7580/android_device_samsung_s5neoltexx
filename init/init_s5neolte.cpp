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
 */

#include <stdlib.h>
#include <stdio.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

void make_me_dual() {
	property_set("rild.libpath2", "/system/lib/libsec-ril-dsds.so");
	property_set("persist.radio.multisim.config", "dsds");
	property_set("ro.multisim.simslotcount", "2");
}

void vendor_load_properties() {
	std::string platform = property_get("ro.board.platform");
	
	if (platform != ANDROID_TARGET) {
		return;
	}

	std::string bootloader = property_get("ro.bootloader");

	if (bootloader.find("G903F") == 0) {
		property_set("ro.build.fingerprint", "samsung/s5neoltexx/s5neolte:6.0.1/MMB29K/G903FXXU1BQC1:user/release-keys");
		property_set("ro.build.description", "s5neoltexx-user 6.0.1 MMB29K G903FXXU1BQC1 release-keys");
		property_set("ro.product.model", "SM-G903F");
		property_set("ro.product.device", "s5neolte");
		make_me_dual();
	}
	else if (bootloader.find("G903M") == 0) {
		property_set("ro.build.fingerprint", "samsung/s5neolteub/s5neolte:6.0.1/MMB29K/G903MUBU1BPD3:user/release-keys");
		property_set("ro.build.description", "s5neolteub-user 6.0.1 MMB29K G903MUBU1BPD3 release-keys");
		property_set("ro.product.model", "SM-G903M");
		property_set("ro.product.device", "s5neolte");
		property_set("ro.product.name", "s5neolteub");
		make_me_dual();
	}
	else {
		property_set("ro.product.model", "SM-G903W");
		property_set("ro.product.device", "s5neolte");
	}

	std::string device = property_get("ro.product.device");
	INFO("Found bootloader id %s setting build properties for %s device\n", bootloader.c_str(), device.c_str());
}
