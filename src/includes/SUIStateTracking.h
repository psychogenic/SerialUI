/*
 * SUIStateTracking.h -- Tracked variables.
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


#ifndef SUISTATETRACKING_H_
#define SUISTATETRACKING_H_

#include "SUIConfig.h"

#ifdef BUILDING_DRUID

#ifndef SOVA_FLASHSTRING_PTR
#define SOVA_FLASHSTRING_PTR	char *
#endif
#else

#include "SUIPlatform.h"
#include "SUITypes.h"

#endif


#ifdef SUI_ENABLE_STATE_TRACKER
namespace SUI {
namespace Tracked {


class State {
public:
	State(SOVA_FLASHSTRING_PTR name) : state_name(name), sys_gen(false) {}
	virtual ~State() {}


	virtual void updateCache() {}
	virtual bool hasChanged() = 0;
	virtual SUI::Tracked::Type::ID type() = 0;
	virtual uint8_t toString(char * dest, uint8_t maxLen) = 0;

	inline SOVA_FLASHSTRING_PTR name() { return state_name;}

	inline bool systemGenerated() { return sys_gen;}
	inline void setSystemGenerated(bool setTo) { sys_gen = setTo;}
private:
	SOVA_FLASHSTRING_PTR state_name;
	bool sys_gen;

};

template<class TRACKTYPE>
class StateSimple: public SUI::Tracked::State {
public:
	StateSimple(SOVA_FLASHSTRING_PTR name, TRACKTYPE * var) : SUI::Tracked::State(name),
	ptr_val(var), last_value(0) {}


	virtual void updateCache() {
		last_value = getValue();
	}

	virtual TRACKTYPE getValue() { return *ptr_val;}

	virtual bool hasChanged() {
		return last_value != getValue();
	}


private:
	TRACKTYPE * ptr_val;
	TRACKTYPE last_value;

};


class Boolean : public StateSimple<bool> {

public:
	Boolean(SOVA_FLASHSTRING_PTR name, bool * var) : StateSimple(name, var) {}

	virtual SUI::Tracked::Type::ID type() { return Tracked::Type::Bool;}
	virtual uint8_t toString(char * dest, uint8_t maxLen);
};

class Float: public StateSimple<float> {
public:
	Float(SOVA_FLASHSTRING_PTR name, float * var) : StateSimple(name, var) {}
	virtual SUI::Tracked::Type::ID type() { return Tracked::Type::Float;}
	virtual uint8_t toString(char * dest, uint8_t maxLen);
};

class UInteger: public StateSimple<unsigned long> {
public:

	UInteger(SOVA_FLASHSTRING_PTR name, unsigned long * var) : StateSimple(name, var) {}
	virtual SUI::Tracked::Type::ID type() { return Tracked::Type::UInt;}
	virtual uint8_t toString(char * dest, uint8_t maxLen);
};
class Character: public StateSimple<char> {
public:

	Character(SOVA_FLASHSTRING_PTR name, char * var) : StateSimple(name, var) {}
	virtual SUI::Tracked::Type::ID type() { return Tracked::Type::Char;}
	virtual uint8_t toString(char * dest, uint8_t maxLen);
};



class CString: public SUI::Tracked::State {
public:
	CString(SOVA_FLASHSTRING_PTR name, char * var) : SUI::Tracked::State(name),
	ptr_val(var), last_value(NULL){}



	virtual void updateCache() {
		//last_value = ptr_val;
	}

	virtual const char* getValue() { return ptr_val;}
	virtual SUI::Tracked::Type::ID type() { return Tracked::Type::String;}

	virtual uint8_t toString(char * dest, uint8_t maxLen);
	virtual bool hasChanged() {
		return true;
	}


private:
	char * ptr_val;
	char * last_value;

};


class AString: public SUI::Tracked::State {
public:
	AString(SOVA_FLASHSTRING_PTR name, ::String * var) : SUI::Tracked::State(name),
	ptr_val(var) {}



	virtual void updateCache() {
		last_value = *ptr_val;
	}

	virtual ::String* getValue() { return ptr_val;}
	virtual SUI::Tracked::Type::ID type() { return Tracked::Type::String;}

	virtual uint8_t toString(char * dest, uint8_t maxLen);
	virtual bool hasChanged() {
		return (last_value != (*ptr_val));
	}


private:
	::String * ptr_val;
	::String last_value;

};





/*
typedef struct StateVariableDetailsStruct {

	TrackedType type;
	SOVA_FLASHSTRING_PTR name;
	union {
		State *	ptr_obj;
		float * 		ptr_float;
		unsigned long *	ptr_int;
		bool *			ptr_bool;
	};

	union {
		float 			lastval_float;
		unsigned long 	lastval_int;
		bool			lastval_bool;
	};

	StateVariableDetailsStruct(SOVA_FLASHSTRING_PTR name, TrackedType type, void * variable );
	StateVariableDetailsStruct(SOVA_FLASHSTRING_PTR name, State * variable );

	bool hasChanged();


} StateVariableDetails;

*/

} /* namespace Tracked */
} /* namespace SUI */

#endif




#endif /* SUISTATETRACKING_H_ */
