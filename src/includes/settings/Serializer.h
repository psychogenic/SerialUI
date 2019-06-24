/*
 * Serializer.h
 *
 *  Created on: May 1, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_SETTINGS_SERIALIZER_H_
#define SERIALUI_SRC_INCLUDES_SETTINGS_SERIALIZER_H_

#include "../menuitem/items.h"
#include "StateStorage.h"


namespace SerialUI {
namespace Settings {

	class Serializer {
	public:
		Serializer(Storage * store, uint16_t maxsize) ;
		virtual ~Serializer() {}

		bool store(Menu::Item::Item * item);

	protected:
		// serializerStart call once at start with top level item
		virtual bool serializerStart(Menu::Item::Item * item) { return true;}


		virtual bool serializePushParent(Menu::Item::SubMenu * itmContainer) = 0;

		virtual bool serialize(Menu::Item::Command * cmd) = 0;


		virtual bool serialize(Menu::Item::Request::Boolean * req) = 0;
		virtual bool serialize(Menu::Item::Request::BoundedLong * req) = 0;
		virtual bool serialize(Menu::Item::Request::OptionsList * req) = 0;
		virtual bool serialize(Menu::Item::Request::Float * req) = 0;
		virtual bool serialize(Menu::Item::Request::Long * req) = 0;
		virtual bool serialize(Menu::Item::Request::UnsignedLong * req) = 0;
		virtual bool serialize(Menu::Item::Request::Event * req) = 0;
		virtual bool serialize(Menu::Item::Request::Color * req) = 0;
		virtual bool serialize(Menu::Item::Request::Time * req) = 0;
		virtual bool serialize(Menu::Item::Request::DateTime * req) = 0;
		virtual bool serialize(Menu::Item::Request::Character * req) = 0;
		virtual bool serialize(Menu::Item::Request::String * req) = 0;


		virtual bool serializePopParent(Menu::Item::SubMenu * itmContainer) = 0;

		virtual bool serializerEnd(SerializedState into, uint16_t maxsize) = 0;


	private:

		bool serializeItem(Menu::Item::Item * item);

		bool serializeRequest(Menu::Item::Request::Request * req);

		// Menu::Item::SubMenu * menuStack[SERIALIZER_MAX_DEPTH];
		Storage * _storage;
		uint16_t max_serstate_len;

	};


} /* namespace Settings */
} /* namespace SerialUI */


#endif /* SERIALUI_SRC_INCLUDES_SETTINGS_SERIALIZER_H_ */
