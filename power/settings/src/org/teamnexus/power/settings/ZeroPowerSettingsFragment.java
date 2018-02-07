/*
 * Copyright (C) 2017 The LineageOS Project
 * Copyright (C) 2018 TeamNexus
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.teamnexus.power.settings;

import android.app.ActionBar;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.res.Resources;
import android.os.Bundle;
import android.os.PowerManager;
import android.support.v7.preference.Preference;
import android.support.v7.preference.Preference.OnPreferenceChangeListener;
import android.support.v14.preference.PreferenceFragment;
import android.support.v14.preference.SwitchPreference;

import org.teamnexus.power.settings.R;

public class ZeroPowerSettingsFragment extends PreferenceFragment
		implements OnPreferenceChangeListener {

	private native void nativePowerConfigInitialize();

	/** profiles */
	private native boolean nativePowerProfiles();
	private native void nativePowerProfilesSet(boolean state);

	/** profiles_automated */
	private native boolean nativePowerProfilesAutomated();
	private native void nativePowerProfilesAutomatedSet(boolean state);

	/** profiles_dozing */
	private native boolean nativePowerProfilesDozing();
	private native void nativePowerProfilesDozingSet(boolean state);

	/** boost_interaction */
	private native boolean nativePowerBoostInteraction();
	private native void nativePowerBoostInteractionSet(boolean state);

	/** boost_cpu */
	private native boolean nativePowerBoostCpu();
	private native void nativePowerBoostCpuSet(boolean state);

	/** fingerprint_always_on */
	private native boolean nativePowerFingerprintAlwaysOn();
	private native void nativePowerFingerprintAlwaysOnSet(boolean state);

	/** fingerprint_wakelocks */
	private native boolean nativePowerFingerprintWakelocks();
	private native void nativePowerFingerprintWakelocksSet(boolean state);

	/** subprofile */
	private native boolean nativePowerSubprofile(String subprofile);
	private native void nativePowerSubprofileSet(String subprofile, boolean state);

	private final String KEY_PROFILES = "profiles";
	private final String KEY_PROFILES_AUTOMATED = "profiles_automated";
	private final String KEY_PROFILES_DOZING = "profiles_dozing";
	private final String KEY_BOOST_INTERACTION = "boost_interaction";
	private final String KEY_BOOST_CPU = "boost_cpu";
	private final String KEY_FINGERPRINT_ALWAYS_ON = "fingerprint_always_on";
	private final String KEY_FINGERPRINT_WAKELOCKS = "fingerprint_wakelocks";
	private final String KEY_PROFILES_CLUSTER0 = "profiles_cluster0";
	private final String KEY_PROFILES_CLUSTER1 = "profiles_cluster1";
	private final String KEY_PROFILES_HMP = "profiles_hmp";
	private final String KEY_PROFILES_GPU = "profiles_gpu";
	private final String KEY_PROFILES_INPUT = "profiles_input";
	private final String KEY_PROFILES_THERMAL = "profiles_thermal";
	private final String KEY_PROFILES_KERNEL = "profiles_kernel";

	private Context mContext;

	private SwitchPreference mProfiles;
	private SwitchPreference mProfilesAutomated;
	private SwitchPreference mProfilesDozing;
	private SwitchPreference mBoostInteraction;
	private SwitchPreference mBoostCpu;
	private SwitchPreference mFingerprintAlwaysOn;
	private SwitchPreference mFingerprintWakelocks;
	private SwitchPreference mProfilesCluster0;
	private SwitchPreference mProfilesCluster1;
	private SwitchPreference mProfilesHMP;
	private SwitchPreference mProfilesGPU;
	private SwitchPreference mProfilesInput;
	private SwitchPreference mProfilesThermal;
	private SwitchPreference mProfilesKernel;

	public ZeroPowerSettingsFragment() {
		System.loadLibrary("zeropowersettings_jni");
		nativePowerConfigInitialize();
	}

	@Override
	public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
		addPreferencesFromResource(R.xml.screen_preferences);

		mContext = getContext();

		final ActionBar actionBar = getActivity().getActionBar();
		actionBar.setDisplayHomeAsUpEnabled(true);

        mProfiles = initializePreference(KEY_PROFILES, nativePowerProfiles());
        mProfilesAutomated = initializePreference(KEY_PROFILES_AUTOMATED, nativePowerProfilesAutomated(), mProfiles);
        mProfilesDozing = initializePreference(KEY_PROFILES_DOZING, nativePowerProfilesDozing(), mProfiles);
        mBoostInteraction = initializePreference(KEY_BOOST_INTERACTION, nativePowerBoostInteraction());
        mBoostCpu = initializePreference(KEY_BOOST_CPU, nativePowerBoostCpu());
        mFingerprintAlwaysOn = initializePreference(KEY_FINGERPRINT_ALWAYS_ON, nativePowerFingerprintAlwaysOn());
        mFingerprintWakelocks = initializePreference(KEY_FINGERPRINT_WAKELOCKS, nativePowerFingerprintWakelocks(), mFingerprintAlwaysOn);

        mProfilesCluster0 = initializePreference(KEY_PROFILES_CLUSTER0, nativePowerSubprofile("cluster0"), mProfiles);
        mProfilesCluster1 = initializePreference(KEY_PROFILES_CLUSTER1, nativePowerSubprofile("cluster1"), mProfiles);
        mProfilesHMP = initializePreference(KEY_PROFILES_HMP, nativePowerSubprofile("hmp"), mProfiles);
        mProfilesGPU = initializePreference(KEY_PROFILES_GPU, nativePowerSubprofile("gpu"), mProfiles);
        mProfilesInput = initializePreference(KEY_PROFILES_INPUT, nativePowerSubprofile("input"), mProfiles);
        mProfilesThermal = initializePreference(KEY_PROFILES_THERMAL, nativePowerSubprofile("thermal"), mProfiles);
        mProfilesKernel = initializePreference(KEY_PROFILES_KERNEL, nativePowerSubprofile("kernel"), mProfiles);
	}

	private SwitchPreference initializePreference(String key, boolean defValue) {
		return initializePreference(key, defValue, null);
	}

	private SwitchPreference initializePreference(String key, boolean defValue, SwitchPreference dependency) {
		SwitchPreference pref = (SwitchPreference)findPreference(key);
		pref.setChecked(defValue);
		pref.setOnPreferenceChangeListener(this);

		if (dependency != null)
			pref.setEnabled(dependency.isChecked());

		return pref;
	}
	
	@Override
	public boolean onPreferenceChange(Preference preference, Object newValue) {
		boolean boolval = (boolean)newValue;

		if (mProfiles == preference) {
			nativePowerProfilesSet(boolval);

			mProfilesAutomated.setEnabled(boolval);
			if (boolval && mProfilesAutomated.isChecked())
				mProfilesDozing.setEnabled(true);
			else
				mProfilesDozing.setEnabled(false);

			mProfilesCluster0.setEnabled(boolval);
			mProfilesCluster1.setEnabled(boolval);
			mProfilesHMP.setEnabled(boolval);
			mProfilesGPU.setEnabled(boolval);
			mProfilesInput.setEnabled(boolval);
			mProfilesThermal.setEnabled(boolval);
			mProfilesKernel.setEnabled(boolval);
		} else if (mProfilesAutomated == preference) {
			nativePowerProfilesAutomatedSet(boolval);

			mProfilesDozing.setEnabled(boolval);
		} else if (mProfilesDozing == preference) {
			nativePowerProfilesDozingSet(boolval);
		} else if (mBoostInteraction == preference) {
			nativePowerBoostInteractionSet(boolval);
		} else if (mBoostCpu == preference) {
			nativePowerBoostCpuSet(boolval);
		} else if (mFingerprintAlwaysOn == preference) {
			nativePowerFingerprintAlwaysOnSet(boolval);

			mFingerprintWakelocks.setEnabled(boolval);
		} else if (mFingerprintWakelocks == preference) {
			nativePowerFingerprintWakelocksSet(boolval);
		}
		else if (mProfilesCluster0 == preference) {
			nativePowerSubprofileSet("cluster0", boolval);
		} else if (mProfilesCluster1 == preference) {
			nativePowerSubprofileSet("cluster1", boolval);
		} else if (mProfilesHMP == preference) {
			nativePowerSubprofileSet("hmp", boolval);
		} else if (mProfilesGPU == preference) {
			nativePowerSubprofileSet("gpu", boolval);
		} else if (mProfilesInput == preference) {
			nativePowerSubprofileSet("input", boolval);
		} else if (mProfilesThermal == preference) {
			nativePowerSubprofileSet("thermal", boolval);
		} else if (mProfilesKernel == preference) {
			nativePowerSubprofileSet("kernel", boolval);
		}

        return true;
	}

}
