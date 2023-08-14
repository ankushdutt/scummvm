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

#ifndef BACKENDS_DLC_PLAYSTORE_H
#define BACKENDS_DLC_PLAYSTORE_H

#if defined(__ANDROID__)

#include "backends/dlc/store.h"
#include <jni.h>

namespace DLC {
namespace PlayStore {
class PlayStore : public DLC::Store {

public:
	PlayStore();
	virtual ~PlayStore() {}

	void initJNI();
	void setDLCPath(Common::String path);

	virtual void getAllDLCs() override;
	virtual void startDownloadAsync(const Common::String &id, const Common::String &url) override;
	virtual void removeCacheFile(const Common::Path &file) override {}
	virtual void cancelDownload() override;

private:
	bool _JNIinit = false;
	jmethodID _MID_startDownload = 0;
	jmethodID _MID_cancelDownload = 0;
	jmethodID _MID_setDLCPath = 0;
	jobject _dlcPlaystore;
};

} // End of namespace PlayStore
} // End of namespace DLC

#endif
#endif
