/*
 * DeSerializer.h
 *
 *  Created on: May 3, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_SETTINGS_DESERIALIZER_H_
#define SERIALUI_SRC_INCLUDES_SETTINGS_DESERIALIZER_H_


#include "../menuitem/items.h"
#include "StateStorage.h"


namespace SerialUI {
namespace Settings {

	class DeSerializer {
	public:
		DeSerializer(Storage * store, uint16_t maxsize) ;
		virtual ~DeSerializer() {}

		bool restore();

	protected:
		uint16_t maxSize() { return max_serstate_len;}

		virtual bool deserializeFrom(SerializedState buf) = 0;

		bool setValue(Menu::Item::Request::Request * req, bool val);
		bool setValue(Menu::Item::Request::Request * req, unsigned long int val);
		bool setValue(Menu::Item::Request::Request * req, long int val);
		bool setValue(Menu::Item::Request::Request * req, float val);
		bool setValue(Menu::Item::Request::Request * req, char val);
		bool setValue(Menu::Item::Request::Request * req, const char * val);



	private:
		Storage * _storage;
		uint16_t max_serstate_len;

	};


} /* namespace Settings */
} /* namespace SerialUI */




#endif /* SERIALUI_SRC_INCLUDES_SETTINGS_DESERIALIZER_H_ */
