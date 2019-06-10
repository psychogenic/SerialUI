/*
 * AuthValidator.cpp
 *
 *  Created on: Jun 6, 2019
 *      Author: malcalypse
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
