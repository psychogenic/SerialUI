/*
 * Request.cpp
 *
 *  Created on: May 17, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2016 Pat Deegan, http://psychogenic.com
 * 
 */

#include "includes/menuitem/Request.h"


namespace SUI {
namespace MenuItem {
namespace Request {



void Char::showPrompt(Menu * callingMenu) {

		callingMenu->driver()->showEnterDataPrompt();
	}
	bool Char::getValue(Menu * callingMenu, char * into) {

		char aChar;

		SUI_OUTPUT_DEBUG_DEFAULTSTREAM(F("Char::val: "));
		if (callingMenu->driver()->readBytes(&aChar, 1) < 1)
		{
			SUI_OUTPUTLN_DEBUG_DEFAULTSTREAM(F("nothing"));
			return false;
		}
		SUI_OUTPUTLN_DEBUG_DEFAULTSTREAM(aChar);

		bool hasChanged = (aChar != *the_value);
		*into = aChar;
		return hasChanged;

	}

	void Char::printPrefix(Menu * callingMenu, SUI::Mode::Selection curMode)
	{
		if (curMode == SUI::Mode::User)
		{
			return this->RequestBase<char>::printPrefix(callingMenu, curMode);
		}


		callingMenu->driver()->print(this->getString(StaticString::RequestCharPrefix));
	}



	char Bool::numericRequestSubtype() {
		return (char)Request::Type::Bool;
	}

bool Bool::getValue(Menu * callingMenu, bool * into) {
		bool val = callingMenu->driver()->parseInt();


		SUI_OUTPUT_DEBUG_DEFAULTSTREAM(F("Bool::val: "));
		SUI_OUTPUTLN_DEBUG_DEFAULTSTREAM(val ? F("TRUE") : F("FALSE"));

		bool hasChanged = false;
		if (val != *the_value)
		{
			hasChanged = true;
		}


		*into = val;
		return hasChanged;

	}

char Toggle::numericRequestSubtype() {
	return (char)Request::Type::Toggle;
}

void Toggle::showPrompt(Menu * callingMenu)
{
	SUI_UNUSED_PARAM(callingMenu);
	// don't show -- it's auto.

}

bool Toggle::getValue(Menu * callingMenu, bool * into) {

	SUI_UNUSED_PARAM(callingMenu);

	bool val = ! *the_value;

	SUI_OUTPUT_DEBUG_DEFAULTSTREAM(F("Togg::val: "));
	SUI_OUTPUTLN_DEBUG_DEFAULTSTREAM(val ? F("TRUE") : F("FALSE"));

	*into = val;
	return true; // has indeed changed

}


char Long::numericRequestSubtype() {
	return (char)Request::Type::Int;
}

bool Long::getValue(Menu * callingMenu, long int * into) {
	long int val = callingMenu->driver()->parseInt();


	SUI_OUTPUT_DEBUG_DEFAULTSTREAM(F("Long::val: "));
	SUI_OUTPUTLN_DEBUG_DEFAULTSTREAM(val);

	bool hasChanged = false;
	if (val != *the_value)
	{
		hasChanged = true;
	}

	*into = val;

	return hasChanged;

}



BoundedLong::BoundedLong(long int * val, long int min, long int max,
		SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr, BoundedLong::validatorCallback vcb,
		MenuRequest_Callback cb):
			Long(val, key_pstr, help_pstr, vcb, cb),
			min_allowed(min),
			max_allowed(max),
			step_val(1)
{
	ensureSaneBounds();
	*val = min_allowed;
}

BoundedLong::BoundedLong(long int * val, long int min, long int max, SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
		MenuRequest_Callback cb) :
	Long(val, key_pstr, help_pstr, NULL, cb),
	min_allowed(min),
	max_allowed(max),
	step_val(1)
{
	ensureSaneBounds();
	*val = min_allowed;

}

void BoundedLong::ensureSaneBounds()
{
	if (min_allowed > max_allowed)
	{
			long int t = max_allowed;
			max_allowed = min_allowed;
			min_allowed = t;

	}

}

void BoundedLong::setStep(uint8_t val)
{
	if (val)
		step_val = val;

}

void BoundedLong::printMetaInfo(SUI::Menu * callingMenu)
{
	SerialUI * driver = callingMenu->driver();
	driver->print(this->getString(StaticString::MetaInfoSeparator));
	driver->print(*the_value);
	driver->print(SUI_SERIALUI_PROG_STR_SEP_CHAR);

	driver->print(min_allowed);
	driver->print(SUI_SERIALUI_PROG_STR_SEP_CHAR);
	driver->print(max_allowed);
	driver->print(SUI_SERIALUI_PROG_STR_SEP_CHAR);
	driver->print(step_val);




}

bool BoundedLong::valueIsValid(long int & val) {

	bool withinBounds = (val >= min_allowed && val <= max_allowed);

#ifdef SUI_INCLUDE_DEBUG
	if (withinBounds)
	{
		SUI_OUTPUTLN_DEBUG_DEFAULTSTREAM(F("Value within bounds"));
	} else {

		SUI_OUTPUT_DEBUG_DEFAULTSTREAM(F("V outta bounds. Should be:["));
		SUI_OUTPUT_DEBUG_DEFAULTSTREAM(min_allowed);
		SUI_OUTPUT_DEBUG_DEFAULTSTREAM(',');
		SUI_OUTPUT_DEBUG_DEFAULTSTREAM(max_allowed);
		SUI_OUTPUTLN_DEBUG_DEFAULTSTREAM(']');
	}

#endif
	return withinBounds;
}


char ULong::numericRequestSubtype() {
	return (char)Request::Type::UInt;
}

bool ULong::getValue(Menu * callingMenu, long unsigned int * into) {
	long unsigned int val = callingMenu->driver()->parseULong();
	SUI_OUTPUT_DEBUG_DEFAULTSTREAM(F("ULong::val: "));
	SUI_OUTPUTLN_DEBUG_DEFAULTSTREAM(val);

	bool hasChanged = false;
	if (val != *the_value)
	{
		hasChanged = true;
	}

	*into = val;
	return hasChanged;

}

char Float::numericRequestSubtype() {
	return (char)Request::Type::Float;
}


bool Float::getValue(Menu * callingMenu, float * into) {
	float val = callingMenu->driver()->parseFloat();
	bool hasChanged = false;
	if (val != *the_value)
	{
		hasChanged = true;
	}

	*into = val;
	return hasChanged;

}



void String::showPrompt(Menu * callingMenu) {

	callingMenu->driver()->showEnterDataPrompt();
}

void String::printMetaInfo(SUI::Menu * callingMenu)
{

	SerialUI * driver = callingMenu->driver();

	driver->print(this->getString(StaticString::MetaInfoSeparator));
	driver->print((int)max_length);
	driver->print(SUI_SERIALUI_PROG_STR_SEP_CHAR);
	if ( (*the_value).length() )
	{
		driver->print(*the_value);

	}

}
bool String::getValue(Menu * callingMenu, ::String* into) {
	*into = "";
	char * buf = new char[max_length + 1];
	SovA::Utils::DynCharBuffer dynBufDeleter(buf);

	SUI_OUTPUT_DEBUG_DEFAULTSTREAM(F("String::val: "));
	SUI_OUTPUTLN_DEBUG_DEFAULTSTREAM();

	if (! buf)
	{
		SUI_OUTPUTLN_DEBUG_DEFAULTSTREAM(F("nobuf!"));
		return false;
	}

	memset(buf, 0, max_length + 1);
	// buf[max_length] = 0;

	uint8_t numRead = callingMenu->driver()->readBytesToEOL(buf, max_length);

	if (! numRead)
	{
		SUI_OUTPUTLN_DEBUG_DEFAULTSTREAM(F("noread!"));
		return false;
	}

	/* debug...
	callingMenu->driver()->print(F("EOLSTR: "));
	for (uint8_t i=0; i<max_length; i++) {
		callingMenu->driver()->print(buf[i]);

	}
	*/

	// actually copy the buffer contents over
	into->concat(buf);

	// return whether we've actually changed the value
	return (*into != *the_value);
}

void String::printPrefix(Menu * callingMenu, SUI::Mode::Selection curMode)
{
	if (curMode == SUI::Mode::User)
	{
		return this->RequestBase<TopLevString>::printPrefix(callingMenu, curMode);
	}


	callingMenu->driver()->print(this->getString(StaticString::RequestStringPrefix));
}

/*

OptionsList::OptionsList(long int * val, SOVA_FLASHSTRING_PTR options[], uint8_t numOptions,
		SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
		MenuRequest_Callback cb) :
		BoundedLong(val, 1, numOptions, key_pstr, help_pstr, cb),
		opt_list(options)
{

}
*/



OptionsList::OptionsList(long int* val,
		SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
		OptionsList::validatorCallback vcb,
		SOVA_FLASHSTRING_PTR opt1, SOVA_FLASHSTRING_PTR opt2,
		SOVA_FLASHSTRING_PTR opt3, SOVA_FLASHSTRING_PTR opt4, SOVA_FLASHSTRING_PTR opt5, SOVA_FLASHSTRING_PTR opt6,
		MenuRequest_Callback cb) :
					BoundedLong(val, 1, 6, key_pstr, help_pstr, vcb, cb)
{

	opt_list[0] = opt1;
	opt_list[1] = opt2;
	opt_list[2] = opt3;
	opt_list[3] = opt4;
	opt_list[4] = opt5;
	opt_list[5] = opt6;

	calculateBounds(val);
}


OptionsList::OptionsList(long int* val,
		SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr,
		SOVA_FLASHSTRING_PTR opt1, SOVA_FLASHSTRING_PTR opt2,
		SOVA_FLASHSTRING_PTR opt3, SOVA_FLASHSTRING_PTR opt4, SOVA_FLASHSTRING_PTR opt5, SOVA_FLASHSTRING_PTR opt6,
		MenuRequest_Callback cb) :
					BoundedLong(val, 1, 6, key_pstr, help_pstr, cb)
{
	opt_list[0] = opt1;
	opt_list[1] = opt2;
	opt_list[2] = opt3;
	opt_list[3] = opt4;
	opt_list[4] = opt5;
	opt_list[5] = opt6;

	calculateBounds(val);
}

void OptionsList::calculateBounds(long int* val)
{


	for (uint8_t i=0; i<SUI_OPTIONSLIST_MAX_NUM; i++)
	{
		if (opt_list[i] == NULL)
		{
			this->setMaximum(i); // the last value + 1 == i
			break;
		}
	}

	if (! this->valueIsValid(*val))
	{
		*val = 1;
	}
}



void OptionsList::printMetaInfo(SUI::Menu * callingMenu) {

	SerialUI * driver = callingMenu->driver();

	driver->print(this->getString(StaticString::MetaInfoSeparator));
	driver->print(*the_value);
	for (uint8_t i=0; i<maximum(); i++)
	{

		driver->print(SUI_SERIALUI_PROG_STR_SEP_CHAR);
		driver->print(opt_list[i]);

	}
}

char OptionsList::numericRequestSubtype() {

	return (char)Request::Type::Options;
}

void OptionsList::showPrompt(Menu * callingMenu) {

	SerialUI * driver = callingMenu->driver();
	if (driver->mode() == SUI::Mode::User)
	{
		driver->println(' ');
		for (uint8_t i=0; i<maximum(); i++)
		{
			driver->print((int)(i+1));
			driver->print(':');
			driver->println(opt_list[i]);
		}
		driver->println(' ');
	}

	callingMenu->driver()->showEnterNumericDataPrompt();
}

}
}
}

