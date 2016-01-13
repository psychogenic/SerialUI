/*
 * SUIStream.cpp
 *
 *  Created on: Jan 7, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */


#include "includes/SUIPlatform.h"

#ifdef SUI_BASEIMPLEMENTATION_STANDARD

#include "includes/stream/SUIStream.h"
namespace SUI {

#ifdef SUI_PROGMEM_PTR
size_t SUIStream::print_P(SUI_PROGMEM_PTR message) {
	char p_buffer[SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN + 1];
	strncpy_P(p_buffer, message, SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN);

	return this->print(p_buffer);

}


size_t SUIStream::println_P(SUI_PROGMEM_PTR message) {
	char p_buffer[SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN + 1];
	strncpy_P(p_buffer, message, SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN);

	return this->println(p_buffer);


}
#endif


}

#endif /* SUI_BASEIMPLEMENTATION_STANDARD */
