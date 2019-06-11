/*
 * AuthValidator.cpp
 *
 *  Created on: Jun 6, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */


#include "includes/auth/AuthValidator.h"

namespace SerialUI {
namespace Auth {

Validator::Validator(Storage * storage) : auth_store(storage) {


}

Validator::~Validator() {
}

} /* namespace Auth */
} /* namespace SerialUI */
