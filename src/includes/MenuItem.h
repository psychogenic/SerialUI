#ifndef SUI_MENUITEM_H_
#define SUI_MENUITEM_H_
#include "SUIPlatform.h"
#include "SUITypes.h"
#include "SerialUI.h"
#include "SUIStrings.h"
namespace SUI {

class Menu; // forward decl

namespace MenuItem {




namespace Type {
	typedef enum {
		Command = 0,
		SubMenu,
		Request
	} Type;
}

class Base {
	public:

		SUI_FLASHSTRING_PTR key;
		SUI_FLASHSTRING_PTR help;
		uint8_t key_size;
		Type::Type item_type;

		Base();
		Base(SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr, Type::Type t);
		virtual ~Base() {}

		bool is(Type::Type t) { return t == item_type;}
		virtual Menu* call(Menu * callingMenu) = 0;

		virtual void printPrefix(Menu * callingMenu, SUI::Mode::Selection curMode) = 0;

	};


class SubMenu: public MenuItem::Base {
public:

	SubMenu(Menu * sub, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr) :
		MenuItem::Base(key_pstr, help_pstr, MenuItem::Type::SubMenu), subMenu(sub) {}

	Menu * subMenu;

	virtual Menu* call(Menu * callingMenu);
	virtual void printPrefix(Menu * callingMenu, SUI::Mode::Selection curMode) ;
};

class Command: public MenuItem::Base {
public:

	Command(MenuCommand_Callback cb, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr) :
		MenuItem::Base(key_pstr, help_pstr, MenuItem::Type::Command), command(cb) {}

		MenuCommand_Callback command;

		virtual Menu* call(Menu * callingMenu);

		virtual void printPrefix(Menu * callingMenu, SUI::Mode::Selection curMode) ;

};

template<class REQTYPE>
class Request : public MenuItem::Base {
public:

	typedef  bool(*validatorCallback)(REQTYPE & newVal);
	virtual ~Request() {}

	Request(REQTYPE * val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr, validatorCallback vcb, MenuRequest_Callback cb=NULL) :
		MenuItem::Base(key_pstr, help_pstr, MenuItem::Type::Request), the_value(val), validator(vcb), value_changed_cb(cb) {}

/*
	Request(REQTYPE * val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr, MenuRequest_Callback cb=NULL) :
		MenuItem::Base(key_pstr, help_pstr, MenuItem::Type::Request), the_value(val), validator(NULL), value_changed_cb(cb) {}
*/
	REQTYPE * the_value;
	validatorCallback validator;

	MenuRequest_Callback value_changed_cb;

	virtual void showPrompt(Menu * callingMenu) = 0;
	virtual bool getValue(Menu * callingMenu, REQTYPE * v) = 0;

	void printValue(Menu * callingMenu, REQTYPE v)
	{
		if (callingMenu->driver()->echoCommands())
			callingMenu->driver()->println(v);

	}
	virtual void setValue(REQTYPE v)
	{
		*the_value = v;
	}
	virtual Menu* call(Menu * callingMenu) {

		callingMenu->driver()->println(help);

		this->showPrompt(callingMenu);

		REQTYPE v;
		if (!this->getValue(callingMenu, &v) )
		{
			printValue(callingMenu, v);
			callingMenu->returnOK();
			return NULL;
		}


		printValue(callingMenu, v);

		if (validator)
		{
			if (! validator(v))
			{
				callingMenu->returnError("Invalid value");
				return NULL;
			}
		}
		this->setValue(v);

		if (value_changed_cb) {
			value_changed_cb();
		}

		callingMenu->returnOK();
		return NULL;
	}

	virtual void printPrefix(Menu * callingMenu, SUI::Mode::Selection curMode)
	{
		if (curMode == SUI::Mode::User)
		{
			callingMenu->driver()->print(SUI_STR(SUI_SERIALUI_KEYHELP_REQUEST_PREFIX));
		}

	}


};

class RequestChar : public Request<char> {
public:


	RequestChar(char * val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr, bool(*vcb)(char & newVal), MenuRequest_Callback cb=NULL) :
		Request<char>(val, key_pstr, help_pstr, vcb, cb) {}
/*
	RequestChar(char * val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr, MenuRequest_Callback cb=NULL) :
		Request<char>(val, key_pstr, help_pstr, cb) {}
		*/
	virtual void showPrompt(Menu * callingMenu) {

		callingMenu->driver()->showEnterDataPrompt();
	}
	virtual bool getValue(Menu * callingMenu, char * into) {

		char aChar;

		if (callingMenu->driver()->readBytes(&aChar, 1) < 1)
		{
			return false;
		}
		bool hasChanged = (aChar != *the_value);
		*into = aChar;
		return hasChanged;

	}
	virtual void printPrefix(Menu * callingMenu, SUI::Mode::Selection curMode)
	{
		if (curMode == SUI::Mode::User)
		{
			return this->Request<char>::printPrefix(callingMenu, curMode);
		}


		callingMenu->driver()->print(SUI_STR(SUI_SERIALUI_KEYHELP_REQUEST_CHAR_PREFIX_PROG));
	}
};

template<class REQTYPE>
class RequestNumeric : public Request<REQTYPE> {
public:

	RequestNumeric(REQTYPE * val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr, bool(*vcb)(REQTYPE & newVal), MenuRequest_Callback cb=NULL) :
		Request<REQTYPE>(val, key_pstr, help_pstr, vcb, cb) {}


	/*
	RequestNumeric(REQTYPE * val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr, MenuRequest_Callback cb=NULL) :
		Request<REQTYPE>(val, key_pstr, help_pstr, cb) {}
		*/


	virtual ~RequestNumeric() {}
	virtual void showPrompt(Menu * callingMenu) {

		callingMenu->driver()->showEnterNumericDataPrompt();
	}


	virtual void printPrefix(Menu * callingMenu, SUI::Mode::Selection curMode)
	{
		if (curMode == SUI::Mode::User)
		{
			return this->Request<REQTYPE>::printPrefix(callingMenu, curMode);
		}


		callingMenu->driver()->print(SUI_STR(SUI_SERIALUI_KEYHELP_REQUEST_NUM_PREFIX_PROG));
	}
};


class RequestBool: public RequestNumeric<bool>
{
public:

	RequestBool(bool * val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr, bool(*vcb)(bool & newVal), MenuRequest_Callback cb=NULL) :
		RequestNumeric<bool>(val, key_pstr, help_pstr, vcb, cb) {}


	/*
	RequestBool(bool * val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr, MenuRequest_Callback cb=NULL) :
		RequestNumeric<bool>(val, key_pstr, help_pstr, cb) {}
		*/

	virtual bool getValue(Menu * callingMenu, bool * into) {
		bool val = callingMenu->driver()->parseInt();

		bool hasChanged = false;
		if (val != *the_value)
		{
			hasChanged = true;
		}


		*into = val;
		return hasChanged;

	}
};

class RequestLong: public RequestNumeric<long int> {
public:


	RequestLong(long int * val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr, bool(*vcb)(long int &), MenuRequest_Callback cb=NULL) :
		RequestNumeric<long int>(val, key_pstr, help_pstr, vcb, cb) {}


	/*
	RequestLong(long int * val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr, MenuRequest_Callback cb=NULL) :
		RequestNumeric<long int>(val, key_pstr, help_pstr, cb) {}
		*/
	virtual bool getValue(Menu * callingMenu, long int * into) {
		long int val = callingMenu->driver()->parseInt();


		bool hasChanged = false;
		if (val != *the_value)
		{
			hasChanged = true;
		}

		*into = val;
		return hasChanged;

	}
};


class RequestULong: public RequestNumeric<long unsigned int> {
public:


	RequestULong(long unsigned int * val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr,bool(*vcb)(long unsigned int &), MenuRequest_Callback cb=NULL) :
		RequestNumeric<long unsigned int>(val, key_pstr, help_pstr, vcb, cb) {}


	/*
	RequestULong(long unsigned int * val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr, MenuRequest_Callback cb=NULL) :
		RequestNumeric<long unsigned int>(val, key_pstr, help_pstr, cb) {}
		*/


	virtual bool getValue(Menu * callingMenu, long unsigned int * into) {
		long unsigned int val = callingMenu->driver()->parseULong();

		bool hasChanged = false;
		if (val != *the_value)
		{
			hasChanged = true;
		}

		*into = val;
		return hasChanged;

	}
};

class RequestFloat: public RequestNumeric<float> {
public:


	RequestFloat(float * val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr,bool(*vcb)(float &), MenuRequest_Callback cb=NULL) :
		RequestNumeric<float>(val, key_pstr, help_pstr, vcb, cb) {}

/*
	RequestFloat(float * val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr, MenuRequest_Callback cb=NULL) :
		RequestNumeric<float>(val, key_pstr, help_pstr, cb) {}
*/
	virtual bool getValue(Menu * callingMenu, float * into) {
		long unsigned int val = callingMenu->driver()->parseFloat();
		bool hasChanged = false;
		if (val != *the_value)
		{
			hasChanged = true;
		}

		*into = val;
		return hasChanged;

	}
};


class RequestString: public Request<String> {
public:


	RequestString(String* val, uint8_t maxLength, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr, bool(*vcb)(String &), MenuRequest_Callback cb=NULL) :
		Request<String>(val, key_pstr, help_pstr, vcb, cb), max_length(maxLength) {}

/*
	RequestString(String* val, uint8_t maxLength, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr, MenuRequest_Callback cb=NULL) :
		Request<String>(val, key_pstr, help_pstr, cb),  max_length(maxLength) {}
*/
	uint8_t max_length;

	virtual void showPrompt(Menu * callingMenu) {

		callingMenu->driver()->showEnterDataPrompt();
	}
	virtual bool getValue(Menu * callingMenu, String* into) {
		*into = "n/a";

		char * buf = new char[max_length + 1];

		if (! buf)
		{
			return false;
		}

		memset(buf, 0, max_length + 1);

		uint8_t numRead = callingMenu->driver()->readBytesToEOL(buf, max_length);

		if (! numRead)
		{
			delete [] buf;
			return false;
		}

		String tmp(buf);

		bool strChanged = false;
		if (tmp != *the_value)
		{
			strChanged = true;
		}
		*into = buf;
		delete [] buf;

		return strChanged;


	}

	virtual void printPrefix(Menu * callingMenu, SUI::Mode::Selection curMode)
	{
		if (curMode == SUI::Mode::User)
		{
			return this->Request<String>::printPrefix(callingMenu, curMode);
		}


		callingMenu->driver()->print(SUI_STR(SUI_SERIALUI_KEYHELP_REQUEST_STR_PREFIX_PROG));
	}
};





#if 0
class RequestChar: public Request {
	public:
	RequestChar(char * val, SUI_FLASHSTRING_PTR key_pstr,
			SUI_FLASHSTRING_PTR help_pstr, MenuRequest_Callback cb=NULL) :
		Request(key_pstr, help_pstr, cb), the_value(val) {}

	char * the_value;
	virtual bool showPromptAndGetValue(Menu * callingMenu);


};

class RequestBool: public Request {
	public:
	RequestBool(bool * val, SUI_FLASHSTRING_PTR key_pstr,
			SUI_FLASHSTRING_PTR help_pstr, MenuRequest_Callback cb=NULL) :
		Request(key_pstr, help_pstr, cb), the_value(val) {}

	bool * the_value;
	virtual bool showPromptAndGetValue(Menu * callingMenu);


};

class RequestLong: public Request {
	public:
	RequestLong(long int * val, SUI_FLASHSTRING_PTR key_pstr,
			SUI_FLASHSTRING_PTR help_pstr, MenuRequest_Callback cb=NULL) :
		Request(key_pstr, help_pstr, cb), the_value(val) {}

	long int * the_value;
	virtual bool showPromptAndGetValue(Menu * callingMenu);


};


class RequestULong: public Request {
	public:
	RequestULong(long unsigned int * val, SUI_FLASHSTRING_PTR key_pstr,
			SUI_FLASHSTRING_PTR help_pstr, MenuRequest_Callback cb=NULL) :
		Request(key_pstr, help_pstr, cb), the_value(val) {}

	long unsigned int * the_value;
	virtual bool showPromptAndGetValue(Menu * callingMenu);


};

class RequestFloat: public Request {
	public:
	RequestFloat(float * val, SUI_FLASHSTRING_PTR key_pstr,
			SUI_FLASHSTRING_PTR help_pstr, MenuRequest_Callback cb=NULL) :
		Request(key_pstr, help_pstr, cb), the_value(val) {}

	float * the_value;
	virtual bool showPromptAndGetValue(Menu * callingMenu);

};

class RequestString: public Request {
public:
	RequestString(char * val, uint8_t maxLen, SUI_FLASHSTRING_PTR key_pstr,
			SUI_FLASHSTRING_PTR help_pstr, MenuRequest_Callback cb=NULL) :
		Request(key_pstr, help_pstr, cb), the_value(val), max_length(maxLen) {}

	char * the_value;
	uint8_t max_length;
	virtual bool showPromptAndGetValue(Menu * callingMenu);

};

#endif

}
}
#endif
