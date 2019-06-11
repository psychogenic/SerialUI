/*
 * StateStorage.h
 *
 *  Created on: May 1, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_SETTINGS_STATESTORAGE_H_
#define SERIALUI_SRC_INCLUDES_SETTINGS_STATESTORAGE_H_

#include "../SerialUIPlatform.h"

namespace SerialUI {
namespace Settings {

typedef char SerializedStateByte;
typedef SerializedStateByte * SerializedState;
class Storage {
public:
	Storage() {}
	virtual ~Storage() {}

	virtual bool retrieve(SerializedState into, uint16_t maxlen) = 0;
	virtual bool save(SerializedState st) = 0;

};

} /* namespace Settings */
} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_SETTINGS_STATESTORAGE_H_ */
