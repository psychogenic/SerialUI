/*
 * TrackedVariable.cpp
 *
 *  Created on: May 26, 2018
 *      Author: Pat Deegan
 *
 *  TrackedVariable is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/tracked/TrackedVariable.h"


namespace SerialUI {
namespace Tracked {

ID State::id_counter = 0;

State::State() : _name(NULL), _hasChanged(true) {
	_id = id_counter++;

}

State::State(ID setId) : _id(setId),
		_name(NULL),
		_hasChanged(true) {

	if (setId >= id_counter) {
		id_counter = setId+1;
	}

}


State::State(StaticString nm) : _name(nm), _hasChanged(true) {
	_id = id_counter++;

}
State::State(StaticString nm, ID setId) :_id(setId),
		_name(nm), _hasChanged(true) {
	if (setId >= id_counter) {
		id_counter = setId+1;
	}

}



}
}


