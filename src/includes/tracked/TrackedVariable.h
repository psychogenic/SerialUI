/*
 * TrackedVariable.h
 *
 *  Created on: May 26, 2018
 *      Author: Pat Deegan
 *
 *  TrackedVariable is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_TRACKED_TRACKEDVARIABLE_H_
#define SERIALUI_SRC_INCLUDES_TRACKED_TRACKEDVARIABLE_H_
#include "../SerialUITypes.h"
#include "../comm/CommSink.h"

namespace SerialUI {
namespace Tracked {

class State {
public:
	State();
	State(ID setId);

	State(StaticString name);
	State(StaticString name, ID setId);
	virtual ~State() {}
	inline bool hasChanged() const { return _hasChanged;}
	inline void changeRegistered() { _hasChanged = false;}

	virtual Type::Value type() = 0;


	virtual void printValueTo(Comm::Sink * toSink, bool asBinary=false) const = 0;


	inline ID id() const { return _id;}
	inline ID parentId() const { return 0;}
	inline StaticString name() const { return _name;}
	inline void setName(StaticString nm) { _name = nm;}





	template<class TYPE>
	TYPE * castAsSubType() {
		return (TYPE*)(this);
	}




protected:
	static ID id_counter;
	inline void markAsChanged() { _hasChanged = true;}
	ID _id;
	StaticString _name;
	bool _hasChanged;

};

// curiously recursive template here, so we can chain ops
template<Type::Value TYPEVAL, typename TRACKTYPE, class FINALTYPE>
class StateImplComparable : public State {

public:
	StateImplComparable(StaticString name) : State(name) {}
	StateImplComparable(StaticString name, TRACKTYPE v) : State(name), value(v) {}
	StateImplComparable(StaticString name, ID sid, TRACKTYPE v) : State(name, sid), value(v) {}

	virtual Type::Value type() { return TYPEVAL;}

	operator TRACKTYPE() { return value; }


	FINALTYPE& operator=(const TRACKTYPE & v) {

		this->assign(v);
		// this->markAsChanged();
		return *((FINALTYPE*)(this));
	}


	inline bool operator==(const StateImplComparable& rhs){ return (value == rhs.value); }
	// inline bool operator==(const TRACKTYPE& lhs, const StateImplComparable& rhs){ return (lhs == rhs.value); }
	inline bool operator==(const TRACKTYPE& rhs){ return (value == rhs); }


	inline bool operator!=(const StateImplComparable& rhs){ return !(*this == rhs); }
	inline bool operator!=(const TRACKTYPE& rhs){ return !(*this == rhs); }
	// inline bool operator!=(const TRACKTYPE& lhs, const StateImplComparable& rhs){ return !(lhs == rhs); }


protected:
	void assign(TRACKTYPE v) {
		// if (value != v) {
			markAsChanged();
		// }
		value = v;
	}
	TRACKTYPE value;

};







template<Type::Value TYPEVAL, class TRACKTYPE, class FINALTYPE>
class StateImplCountable : public StateImplComparable<TYPEVAL, TRACKTYPE, FINALTYPE> {

public:
	StateImplCountable(StaticString name): StateImplComparable<TYPEVAL, TRACKTYPE, FINALTYPE>(name) {}
	StateImplCountable(StaticString name, TRACKTYPE v) : StateImplComparable<TYPEVAL, TRACKTYPE, FINALTYPE>(name, v) {}
	StateImplCountable(StaticString name, ID sid, TRACKTYPE v) : StateImplComparable<TYPEVAL, TRACKTYPE, FINALTYPE>(name, sid, v) {}
	virtual ~StateImplCountable() {}

	using StateImplComparable<TYPEVAL, TRACKTYPE, FINALTYPE>::operator=;
	using StateImplComparable<TYPEVAL, TRACKTYPE, FINALTYPE>::operator==;
	using StateImplComparable<TYPEVAL, TRACKTYPE, FINALTYPE>::operator!=;

	FINALTYPE operator-() {
		return FINALTYPE(this->name(), this->id(), - this->value);
	}
	FINALTYPE operator+(TRACKTYPE b) {
		return FINALTYPE(this->name(),  this->id(),  this->value + b);
	}

	FINALTYPE operator*(TRACKTYPE b) {
		return FINALTYPE(this->name(),  this->id(),  this->value * b);
	}
	FINALTYPE operator/(TRACKTYPE b) {
		return FINALTYPE(this->name(),  this->id(),  this->value / b);
	}

    // overloaded prefix ++ operator
	FINALTYPE & operator++( ) {
		// save orig
		 this->value++;
		this->assign( this->value);
		// this->markAsChanged();
		return *((FINALTYPE*)(this));
	}

    // overloaded postfix ++ operator
	FINALTYPE operator++( int ) {
		// save orig
		FINALTYPE orig(this->name(),  this->id(),  this->value);
		 this->value++;
		this->assign( this->value);
		this->markAsChanged();
		return orig;
	}

    // overloaded prefix -- operator
	FINALTYPE & operator--( ) {
		 this->value--;
		this->assign( this->value);
		// this->markAsChanged();
		return *((FINALTYPE*)(this));
	}
    // overloaded postfix -- operator
	FINALTYPE operator--( int ) {
		// save orig
		FINALTYPE orig(this->name(),  this->id(),   this->value);
		 this->value--;
		this->assign( this->value);
		// this->markAsChanged();
		return orig;
	}

	FINALTYPE& operator+= (TRACKTYPE v) {
			TRACKTYPE o =  this->value;
			o += v;
			this->assign(o);
			return *((FINALTYPE*)(this));
	}
	FINALTYPE& operator-= (TRACKTYPE v) {
			TRACKTYPE o =  this->value;
			o -= v;
			this->assign(o);
			return *((FINALTYPE*)(this));
	}
	FINALTYPE& operator*= (TRACKTYPE v) {

			TRACKTYPE o =  this->value;
			o *= v;
			this->assign(o);
			return *((FINALTYPE*)(this));
	}
	FINALTYPE& operator/= (TRACKTYPE v) {

			TRACKTYPE o =  this->value;
			o /= v;
			this->assign(o);
			return *((FINALTYPE*)(this));
	}
	/*
	int operator%(int m) {
			return  this->value % m;
	}
	*/

	virtual void printValueTo(Comm::Sink * toSink, bool asBinary=false) const {
		if (asBinary) {
			uint8_t * asP = (uint8_t*)&(this->value);
			for (uint8_t i=0; i< sizeof(this->value); i++) {
				toSink->write(asP[i]);
			}
		} else {
			toSink->print(this->value);
		}
	}

	inline bool operator< (const StateImplCountable& rhs){ return this->value < rhs.value; }
	// inline bool operator< (const TRACKTYPE& lhs, const StateImplCountable& rhs){ return value < rhs.value; }
	inline bool operator< ( const TRACKTYPE& rhs){ return this->value < rhs; }

	inline bool operator> (const TRACKTYPE& rhs){ return rhs < *this; }
	inline bool operator<=(const TRACKTYPE& rhs){ return !(*this > rhs); }
	inline bool operator>=( const TRACKTYPE& rhs){ return !(*this < rhs); }

/*
	inline bool operator> (const TRACKTYPE& lhs, const StateImplCountable& rhs){ return rhs < lhs; }
	inline bool operator<=(const TRACKTYPE& lhs, const StateImplCountable& rhs){ return !(lhs > rhs); }
	inline bool operator>=(const TRACKTYPE& lhs, const StateImplCountable& rhs){ return !(lhs < rhs); }
*/

	inline bool operator> (const StateImplCountable& rhs){ return rhs < *this; }
	inline bool operator<=(const StateImplCountable& rhs){ return !(*this > rhs); }
	inline bool operator>=(const StateImplCountable& rhs){ return !(*this < rhs); }



};





class Boolean: public StateImplComparable<Type::Bool, bool, Boolean>
{
public:
	Boolean(StaticString name) : StateImplComparable(name, false) {}
	Boolean(StaticString name, bool v) : StateImplComparable(name, v) {}
	Boolean(StaticString name, ID sid, bool v) : StateImplComparable(name, sid, v) {}

	using StateImplComparable<Type::Bool, bool, Boolean>::operator=;
	using StateImplComparable<Type::Bool, bool, Boolean>::operator==;
	using StateImplComparable<Type::Bool, bool, Boolean>::operator!=;

	virtual void printValueTo(Comm::Sink * toSink, bool asBinary=false) const {
		if (asBinary) {
			toSink->write(this->value ? 1 : 0);

		} else {
			toSink->print(this->value ? SUI_STR("true") : SUI_STR("false"));
		}
	}
};

typedef Boolean Toggle;

}
}


#endif /* SERIALUI_SRC_INCLUDES_TRACKED_TRACKEDVARIABLE_H_ */

