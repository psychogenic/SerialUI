/*
 * SUIStateTracking.cpp -- state tracking implementation
 * Copyright (C) 2013-2017 Pat Deegan, psychogenic.com.
 *
 *
 * http://www.flyingcarsandstuff.com/projects/SerialUI
 *
 *
 * This program library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See file LICENSE.txt for further informations on licensing
 * terms.
 *
 */


#include "includes/SUIStateTracking.h"


#ifdef SUI_ENABLE_STATE_TRACKER

namespace SUI {
namespace Tracked {

uint8_t Character::toString(char * dest, uint8_t maxLen)
{

	if (! maxLen)
		return 0;

	dest[0] = getValue();

	return 1;

}

uint8_t Boolean::toString(char * dest, uint8_t maxLen)
{
	if (! maxLen)
		return 0;

	if (getValue())
	{
		dest[0] = '1';
	} else {
		dest[0] = '0';
	}
	return 1;
}

uint8_t Float::toString(char * dest, uint8_t maxLen)
{
	uint8_t len=0;
	float dval = getValue();
	char outBuf[20] = {0,};

#ifdef PLATFORM_DESKTOP
	len= sprintf(outBuf, "%.2f", dval);
#else
	len = SOVA_CONVERT_FLOAT_TO_STRING_AND_RETLEN(dval, outBuf);
#endif


	if (len > maxLen)
	{
		len = maxLen;
	}

	if (! len)
		return 0;

	strncpy(dest, outBuf, len);

	return len;


}

uint8_t UInteger::toString(char * dest, uint8_t maxLen)
{
	uint8_t len = 0;
	char strBuf[20] = {0,};
	char * str = &(strBuf[18]);

	unsigned long m;
	unsigned long tmpInt = getValue();
	do {
		m = tmpInt;
		tmpInt /= 10;
		char c = m - 10 * tmpInt;
		*--str = c + '0';
		len++;
	} while (tmpInt);

	if (len > maxLen)
	{
		len = maxLen;
	}

	strncpy(dest, str, len);
	return len;
}

uint8_t CString::toString(char * dest, uint8_t maxLen)
{
	uint8_t slen = strlen(ptr_val);

	if (slen > maxLen)
	{
		slen = maxLen;
	}

	if (! slen)
		return 0;

	strncpy(dest, ptr_val, slen);

	return slen;


}

uint8_t AString::toString(char * dest, uint8_t maxLen)
{
	uint8_t strLen = ptr_val->length();
	if (maxLen < strLen)
	{
		strLen = maxLen;
	}

	if (! strLen)
		return 0;

	strncpy(dest, ptr_val->c_str(), strLen);
	return strLen;



}

/*
StateVariableDetailsStruct::StateVariableDetailsStruct(SOVA_FLASHSTRING_PTR n, State * variable ) :
		type(SUITracked_Object), name(n)
{
	ptr_obj = variable;
	lastval_int = 0;
}
StateVariableDetailsStruct::StateVariableDetailsStruct(SOVA_FLASHSTRING_PTR n,
		TrackedType t, void * variable ) : type(t), name(n)
{
	switch (type)
	{

	case SUITracked_Bool:
		ptr_bool = (bool*) variable;
		lastval_bool = *ptr_bool;
		break;
	case SUITracked_UInt:
		ptr_int = (unsigned long*) variable;
		lastval_int = *ptr_int;
		break;
	case SUITracked_Float:
		ptr_float = (float*) variable;
		lastval_float = *ptr_float;
		break;
	}
}


bool StateVariableDetails::hasChanged()
{

	switch (type)
	{

	case SUITracked_Bool:
		return (lastval_bool != *ptr_bool);
		break;
	case SUITracked_UInt:
		return (lastval_int != *ptr_int);
		break;
	case SUITracked_Float:
		return (lastval_float != *ptr_float);
		break;
	case SUITracked_Object:
		return ptr_obj->hasChanged();
		break;
	}

	return false;
}

*/

} /* namespace Tracked */
} /* namespace SUI */


#endif

