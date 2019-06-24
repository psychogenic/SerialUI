/*
 * SerializerJson.h
 *
 *  Created on: May 1, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_SETTINGS_SERIALIZERJSON_H_
#define SERIALUI_SRC_INCLUDES_SETTINGS_SERIALIZERJSON_H_

#include "../SerialUIPlatform.h"

#ifdef SERIALUI_SERIALIZER_JSON_ENABLE

#define JSONSERIALIZER_KEY_ID			"_i"
#define JSONSERIALIZER_KEY_VALUE		"_v"
#define JSONSERIALIZER_KEY_NAME			"_n"

#include <ArduinoJson.h>
#include "Serializer.h"
namespace SerialUI {
namespace Settings {

#define SERIALIZER_MAX_DEPTH 	10
class SerializerJson : public Serializer {
public:
	SerializerJson(Storage * store, uint16_t maxsize=256);

protected:

	virtual bool serializerStart(Menu::Item::Item * item);
	virtual bool serializePushParent(Menu::Item::SubMenu * itmContainer);
	virtual bool serialize(Menu::Item::Command * cmd) { return true;}



	virtual bool serialize(Menu::Item::Request::Boolean * req);
	virtual bool serialize(Menu::Item::Request::BoundedLong * req);
	virtual bool serialize(Menu::Item::Request::OptionsList * req);
	virtual bool serialize(Menu::Item::Request::Float * req);
	virtual bool serialize(Menu::Item::Request::Long * req);
	virtual bool serialize(Menu::Item::Request::UnsignedLong * req);
	virtual bool serialize(Menu::Item::Request::Event * req);
	virtual bool serialize(Menu::Item::Request::Color * req);
	virtual bool serialize(Menu::Item::Request::Time * req);
	virtual bool serialize(Menu::Item::Request::DateTime * req);
	virtual bool serialize(Menu::Item::Request::Character * req);
	virtual bool serialize(Menu::Item::Request::String * req);

	virtual bool serializePopParent(Menu::Item::SubMenu * itmContainer);


	virtual bool serializerEnd(SerializedState into, uint16_t maxsize);

private:
	typedef struct ParentDetailsStruct {
		Menu::Item::SubMenu * item;
		JsonObject jsonobj;
		JsonArray  jsonarray;
		bool is_array;
		ParentDetailsStruct(Menu::Item::SubMenu *itm, JsonObject & obj) :
			item(itm), jsonobj(obj), is_array(false) {}
		ParentDetailsStruct(Menu::Item::SubMenu *itm, JsonArray & arr) :
			item(itm), jsonarray(arr), is_array(true) {}
		ParentDetailsStruct() :
			item(NULL), jsonobj(), jsonarray(), is_array(false) {}

		inline bool isArray() {
			return is_array;
		}


	} ParentDetails;

	bool addRequestToParent(Menu::Item::Request::String * req) {
		JsonObject reqObj;
				if (currentParentIsArray()) {
					JsonObject oRef = currentParentJsonArray().createNestedObject();
					oRef[JSONSERIALIZER_KEY_NAME] = req->key();
					reqObj = oRef;
				} else {
					JsonObject oRef = currentParentJsonObj().createNestedObject(req->key());
					reqObj = oRef;
				}
				reqObj[JSONSERIALIZER_KEY_ID] = req->id();
				reqObj[JSONSERIALIZER_KEY_VALUE] = req->currentValue().c_str();
				return true;
	}

	template <class REQTYPE>
	bool addRequestToParent(REQTYPE * req) {
		JsonObject reqObj;
		if (currentParentIsArray()) {
			JsonObject oRef = currentParentJsonArray().createNestedObject();

			oRef[JSONSERIALIZER_KEY_NAME] = req->key();
			reqObj = oRef;
		} else {
			JsonObject oRef = currentParentJsonObj().createNestedObject(req->key());
			reqObj = oRef;
		}
		reqObj[JSONSERIALIZER_KEY_ID] = req->id();
		reqObj[JSONSERIALIZER_KEY_VALUE] = req->currentValue();
		return true;
	}
	inline Menu::Item::SubMenu* currentParentMenu() { return parentstack[parentIdx].item;}
	inline bool currentParentIsArray() {
		return parentstack[parentIdx].isArray();
	}
	inline JsonObject & currentParentJsonObj() { return parentstack[parentIdx].jsonobj;}
	inline JsonArray & currentParentJsonArray() {
		return parentstack[parentIdx].jsonarray;
	}

	// ArduinoJson::
	DynamicJsonDocument root;
	ParentDetails parentstack[SERIALIZER_MAX_DEPTH];
	int8_t parentIdx;
};

} /* namespace Settings */
} /* namespace SerialUI */

#endif /* SERIALUI_SERIALIZER_JSON_ENABLE */

#endif /* SERIALUI_SRC_INCLUDES_SETTINGS_SERIALIZERJSON_H_ */
