/*
 * GrowableList.h
 *
 *  Created on: May 26, 2018
 *      Author: Pat Deegan
 *
 *  GrowableList is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_MENU_GROWABLELIST_H_
#define SERIALUI_SRC_INCLUDES_MENU_GROWABLELIST_H_

#include "SerialUIExtIncludes.h"


#ifndef SERIALUI_MENUSTRUCTURE_NUMITEMS_GROWLIST
#define SERIALUI_MENUSTRUCTURE_NUMITEMS_GROWLIST	SERIALUI_MENUSTRUCTURE_NUMITEMS_GROWLIST_DEFAULT
#endif

namespace SerialUI {


template<class ITEMTYPE, class IDTYPE>
class GrowableList {
public:
	GrowableList(uint8_t numItems):
		_numItems(0),_maxItems(numItems), _itemsList(NULL)
	{
		_itemsList = new ITEMTYPE*[_maxItems];
		for (uint8_t i = 0; i < _maxItems; i++) {
		_itemsList[i] = NULL;
		}

	}

	virtual ~GrowableList() {
		if (_itemsList) {
			delete[] _itemsList;
		}
	}

	bool addItem(ITEMTYPE * itm) {
		if (! itm) {
			return false;
		}
		if (_numItems >= _maxItems) {
			if (!growItemList()) {
				return false;
			}
		}


		_itemsList[_numItems] = itm;
		_numItems++;
		/*
		Serial.print(F("ADDED ITEM! NOW HAVE: "));
		Serial.println(_numItems);
		Serial.print((int)(itm->id()));
		Serial.print(F(" w/par "));
		Serial.println((int)(itm->parentId()));
		*/

		return true;
	}
	ITEMTYPE * getItemById(IDTYPE id) {
		for (uint8_t i = 0; i < _numItems; i++) {
			if (_itemsList[i]->id() == id) {
				return _itemsList[i];
			}
		}

		return NULL;
	}

	ITEMTYPE * itemByIndex(uint8_t i) { return _itemsList[i];}
	uint8_t numItems() { return _numItems;}
protected:
	uint8_t itemCapacity() { return _maxItems;}
	bool growItemList(uint8_t byAmount=5) {

		uint8_t newMax = _maxItems + byAmount;
		ITEMTYPE ** daNewList = new ITEMTYPE*[newMax];
		if (!daNewList) {
			return false;
		}

		_maxItems = newMax;
		for (uint8_t i=_numItems; i<_maxItems; i++){
			daNewList[i] = NULL;
		}

		for (uint8_t i = 0; i < _numItems; i++) {
			daNewList[i] = _itemsList[i];
		}
		delete[] _itemsList;
		_itemsList = daNewList;


		return true;
	}

private:
	uint8_t _numItems;
	uint8_t _maxItems;
	ITEMTYPE ** _itemsList;

};



} /* namespace SerialUI */



#endif /* SERIALUI_SRC_INCLUDES_MENU_GROWABLELIST_H_ */
