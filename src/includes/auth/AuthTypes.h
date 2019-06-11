/*
 * AuthTypes.h
 *
 *  Created on: Jun 6, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_AUTH_AUTHTYPES_H_
#define SERIALUI_SRC_INCLUDES_AUTH_AUTHTYPES_H_


#include "../SerialUIPlatform.h"
#include "../SerialUITypes.h"
namespace SerialUI {
namespace Auth {

typedef DynamicString Passphrase;
typedef DynamicString Challenge;
typedef DynamicString ChallengeResponse;

namespace Level {
typedef enum ValueEnum {
	NoAccess= 0,
	Guest = 1,
	User = 2,
	Admin = 3
} Value;

}

}
}


#endif /* SERIALUI_SRC_INCLUDES_AUTH_AUTHTYPES_H_ */
