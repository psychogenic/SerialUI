/*
 * ItemTrackedView.cpp
 *
 *  Created on: Sep 27, 2018
 *      Author: Pat Deegan
 *
 *  ItemTrackedView is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/ItemTrackedView.h"


namespace SerialUI {
namespace Menu {
namespace Item {


TrackedView::TrackedView(Tracked::View::Type vType, StaticString key, StaticString help) :
		Item(Type::TrackedView, key, help),
		_viewType(vType),
		_num(0)
{
	for (uint8_t i=0; i<SERIALUI_TRACKEDVIEW_MAXNUM_STATES; i++) {
		trackedVars[i] = NULL;
	}
}


TrackedView::TrackedView(ID id, Tracked::View::Type vType, StaticString key, StaticString help) :
		Item(id, Type::TrackedView, key, help),
		_viewType(vType),
		_num(0)
{
	for (uint8_t i=0; i<SERIALUI_TRACKEDVIEW_MAXNUM_STATES; i++) {
		trackedVars[i] = NULL;
	}

}

bool TrackedView::associate(Tracked::State & st) {
	if (_num >= SERIALUI_TRACKEDVIEW_MAXNUM_STATES) {
		return false;
	}
	trackedVars[_num++] = &st;
	return true;
}





} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */
