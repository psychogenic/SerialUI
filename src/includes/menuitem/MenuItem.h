/*
 * MenuItem.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  MenuItem is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_MENUITEM_MENUITEM_H_
#define SERIALUIV3_SRC_INCLUDES_MENUITEM_MENUITEM_H_

#include "../SerialUITypes.h"


namespace SerialUI {
namespace Menu {

class Menu; // forward decl


namespace Item {

class Item {
public:
	Item(Type::Value itemType);
	Item(ID id, Type::Value itemType, StaticString key, StaticString help);
	Item(Type::Value itemType, StaticString key, StaticString help);
	virtual ~Item() {}

	virtual void call(Menu * callingMenu) = 0;

	virtual ID id() { return _id;}
	inline void setId(ID id) { _id = id;}

	Item * parent();

	inline ID parentId() { return _parentId;}
	inline void setParentId(ID pid) { _parentId = pid;}


	inline StaticStringLength lengthKey() {
		return stringLength(_key);
	}
	inline StaticStringLength lengthHelp() {
		return stringLength(_help);
	}

	inline void setKey(StaticString k) { _key = k;}
	inline void setHelp(StaticString h) { _help = h;}
	inline StaticString key() { return _key;}
	inline StaticString help() { return _help;}
	inline Type::Value type() { return _type;}


	template<class TYPE>
	TYPE * castAsSubType() {
		return (TYPE*)(this);
	}


	// positionInParent -- position (index) within parent
	// only applies to items within a parent (i.e. top level menu will return -1)
	// 0 indexed (first item has position "0")
	int8_t positionInParent();

protected:
	inline void setType(Type::Value t) { _type = t;}
private:
	static ID id_counter;
	StaticStringLength stringLength(StaticString str);

	ID _id;
	ID _parentId;
	Type::Value _type;
	StaticString _key;
	StaticString _help;
};

}
}
}




#endif /* SERIALUIV3_SRC_INCLUDES_MENUITEM_MENUITEM_H_ */
