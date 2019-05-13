/*
 * CommSource.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  CommSource is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_COMM_COMMSOURCE_H_
#define SERIALUIV3_SRC_INCLUDES_COMM_COMMSOURCE_H_
#include "../SerialUIExtIncludes.h"
#include "../SerialUITypes.h"
#include "../SerialUIPlatform.h"
#include "CommRequest.h"


namespace SerialUI {
namespace Comm {

class Source : public SourceBase {
public:
	Source();
	virtual ~Source() {}
	using SourceBase::print;
	using SourceBase::println;
	// virtual int available() = 0;
	virtual void poll() = 0;
	virtual bool getBoolFor(Menu::Item::ID mid, bool * into) = 0;
	virtual bool getCharFor(Menu::Item::ID mid, char * into) = 0;
	virtual bool getLongIntFor(Menu::Item::ID mid, long int * into) = 0;
	virtual bool getLongUIntFor(Menu::Item::ID mid, unsigned long int * into) = 0;
	virtual bool getFloatFor(Menu::Item::ID mid, float * into) = 0;
	virtual bool getStringFor(Menu::Item::ID mid, uint8_t maxLength, ::String * into) = 0;

	virtual bool getNextRequest(Menu::Item::ID inMenu, Request * into) = 0;
};

} /* namespace Comm */
} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_COMM_COMMSOURCE_H_ */
