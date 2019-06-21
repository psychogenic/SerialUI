/*
 * DeSerializerJson.cpp
 *
 *  Created on: May 3, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#include "includes/settings/DeSerializerJson.h"

#ifdef SERIALUI_SERIALIZER_JSON_ENABLE

#include "includes/SUIGlobals.h"
namespace SerialUI {
namespace Settings {

DeSerializerJson::DeSerializerJson(Storage * store, uint16_t maxsize) : DeSerializer(store, maxsize) {


}

bool DeSerializerJson::deserializeFrom(SerializedState buf) {
	DynamicJsonDocument doc(maxSize());
	DeserializationError err=deserializeJson(doc, buf);
	if (err != DeserializationError::Ok) {
		return false;
	}

	JsonObject root=doc.as<JsonObject>();
	return deserializeObj(root);


	// for(JsonObject::iterator it=obj.begin(); it!=obj.end();++it) {
}


bool DeSerializerJson::deserializeVariant(JsonVariant obj) {

	if (obj.is<JsonObject>()) {
		SERIALUI_DEBUG_OUTLN(F("DesrJ: is obj"));
		return deserializeObj(obj.as<JsonObject>());
	}

	if (obj.is<JsonArray>()) {
		SERIALUI_DEBUG_OUTLN(F("DesrJ: is array"));
		return deserializeArray(obj.as<JsonArray>());
	}

	return true;


}

bool DeSerializerJson::deserializeObj(JsonObject obj) {


	SERIALUI_DEBUG_OUT(F("DserJ: obj "));
	if (obj.containsKey(JSONSERIALIZER_KEY_ID)
			&& obj.containsKey(JSONSERIALIZER_KEY_VALUE)) {

		SERIALUI_DEBUG_OUTLN(F("has val! "));
		// this is something we actually have a setting for...
		// get it, set it.
		Menu::Item::ID itmId = obj[JSONSERIALIZER_KEY_ID].as<
				Menu::Item::ID>();
		if (!itmId) {
			SERIALUI_DEBUG_OUTLN(F("no id"));
			return true;
		}
		Menu::Item::Request::Request * req =
				Globals::menuStructure()->getRequestById(itmId);
		if (!req) {
			SERIALUI_DEBUG_OUTLN(F("no such req"));
			return false;
		}
		JsonVariant val = obj[JSONSERIALIZER_KEY_VALUE];


		// setValue is smart enough to refuse to do something
		// stupid, so we rely on it, rather than ArduinoJson (which
		// answers val.is<XYZ>() in the affirmative a little too
		// easily), to set the correct value type for the particular
		// input request
		if (
				setValue(req, val.as<unsigned long int>())
				||
				setValue(req, val.as<long int>())
				||
				setValue(req, val.as<float>())
				||
				setValue(req, val.as<const char*>())
				||
				setValue(req, val.as<bool>())
				) {
			return true;

		}
		/*
		if (val.is<bool>()) {
			SERIALUI_DEBUG_OUTLN(F("is bool"));
			return setValue(req, val.as<bool>());

		}

		if (val.is<const char*>()) {
			SERIALUI_DEBUG_OUTLN(F("is str"));
			return setValue(req, val.as<const char*>());
		}

		if (val.is<float>()) {
			SERIALUI_DEBUG_OUTLN(F("is float"));
			return setValue(req, val.as<float>());
		}
		SERIALUI_DEBUG_OUTLN(F("is int?"));
		long int asInt = val.as<long int>();
		return setValue(req, asInt);
		*/

	}
	// an object with sub objects...
	for (JsonObject::iterator it = obj.begin(); it != obj.end(); ++it) {
		SERIALUI_DEBUG_OUT(F("DesrJ: doing "));
		SERIALUI_DEBUG_OUTLN(it->key().c_str());

		if (!deserializeVariant(it->value())) {
			return false;
		}

	}

	return true;

}
bool DeSerializerJson::deserializeArray(JsonArray arr) {
	for(size_t i=0; i<arr.size(); i++) {
		if (! deserializeVariant(arr[i]) )
		{
			return false;
		}

	}

	return true;

}


} /* namespace Settings */
} /* namespace SerialUI */

#endif /* SERIALUI_SERIALIZER_JSON_ENABLE */
