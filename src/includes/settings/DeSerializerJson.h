/*
 * DeSerializerJson.h
 *
 *  Created on: May 3, 2019
 *      Author: malcalypse
 */

#ifndef SERIALUI_SRC_INCLUDES_SETTINGS_DESERIALIZERJSON_H_
#define SERIALUI_SRC_INCLUDES_SETTINGS_DESERIALIZERJSON_H_


#include "../SerialUIPlatform.h"

#ifdef SERIALUI_SERIALIZER_JSON_ENABLE
#include "SerializerJson.h"
#include "DeSerializer.h"
namespace SerialUI {
namespace Settings {

class DeSerializerJson : public DeSerializer {
public:
	DeSerializerJson(Storage * store, uint16_t maxsize=256);

protected:
	virtual bool deserializeFrom(SerializedState buf);

private:
	bool deserializeObj(JsonObject obj);
	bool deserializeArray(JsonArray obj);
	bool deserializeVariant(JsonVariant obj);

};

} /* namespace Settings */
} /* namespace SerialUI */

#endif /* SERIALUI_SERIALIZER_JSON_ENABLE */


#endif /* SERIALUI_SRC_INCLUDES_SETTINGS_DESERIALIZERJSON_H_ */
