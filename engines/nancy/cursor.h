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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef NANCY_CURSOR_H
#define NANCY_CURSOR_H

#include "common/array.h"

#include "graphics/managed_surface.h"

namespace Nancy {

class NancyEngine;

class CursorManager {
	friend class NancyEngine;

public:
	enum CursorType { kNormal = 0, kHotspot = 1, kMove = 2, kExit = 3, kRotateCW = 4, kRotateCCW = 5, kTurnLeft = 6, kTurnRight = 7, kNormalArrow, kHotspotArrow };

	CursorManager() :
		_isInitialized(false),
		_curItemID(-1),
		_curCursorType(kNormal),
		_curCursorID(0),
		_hasItem(false),
		_numCursorTypes(0) {}

	void init(Common::SeekableReadStream *chunkStream);

	// Change the current cursor ID. Does not change the graphic
	void setCursor(CursorType type, int16 itemID);
	void setCursorType(CursorType type);
	void setCursorItemID(int16 itemID);

	// Change the cursor graphic. Should be called right before drawing to screen
	void applyCursor();

	const Common::Point &getCurrentCursorHotspot() { return _cursors[_curCursorID].hotspot;}
	const Common::Rect &getPrimaryVideoInactiveZone() { return _primaryVideoInactiveZone; }
	const Common::Point &getPrimaryVideoInitialPos() { return _primaryVideoInitialPos; }

private:
	void showCursor(bool shouldShow);

	struct Cursor {
		Common::Rect bounds;
		Common::Point hotspot;
	};

	// CURS data
	Common::Array<Cursor> _cursors;

	Common::Rect _primaryVideoInactiveZone;
	Common::Point _primaryVideoInitialPos;

	Graphics::ManagedSurface _invCursorsSurface;

	CursorType _curCursorType;
	int16 _curItemID;
	uint _curCursorID;
	bool _hasItem;
	bool _isInitialized;
	int _numCursorTypes;
};

} // End of namespace Nancy

#endif // NANCY_CURSOR_H
