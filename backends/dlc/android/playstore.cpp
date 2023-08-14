/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#if defined(__ANDROID__)

#define FORBIDDEN_SYMBOL_EXCEPTION_time_h
#define FORBIDDEN_SYMBOL_EXCEPTION_abort
#define FORBIDDEN_SYMBOL_EXCEPTION_printf

#include "common/config-manager.h"

#include "backends/dlc/android/playstore.h"
#include "backends/platform/android/android.h"
#include "backends/platform/android/jni-android.h"
#include "backends/dlc/dlcmanager.h"

namespace DLC {
namespace PlayStore {

PlayStore::PlayStore() {
	_dlcPlaystore = JNI::getDLCPlaystore();
	JNIEnv *env = JNI::getEnv();
	_dlcPlaystore = env->NewGlobalRef(_dlcPlaystore);
	initJNI();
	// Common::String dlcpath = ConfMan.get("dlcspath");
	Common::String dlcpath = JNI::getScummVMBasePath() + "/DLCs";
	setDLCPath(dlcpath);
}

void PlayStore::initJNI() {
	if (_JNIinit) {
		return;
	}

	JNIEnv *env = JNI::getEnv();

#define FIND_METHOD(prefix, name, signature) do {     						\
	_MID_ ## prefix ## name = env->GetMethodID(cls, #name, signature);      \
		if (_MID_ ## prefix ## name == 0)                                   \
			error("Can't find method ID " #name);                           \
	} while (0)
#define FIND_FIELD(prefix, name, signature) do {                            \
	_FID_ ## prefix ## name = env->GetFieldID(cls, #name, signature);       \
		if (_FID_ ## prefix ## name == 0)                                   \
			error("Can't find field ID " #name);                            \
	} while (0)

	jclass cls = env->FindClass("org/scummvm/scummvm/DLCPlaystore");
	FIND_METHOD(, startDownload, "(Ljava/lang/String;)V");
	FIND_METHOD(, cancelDownload, "()V");
	FIND_METHOD(, setDLCPath, "(Ljava/lang/String;)V");

#undef FIND_FIELD
#undef FIND_METHOD

	_JNIinit = true;
}

void PlayStore::getAllDLCs() {
	DLC::DLCDesc *dlc = new DLC::DLCDesc();
	dlc->id = "lure_vga";
	dlc->name = "Lure of the Temptress - Freeware Version (English)";
	dlc->platform = "pc";
	dlc->gameid = "lure";
	dlc->description = "Lure of the Temptress (VGA/DOS/English)";
	dlc->language = "en";
	dlc->extra = "VGA";
	dlc->engineid = "lure";
	dlc->guioptions = "sndNoSpeech gameOption1 lang_English";
	dlc->idx = 0;
	DLCMan._dlcs.push_back(dlc);
	DLC::DLCDesc *dlc2 = new DLC::DLCDesc();
	dlc2->id = "bass_cd";
	dlc2->name = "Beneath a Steel Sky (v0.0372 CD/DOS)";
	dlc2->platform = "pc";
	dlc2->gameid = "sky";
	dlc2->description = "Beneath a Steel Sky (v0.0372 CD/DOS)";
	dlc2->language = "en";
	dlc2->extra = "v0.0372 CD";
	dlc2->engineid = "sky";
	dlc2->guioptions = "lang_English (GB) lang_German lang_French lang_English (US) lang_Swedish lang_Italian lang_Portuguese (Brazil) lang_Spanish";
	dlc2->idx = 1;
	DLCMan._dlcs.push_back(dlc2);

	DLCMan.refreshDLCList();
}

void PlayStore::startDownloadAsync(const Common::String &id, const Common::String &url) {
	JNIEnv *env = JNI::getEnv();
	jstring assetPack = env->NewStringUTF(id.c_str());
	env->CallVoidMethod(_dlcPlaystore, _MID_startDownload, assetPack);
}

void PlayStore::cancelDownload() {
	JNIEnv *env = JNI::getEnv();
	env->CallVoidMethod(_dlcPlaystore, _MID_cancelDownload);
}

void PlayStore::setDLCPath(Common::String path) {
	JNIEnv *env = JNI::getEnv();
	jstring jpath = env->NewStringUTF(path.c_str());
	env->CallVoidMethod(_dlcPlaystore, _MID_setDLCPath, jpath);
}

} // End of namespace PlayStore
} // End of namespace DLC

#endif
