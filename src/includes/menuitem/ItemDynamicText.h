/*
 * ItemDynamicText.h
 *
 *  Created on: May 31, 2018
 *      Author: Pat Deegan
 *
 *  ItemDynamicText is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_MENUITEM_ITEMDYNAMICTEXT_H_
#define SERIALUI_SRC_INCLUDES_MENUITEM_ITEMDYNAMICTEXT_H_

#include "../SerialUITypes.h"
#include "MenuItem.h"


namespace SerialUI {
namespace Menu {
namespace Item {

class DynamicText : public Item {
public:
	DynamicText(const TopLevelString & dynTitle, const TopLevelString & dynContent="");
	DynamicText(ID id, const TopLevelString & dynTitle, const TopLevelString & dynContent="");

	inline const TopLevelString & title() { return _title;}
	void setTitle(const TopLevelString & setTo);

	inline const TopLevelString & contents() { return _contents;}
	void setContents(const TopLevelString & setTo);



	virtual void call(Menu * callingMenu) {}

private:
	TopLevelString _title;
	TopLevelString _contents;

};

} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_MENUITEM_ITEMDYNAMICTEXT_H_ */
