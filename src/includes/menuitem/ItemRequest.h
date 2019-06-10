/*
 * ItemRequest.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemRequest is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_MENUITEM_ITEMREQUEST_H_
#define SERIALUIV3_SRC_INCLUDES_MENUITEM_ITEMREQUEST_H_

#include "MenuItem.h"
#include "../Menu.h"

#include "requests/externalPythonValidation.h"


namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {


class Request : public Item {
public:
	typedef void(*ValueChangedCallback)(void);

	Request(Type::Value rType, ValueChangedCallback vc=NULL);
	Request(Type::Value rType, StaticString key, StaticString help, ValueChangedCallback vc=NULL);
	Request(ID id, Type::Value rType, StaticString key, StaticString help, ValueChangedCallback vc=NULL);
	virtual ~Request() {
		this->freeResources();
	}

	inline Type::Value requestType() { return _reqType;}

	virtual void freeResources() {} // called when a request will be forever destroyed...



protected:
	void callBegins();
	void callEnds(bool successful);

	void valueWasModified(); // call me when that's the case... duh.
	inline void setRequestType(Type::Value t) { _reqType = t;}

	Type::Value _reqType;
	ValueChangedCallback _valChangedCb;
};




template<Type::Value REQTYPE, class INPUTTYPE>
class TypedRequest : public Request {
public:
	typedef  bool(*ValidatorCallback)(INPUTTYPE & newVal);
	TypedRequest(
				INPUTTYPE initVal,
				ValueChangedCallback vchng=NULL,
				ValidatorCallback validcb=NULL) :
					Request(REQTYPE, vchng), _inValue(initVal), _validator(validcb)
		{

		}

	TypedRequest(
			INPUTTYPE initVal,
			StaticString key,
			StaticString help,
			ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL) :
				Request(REQTYPE, key, help, vchng), _inValue(initVal), _validator(validcb)
	{

	}

	TypedRequest(ID id, INPUTTYPE initVal, StaticString key,
			StaticString help, ValueChangedCallback vchng=NULL, ValidatorCallback validcb=NULL) :
				Request(id, REQTYPE, key, help, vchng), _inValue(initVal), _validator(validcb)
	{

	}

	TypedRequest(const TypedRequest & other) : Request(other) {
		_inValue = other._inValue;
		_validator = other._validator;
	}

	virtual void freeResources() {

	}

	operator INPUTTYPE() { return _inValue; }


	inline bool operator==(const TypedRequest& rhs){ return (_inValue == rhs._inValue); }
	inline bool operator==(const INPUTTYPE& rhs){ return (_inValue == rhs); }


	inline bool operator!=(const TypedRequest& rhs){ return !(*this == rhs); }
	inline bool operator!=(const INPUTTYPE& rhs){ return !(*this == rhs); }

	INPUTTYPE& operator= (const INPUTTYPE & v) {

		this->setValue(v);
		return _inValue;
	}

	INPUTTYPE & currentValue() {
		return _inValue;
	}

	virtual void setValue(INPUTTYPE v)
	{
		_inValue = v;
	}

	virtual bool valueIsValid(INPUTTYPE & v)
	{
		return true;
	}


	virtual bool getValue(Menu * callingMenu, INPUTTYPE * v) = 0;

	virtual bool externalValidation(INPUTTYPE & v) {
		if (_validator && (!_validator(v))) {
			return false;
		}

		return true;

	}

	virtual void call(Menu * callingMenu) {
		callBegins();
		{
			// scope for INPUTTYPE v
			INPUTTYPE v;

			bool getSucceeded = this->getValue(callingMenu, &v);
			if (! getSucceeded)
			{
				callEnds(false);
				return;
			}

			if ( ! this->externalValidation(v) || (! this->valueIsValid(v)) ) {
				callEnds(false);
				return;
			}

			this->setValue(v);
			// end of scope, any useless v is released here.
		}
		// trigger any callbacks
		this->valueWasModified();
		callEnds(true);
		// done
		return;

	}


protected:
	INPUTTYPE _inValue;
	ValidatorCallback _validator;
};


template<Type::Value REQTYPE, class INPUTTYPE, class FINALTYPE>
class CountableTypedRequest : public TypedRequest<REQTYPE, INPUTTYPE> {
public:
	typedef  bool(*ValidatorCallback)(INPUTTYPE & newVal);
	typedef Request::ValueChangedCallback ValueChangedCallback;
	CountableTypedRequest(
					INPUTTYPE initVal,
					ValueChangedCallback vchng=NULL,
					ValidatorCallback validcb=NULL) :
						TypedRequest<REQTYPE, INPUTTYPE>(initVal, vchng, validcb)
			{

			}

	CountableTypedRequest(
				INPUTTYPE initVal,
				StaticString key,
				StaticString help,
				ValueChangedCallback vchng=NULL,
				ValidatorCallback validcb=NULL) :
					TypedRequest<REQTYPE, INPUTTYPE>(initVal, key, help, vchng, validcb)
					{

		}

	CountableTypedRequest(ID id, INPUTTYPE initVal, StaticString key,
				StaticString help, ValueChangedCallback vchng=NULL, ValidatorCallback validcb=NULL) :
					TypedRequest<REQTYPE, INPUTTYPE>(id, initVal, key, help, vchng)
					{
		}



	using TypedRequest<REQTYPE, INPUTTYPE>::operator=;


	FINALTYPE operator-() {
		return FINALTYPE(this->id(), (- this->_inValue), this->key(), this->help(),
				this->_valChangedCb, this->_validator);
	}
	FINALTYPE operator+(INPUTTYPE b) {
		return FINALTYPE(this->id(), (this->_inValue + b), this->key(), this->help(),
						this->_valChangedCb, this->_validator);
	}

	FINALTYPE operator*(INPUTTYPE b) {

		return FINALTYPE(this->id(), (this->_inValue * b), this->key(), this->help(),
						this->_valChangedCb, this->_validator);
	}
	FINALTYPE operator/(INPUTTYPE b) {

		return FINALTYPE(this->id(), (this->_inValue / b), this->key(), this->help(),
						this->_valChangedCb, this->_validator);
	}


    // overloaded prefix ++ operator
	FINALTYPE & operator++( ) {
		 this->_inValue++;
		return *((FINALTYPE*)(this));
	}

    // overloaded postfix ++ operator
	FINALTYPE operator++( int ) {
		// save orig
		FINALTYPE orig(this->id(), (this->_inValue), this->key(), this->help(),
				this->_valChangedCb, this->_validator);
		 this->_inValue++;

		return orig;
	}

    // overloaded prefix -- operator
	FINALTYPE & operator--( ) {
		 this->_inValue--;

		return *((FINALTYPE*)(this));
	}
    // overloaded postfix -- operator
	FINALTYPE operator--( int ) {
		// save orig
		FINALTYPE orig(this->id(), (this->_inValue), this->key(), this->help(),
				this->_valChangedCb, this->_validator);
		this->_inValue--;
		return orig;
	}

	FINALTYPE& operator+= (INPUTTYPE v) {
			INPUTTYPE o =  this->_inValue;
			o += v;
			this->_inValue = o;
			return *((FINALTYPE*)(this));
	}
	FINALTYPE& operator-= (INPUTTYPE v) {
			INPUTTYPE o =  this->_inValue;
			o -= v;
			this->_inValue = o;
			return *((FINALTYPE*)(this));
	}
	FINALTYPE& operator*= (INPUTTYPE v) {

			INPUTTYPE o =  this->_inValue;
			o *= v;

			this->_inValue = o;
			return *((FINALTYPE*)(this));
	}
	FINALTYPE& operator/= (INPUTTYPE v) {

			INPUTTYPE o =  this->_inValue;
			o /= v;
			this->_inValue = o;
			return *((FINALTYPE*)(this));
	}



};

#define COUNTABLEREQCLASS_PARENT(tp, intp, finaltp) CountableTypedRequest<tp, intp, finaltp>

#define COUTABLEREQCLASS_USINGALLOPS(tp, intp, finaltp) \
	using COUNTABLEREQCLASS_PARENT(tp, intp, finaltp)::operator=; \
	using COUNTABLEREQCLASS_PARENT(tp, intp, finaltp)::operator==; \
	using COUNTABLEREQCLASS_PARENT(tp, intp, finaltp)::operator!=; \
	using COUNTABLEREQCLASS_PARENT(tp, intp, finaltp)::operator+; \
	using COUNTABLEREQCLASS_PARENT(tp, intp, finaltp)::operator++; \
	using COUNTABLEREQCLASS_PARENT(tp, intp, finaltp)::operator-; \
	using COUNTABLEREQCLASS_PARENT(tp, intp, finaltp)::operator--; \
	using COUNTABLEREQCLASS_PARENT(tp, intp, finaltp)::operator*; \
	using COUNTABLEREQCLASS_PARENT(tp, intp, finaltp)::operator/; \


} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_MENUITEM_ITEMREQUEST_H_ */
