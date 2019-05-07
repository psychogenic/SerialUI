/*
 * ItemReqOptions.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqOptions is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/requests/ItemReqOptions.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

OptionsList::OptionsList(long int initVal, StaticString key,
		StaticString help,
		StaticString opt1,
		StaticString opt2,
		StaticString opt3,
		StaticString opt4,
		StaticString opt5,
		StaticString opt6,
		ValueChangedCallback vchng,
		ValidatorCallback validcb) :
		BoundedLong(initVal, key, help, 1, 2, vchng, validcb),
		opts(NULL), _numOptions(2)

{

	setRequestType(Type::OptionsList);
	doInit(opt1,opt2,opt3,opt4,opt5,opt6);

}


OptionsList::OptionsList(long int initVal, ValueChangedCallback vchng,
		ValidatorCallback validcb) :
				BoundedLong(initVal, 1, 2, vchng, validcb),
				opts(NULL), _numOptions(0)
			{

			}


void OptionsList::doInit(StaticString opt1,
		StaticString opt2,
		StaticString opt3,
		StaticString opt4,
		StaticString opt5,
		StaticString opt6) {
		uint8_t numOpts = 2;

		StaticString daStrings[6] = {
				opt1,
				opt2,
				opt3,
				opt4,
				opt5,
				opt6
		};

		if (opt6 != NULL) {
			numOpts = 6;
		} else if (opt5 != NULL) {
			numOpts = 5;
		} else if (opt4 != NULL) {
			numOpts = 4;
		}else if (opt3 != NULL) {
			numOpts = 3;
		}

		_numOptions = numOpts;
		if (opts) {
			delete [] opts;
		}
		opts = new StaticString[numOpts];
		memcpy(opts, daStrings, (numOpts * sizeof(StaticString)));

		setMaximum(numOpts);

		if (! this->valueIsValid(this->currentValue()))
		{
			this->setValue(1);
		}
}



void OptionsList::setOptions(
		StaticString opt1,
		StaticString opt2,
		StaticString opt3,
		StaticString opt4,
		StaticString opt5,
		StaticString opt6) {

	_numOptions = 2;
	doInit(opt1, opt2, opt3, opt4, opt5, opt6);
}
void OptionsList::freeResources() {

	this->BoundedLong::freeResources();

	if (opts) {
		delete [] opts;
		opts = NULL;
	}
}

StaticString OptionsList::currentSelection() {
	return optionBySelection(this->currentValue());
}

StaticString OptionsList::optionBySelection(uint8_t idx) {
		if (idx < 1){
			return NULL;
		}
		return opts[idx - 1];
	}

StaticString OptionsList::optionByIndex(uint8_t idx) {
		if (idx >= maximum()){
			return NULL;
		}
		return opts[idx];
	}

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */
