/*
 * ItemReqOptions.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqOptions is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQOPTIONS_H_
#define SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQOPTIONS_H_

#include "ItemReqBoundedLong.h"

#define SERIALUI_MENUITEM_OPTLIST_MAXOPTIONS	6

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

class OptionsList : public BoundedLong {
public:
	OptionsList(long int initVal=0, ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL);
	OptionsList(long int initVal, StaticString key,
			StaticString help,
			StaticString opt1,
			StaticString opt2,
			StaticString opt3 = NULL,
			StaticString opt4 = NULL,
			StaticString opt5 = NULL,
			StaticString opt6 = NULL,
			ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL);

	virtual void freeResources() ;

	void setOptions(
			StaticString opt1,
			StaticString opt2,
			StaticString opt3 = NULL,
			StaticString opt4 = NULL,
			StaticString opt5 = NULL,
			StaticString opt6 = NULL);

	StaticString currentSelection();

	StaticString optionBySelection(uint8_t idx) ;
	StaticString optionByIndex(uint8_t idx);
	inline uint8_t numOptions() { return _numOptions;}
private:
	void doInit(
			StaticString opt1,
			StaticString opt2,
			StaticString opt3 = NULL,
			StaticString opt4 = NULL,
			StaticString opt5 = NULL,
			StaticString opt6 = NULL);

	StaticString * opts;
	uint8_t _numOptions;


};

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQOPTIONS_H_ */
