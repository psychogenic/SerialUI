/*
 * MenuStructure.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  MenuStructure is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/menu/MenuStructure.h"

namespace SerialUI {
namespace Menu {

Structure::Structure(uint8_t numItems) :
		GrowableList<Item::Item, Item::ID>(numItems),
		_topLevel(1, 0, SUI_STR("TOP"), SUI_STR("Main Menu"))
{
	this->addItem(&_topLevel);
}



uint8_t Structure::numItemsWithParent(Item::ID parentId) {
	uint8_t numCount = 0;
	/*
	Serial.print(F("num w/par "));
	Serial.print((int)parentId);
	Serial.print(F("? ("));
	*/
	for (uint8_t i = 0; i < numItems(); i++) {

		/*
		uint8_t pid = itemByIndex(i)->parentId();

		Serial.print((int)pid);
		*/
		if (itemByIndex(i)->parentId() == parentId) {
			numCount++;
		}
	}
	return numCount;
}

Item::Item * Structure::itemById(Item::ID itemId) {
	for (uint8_t i=0; i< numItems(); i++ ) {
		if (itemByIndex(i)->id() == itemId) {
			return itemByIndex(i);
		}
	}

	return NULL;
}
int8_t Structure::indexForItemWithParent(Item::ID parentId, Item::ID targetItemId)  {
	int8_t numCount = 0;

		for (uint8_t i = 0; i < numItems(); i++) {
			Item::Item * itm = itemByIndex(i);
			if (itm->parentId() == parentId) {
				if (itm->id() == targetItemId) {
					return numCount;
				}
				numCount++;
			}
		}

		// Not Found
		return -1;
}
Item::Item * Structure::itemByParentAndIndex(Item::ID parentId, uint8_t idx) {


	uint8_t numCount = 0;
	/*
	Serial.print(F("itemByParentAndIndex looking for "));
	Serial.print((int)parentId);
	Serial.print('/');
	Serial.println((int)idx);
	*/
	for (uint8_t i = 0; i < numItems(); i++) {
	//	Serial.print(F("itemByParentAndIndex -- checking itm #"));
	//	Serial.println((int)i);
		if (itemByIndex(i)->parentId() == parentId) {
	//		Serial.println(F("RIGHT PAR..."));
			if (numCount == idx) {
	//			Serial.println(F("HUZZAH"));
				return itemByIndex(i);
			}
			numCount++;
		}
	}

	// Serial.println(F("NOGO"));
	return NULL;

}

Item::Item * Structure::itemByKey(DynamicString aKey, bool allowPartialMatch) {

	for (uint8_t i = 0; i < numItems(); i++) {
		Item::Item * itm = itemByIndex(i);
		if (staticStringMatch(itm->key(), aKey, allowPartialMatch)) {
				return  itm;
		}
	}

	return NULL;
}
Item::Item * Structure::itemByParentAndKey(Item::ID parentId,
		DynamicString aKey, bool allowPartialMatch) {

	for (uint8_t i = 0; i < numItems(); i++) {
		Item::Item * itm = itemByIndex(i);
		if (itm->parentId() == parentId) {
			if (staticStringMatch(itm->key(), aKey, allowPartialMatch)) {
				return  itm;
			}
		}
	}

	return NULL;
}

} /* namespace Menu */
} /* namespace SerialUI */
