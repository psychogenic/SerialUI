/*
 * ItemTrackedView.h
 *
 *  Created on: Sep 27, 2018
 *      Author: Pat Deegan
 *
 *  ItemTrackedView is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_MENUITEM_ITEMTRACKEDVIEW_H_
#define SERIALUI_SRC_INCLUDES_MENUITEM_ITEMTRACKEDVIEW_H_


#include "MenuItem.h"
#include "../Menu.h"
#include "../tracked/TrackedVariable.h"

#ifndef SERIALUI_TRACKEDVIEW_MAXNUM_STATES
#define SERIALUI_TRACKEDVIEW_MAXNUM_STATES	SERIALUI_TRACKEDVIEW_MAXNUM_STATES_DEFAULT
#endif

namespace SerialUI {
namespace Menu {
namespace Item {

class TrackedView : public Item {
public:

	TrackedView(Tracked::View::Type vType, StaticString key, StaticString help);
	TrackedView(ID id, Tracked::View::Type vType, StaticString key, StaticString help);

	virtual void call(Menu * callingMenu) {}

	inline Tracked::View::Type viewType() { return _viewType;}
	inline uint8_t numAssociated() { return _num;}
	inline Tracked::State* associatedByIdx(uint8_t i) { return trackedVars[i];}
	bool associate(Tracked::State & st);

private:
	Tracked::View::Type _viewType;
	uint8_t _num;
	Tracked::State * trackedVars[SERIALUI_TRACKEDVIEW_MAXNUM_STATES];
};

template<Tracked::View::Type SUBTYPE>
class TrackedViewImpl : public TrackedView {
public:
	TrackedViewImpl(StaticString key, StaticString help) :
		TrackedView(SUBTYPE, key, help) {}

	TrackedViewImpl(ID id, StaticString key, StaticString help):
	TrackedView(id, SUBTYPE, key, help) {}


};


} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */



#endif /* SERIALUI_SRC_INCLUDES_MENUITEM_ITEMTRACKEDVIEW_H_ */
