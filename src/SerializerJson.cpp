/*
 * SerializerJson.cpp
 *
 *  Created on: May 1, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#include "includes/settings/SerializerJson.h"

#ifdef SERIALUI_SERIALIZER_JSON_ENABLE

#define SUIJSON_SERIALIZEREQUEST_IMPLEMENTATION(rtype) \
	bool SerializerJson::serialize(rtype* req) { \
	return addRequestToParent(req); \
}

namespace SerialUI {
namespace Settings {

SerializerJson::SerializerJson(Storage * store, uint16_t maxsize) :
		Serializer(store, maxsize), root(maxsize),
		parentIdx(0)
{

}
bool SerializerJson::serializerStart(Menu::Item::Item * item) {
	// parentstack[0] = ParentDetails(item, &root);
	parentIdx = -1;
	return true;
}
bool SerializerJson::serializePushParent(Menu::Item::SubMenu* itmContainer) {
	JsonObject parentObj;
	JsonArray parentArr;
	bool is_array = false;

	if (parentIdx < 0) {
		parentIdx = 0;
		JsonObject topObj = root.createNestedObject("TOP");

		parentstack[parentIdx] = ParentDetails(itmContainer, topObj);
		return true;
	}


	if (currentParentIsArray()) {

		if (itmContainer->type() == Menu::Item::Type::List) {
			parentArr = currentParentJsonArray().createNestedArray();
			is_array = true;
		} else {
			JsonObject mnuObj = currentParentJsonArray().createNestedObject();
			mnuObj[JSONSERIALIZER_KEY_NAME] = itmContainer->key();
			// JsonObject & mnuObj = currentParentJson()->createNestedArray(itmContainer->key());
			mnuObj[JSONSERIALIZER_KEY_ID] = itmContainer->id();
			parentObj = mnuObj;
		}
	} else {


		if (itmContainer->type() == Menu::Item::Type::List) {
			JsonArray mnuArr = currentParentJsonObj().createNestedArray(itmContainer->key());
			parentArr = mnuArr;
			is_array = true;
		} else {
			JsonObject mnuObj = currentParentJsonObj().createNestedObject(itmContainer->key());
			// JsonObject & mnuObj = currentParentJson()->createNestedArray(itmContainer->key());
			mnuObj[JSONSERIALIZER_KEY_ID] = itmContainer->id();
			parentObj = mnuObj;
		}
	}






	parentIdx ++;
	if (is_array) {
		parentstack[parentIdx] = ParentDetails(itmContainer, parentArr);
	} else  {
		parentstack[parentIdx] = ParentDetails(itmContainer, parentObj);

	}

	return true;


}
/*
 * SUIJSON_SERIALIZEREQUEST_IMPLEMENTATION
 * This is all boiler plate, just used to get the correct return value type
 * from each request and support the generic Serializer interface.
 * Nothing more needed than a call to templated addRequestToParent
 */
SUIJSON_SERIALIZEREQUEST_IMPLEMENTATION(Menu::Item::Request::Boolean)
SUIJSON_SERIALIZEREQUEST_IMPLEMENTATION(Menu::Item::Request::BoundedLong)
SUIJSON_SERIALIZEREQUEST_IMPLEMENTATION(Menu::Item::Request::OptionsList)
SUIJSON_SERIALIZEREQUEST_IMPLEMENTATION(Menu::Item::Request::Float)
SUIJSON_SERIALIZEREQUEST_IMPLEMENTATION(Menu::Item::Request::Long)
SUIJSON_SERIALIZEREQUEST_IMPLEMENTATION(Menu::Item::Request::UnsignedLong)
SUIJSON_SERIALIZEREQUEST_IMPLEMENTATION(Menu::Item::Request::Event)
SUIJSON_SERIALIZEREQUEST_IMPLEMENTATION(Menu::Item::Request::Color)
SUIJSON_SERIALIZEREQUEST_IMPLEMENTATION(Menu::Item::Request::Time)
SUIJSON_SERIALIZEREQUEST_IMPLEMENTATION(Menu::Item::Request::DateTime)
SUIJSON_SERIALIZEREQUEST_IMPLEMENTATION(Menu::Item::Request::Character)
SUIJSON_SERIALIZEREQUEST_IMPLEMENTATION(Menu::Item::Request::String)

bool SerializerJson::serializePopParent(Menu::Item::SubMenu* itmContainer) {
	if (parentIdx > 0) {
		parentIdx --;
		return true;
	}
	return false;
}


bool SerializerJson::serializerEnd(SerializedState into, uint16_t maxsize) {
	serializeJson(root, into, maxsize);
	return true;
}

} /* namespace Settings */
} /* namespace SerialUI */

#endif /* SERIALUI_SERIALIZER_JSON_ENABLE */
