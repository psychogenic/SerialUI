/*
 * Serializer.cpp
 *
 *  Created on: May 1, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */



#include "includes/settings/Serializer.h"

#include "includes/SerialUIPlatform.h"
namespace SerialUI {
namespace Settings {

Serializer::Serializer(Storage * store, uint16_t maxsize) :
		_storage(store), max_serstate_len(maxsize) {
	/*
	for (uint8_t i=0; i<SERIALIZER_MAX_DEPTH; i++) {
		menuStack[i] = NULL;
	}
	*/
}
bool Serializer::store(Menu::Item::Item * item) {
	if (! serializerStart(item)) {
		return false;
	}

	if (! serializeItem(item) ) {
		return false;
	}
	SerializedStateByte * buf = new SerializedStateByte[max_serstate_len];
	if (! buf){
		SERIALUI_DEBUG_OUTLN(F("Could not alloc serializer buf"));
		return false;
	}

	bool success = serializerEnd(buf, max_serstate_len - 1);
	if (success) {
		_storage->save(buf);
	}

	delete [] buf;
	return success;
}

/*
 * Example structure
 * TOP
 * 	- command 1
 * 	- request A
 * 	- submenu
 * 		- command 2
 * 		- request B
 * 		- sub 2
 * 			- request C
 * 		- request D
 * 	- list
 * 		- groupa
 * 		  - cmd 3
 * 		  - request E
 * 		- groupb
 * 		  - cmd 3
 * 		  - request E
 * 	- request F
 * Calls will to subclass
 *
 * 	serializerStart(TOP)
 * 	serializePushParent(TOP)
 * 	 serialize(commannd 1)
 * 	 serialize(request A)
 * 	 serializePushParent(submenu)
 * 	   serialize(command 2)
 * 	   serialize(request B)
 * 	   serializePushParent(sub 2)
 * 	     serialize(request C)
 * 	   serializePopParent(sub 2)
 * 	   serialize(request D)
 * 	 serializePopParent(submenu)
 * 	 serializePushParent(list)
 * 	   serializePushParent(groupa)
 * 	     serialize(cmd 3)
 * 	     serialize(request E)
 * 	   serializePopParent(groupa)
 * 	   serializePushParent(groupb)
 * 	     serialize(cmd 3)
 * 	     serialize(request E)
 * 	   serializePopParent(groupb)
 * 	  serializePopParent(list)
 * 	  serialize(request F)
 * 	 serializePopParent(TOP)
 *
 *
 *
 */
bool Serializer::serializeItem(Menu::Item::Item * item) {
	Menu::Item::SubMenu* subMen = NULL;
	SERIALUI_DEBUG_OUT(F("SERIALIZEITM: "));
	SERIALUI_DEBUG_OUTLN(item->key());

	switch (item->type()) {
	case Menu::Item::Type::Command:
		return serialize((Menu::Item::Command*)item);
	case Menu::Item::Type::Input:
		return serializeRequest((Menu::Item::Request::Request*)item);
	case Menu::Item::Type::Menu:
		/* fall through */
	case Menu::Item::Type::Group:
		/* fall through */
	case Menu::Item::Type::List:
		subMen = (Menu::Item::SubMenu*)item;
		SERIALUI_DEBUG_OUTLN(F("Will have children. (push)"));
		serializePushParent(subMen);
		break;

	default:
		// don't care
		break;

	}
	if (subMen) {
		// this is a container...
		SERIALUI_DEBUG_OUTLN(F("doing children... "));
		SERIALUI_DEBUG_OUTLN(item->key());
		for (uint8_t i=0; i<subMen->numItems(); i++) {
			SERIALUI_DEBUG_OUT(F("child "));
			SERIALUI_DEBUG_OUTLN((int) i);
			if (! serializeItem(subMen->itemByIndex(i)) )
			{
				return false;
			}

		}
		SERIALUI_DEBUG_OUTLN(F("doing with children. (pop)"));
		serializePopParent(subMen);
	}
	return true;

}

bool Serializer::serializeRequest(Menu::Item::Request::Request * req) {

	switch (req->requestType()) {
		case Menu::Item::Request::Type::Boolean:
			/* fall - through */
		case Menu::Item::Request::Type::Toggle:
			return serialize(req->castAsSubType<Menu::Item::Request::Boolean>());
		case Menu::Item::Request::Type::BoundedLongInt:
			return serialize(req->castAsSubType<Menu::Item::Request::BoundedLong>());
		case Menu::Item::Request::Type::OptionsList:
			return serialize(req->castAsSubType<Menu::Item::Request::OptionsList>());
			break;

		case Menu::Item::Request::Type::Float:
			return serialize(req->castAsSubType<Menu::Item::Request::Float>());
		case Menu::Item::Request::Type::LongInt:
			return serialize(req->castAsSubType<Menu::Item::Request::Long>());
		case  Menu::Item::Request::Type::Color:
			/* fall-through */
		case  Menu::Item::Request::Type::UnsignedLongInt:
			return serialize(req->castAsSubType<Menu::Item::Request::UnsignedLong>());
		case Menu::Item::Request::Type::Event:
			return serialize(req->castAsSubType<Menu::Item::Request::Event>());

		case Menu::Item::Request::Type::Time:
			return serialize(req->castAsSubType<Menu::Item::Request::Time>());
			break;

		case Menu::Item::Request::Type::DateTime:
			return serialize(req->castAsSubType<Menu::Item::Request::DateTime>());

		case Menu::Item::Request::Type::Character:
			return serialize(req->castAsSubType<Menu::Item::Request::Character>());
		case Menu::Item::Request::Type::Passphrase:
			/* fall-through */
		case Menu::Item::Request::Type::String:
					return serialize(req->castAsSubType<Menu::Item::Request::String>());

		default:
			// UNKNOWN TYPE
			return true;



		}
}

} /* namespace Settings */
} /* namespace SerialUI */



