/*
 * ChannelChannelArdStd.cpp
 *
 *  Created on: May 26, 2018
 *      Author: Pat Deegan
 *
 *  ChannelChannelArdStd is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/comm/CommFieldBoundaries.h"
#include "includes/comm/CommChannelArdStd.h"
#include "includes/SUIGlobals.h"
#include "includes/SerialUIStrings.h"
#include "includes/SerialUIPlatform.h"
#include "includes/menuitem/requests/requests.h"
#include "includes/menuitem/trackedview/trackedviews.h"


#define CHANNEL_READBUF_MINSIZE_PADDING		5


#define PROGCHANNEL_OUTFIELD(f)		_serPort->print((char)(f))




namespace SerialUI {
namespace Comm {

#ifdef SERIALUI_CHANNELS_USE_STANDARDSTREAMS


typedef struct StrAndLenStruct {
	StaticString str;
	uint8_t strLength;
	StrAndLenStruct() : str(NULL), strLength(0) {}
	StrAndLenStruct(StaticString s) : str(s) {
		strLength = SUI_PLATFORM_STATICSTRING_LENGTH(s);
	}
} StringAndLength;



ChannelModeUser::ChannelModeUser(Mode::Selection forMode, SourceType * port) : SerialChannel(forMode)
{

}




bool ChannelModeUser::getNextRequest(Menu::Item::ID inMenu, Request * into) {

	uint8_t maxLen = SUI_BUILTIN_REQUESTS_MAXSIZE;
	char * rawReadBuf = NULL;
	uint8_t lenRead = 0;

	into->clear(); // make sure it's no longer valid

#ifdef SERIAL_UI_ENABLE_USBSERIAL_TERMINALACCESS

	Menu::Item::SubMenu * curSubMenu = Globals::menuStructure()->getSubMenuById(
			inMenu);
	if (!curSubMenu) {
		return false;
	}

	uint8_t numSubItems = curSubMenu->numItems();

	if (numSubItems) {

		for (uint8_t i = 0; i < numSubItems; i++) {

			Menu::Item::Item * itm = curSubMenu->itemByIndex(i);
			if (! itm) {
				continue;
			}
			switch (itm->type()) {
			case Menu::Item::Type::DynamicText:
				/* fall-though	*/
			case Menu::Item::Type::StaticText:
				/* fall-though	*/
			case Menu::Item::Type::State:
				/* fall-though	*/
			case Menu::Item::Type::TrackedView:
				// ignore these--not exec-able...
				continue;
			default:
				break;
			}
			uint8_t slen = SUI_PLATFORM_STATICSTRING_LENGTH(itm->key());
			if (slen > maxLen) {
				maxLen = slen;
			}
		}


		rawReadBuf = new char[maxLen + (CHANNEL_READBUF_MINSIZE_PADDING + 1)];
		if (!rawReadBuf) {
			// problem!
			_serPort->println(F("OUTTA MEM!"));
			return false;
		}

		memset(rawReadBuf, 0, maxLen + (CHANNEL_READBUF_MINSIZE_PADDING + 1));
		lenRead = this->readUntilEOF(rawReadBuf, maxLen + CHANNEL_READBUF_MINSIZE_PADDING);

		if (lenRead) {

			SERIALUI_DEBUG_OUT(F("XX: "));
			SERIALUI_DEBUG_OUTLN(rawReadBuf);
			bool trimDone = false;
			for (uint8_t i = maxLen; (i > 1 && !trimDone); i--) {
				if (rawReadBuf[i] == '\0' || rawReadBuf[i] == ' '
						|| rawReadBuf[i] == '\r' || rawReadBuf[i] == '\n') {
					rawReadBuf[i] = '\0';
				} else {
					trimDone = true;
				}
			}

			uint8_t idx = 0;
			while (idx < numSubItems && !into->isValid()) {
				Menu::Item::Item * itm = curSubMenu->itemByIndex(idx);

				if (! itm) {
					idx++;
					continue;
				}
				switch (itm->type()) {
				case Menu::Item::Type::DynamicText:
					/* fall-though	*/
				case Menu::Item::Type::StaticText:
					/* fall-though	*/
				case Menu::Item::Type::State:
					/* fall-though	*/
				case Menu::Item::Type::TrackedView:
					// ignore these--not exec-able...
					idx++;
					continue;
				default:
					break;
				}
				if (itm && staticStringMatch(itm->key(), rawReadBuf, true)) {
					// we have a match!
					into->setMenuItem(itm);
				} else {
					idx++;
				}
			}
			if (into->isValid()) {

				delete[] rawReadBuf; // deleting the buf alloc'ed in this scope
				rawReadBuf = NULL;
				return true;
			}

		}

	}

#endif
	// no go.  If read buf has been alloc'd it means we've handled the input side already.
	// if not, we need to read in some bytes
	if (! rawReadBuf ) {
		rawReadBuf = new char[maxLen + (CHANNEL_READBUF_MINSIZE_PADDING+1)];
		if (rawReadBuf ) {

			memset(rawReadBuf, 0, maxLen + (CHANNEL_READBUF_MINSIZE_PADDING+1));
			lenRead = this->readUntilEOF(rawReadBuf, maxLen + CHANNEL_READBUF_MINSIZE_PADDING);
			// lenRead = _serPort->readBytesUntil('\n', rawReadBuf, maxLen);
		}
	}

	if (! lenRead) {
		delete [] rawReadBuf;
		return false;
	}

	bool retVal = getBuiltinRequest(rawReadBuf, into);
	delete [] rawReadBuf;

	return retVal;

}

size_t ChannelModeUser::printFieldSeparator(Menu::Item::Type::Value tp) {
#ifdef SERIAL_UI_ENABLE_USBSERIAL_TERMINALACCESS
	switch (tp) {
		case Menu::Item::Type::State:
			return _serPort->print(SUI_STR(": "));
		case Menu::Item::Type::List:
			/* fall-through */
		case Menu::Item::Type::Group:
			/* fall-through */
		case Menu::Item::Type::Menu:
			return _serPort->print('\t');
		default:
			return 0;
	}
#else
	return 0;
#endif
}

size_t ChannelModeUser::printFieldEnd(Menu::Item::Type::Value tp) {
	return _serPort->println(' ');
}

size_t ChannelModeUser::printHelpFieldStart(Menu::Item::Item* itm) {
	size_t rVal = _serPort->print('\t');


#ifdef SERIAL_UI_ENABLE_USBSERIAL_TERMINALACCESS
	switch(itm->type()) {
	case Menu::Item::Type::Command:
		rVal += _serPort->print('*');
		break;

		break;
	case Menu::Item::Type::Input:
		rVal += _serPort->print('>');
		break;
	case Menu::Item::Type::List:
		/* fall-through */
	case Menu::Item::Type::Group:
		/* fall-through */
	case Menu::Item::Type::Menu:
		rVal += _serPort->print('+');
		break;

	case Menu::Item::Type::DynamicText:
		/* fall-through */
	case Menu::Item::Type::StaticText:
		rVal += _serPort->print(SUI_STR("--- "));
		break;
	default:
		break;

	}
#endif

	return rVal;
}

size_t ChannelModeUser::printHelpFieldEnd(Menu::Item::Item* itm) {
	return _serPort->println(' ');
}

size_t ChannelModeUser::printHelpFieldSeparator(Menu::Item::Item* itm,
		size_t curLen) {

	return printSpaces(curLen);

}

void ChannelModeUser::printErrorStart() {
	return;
}
void ChannelModeUser::printErrorEnd() {
	_serPort->println(' ');
}

#if 0
DEADBEEF

bool ChannelModeUser::getBuiltinRequest(char * userString, Request * into) {
	SubMenuCallbackMap builtins[] = {
			SubMenuCallbackMap(SUI_STR(SUI_SERIALUI_HELP_COMMAND),
					SerialUI::Request::BuiltIn::Help),
			SubMenuCallbackMap(SUI_STR(SUI_SERIALUI_UP_COMMAND),
							SerialUI::Request::BuiltIn::UpLevel),
			SubMenuCallbackMap(SUI_STR(SUI_SERIALUI_EXIT_COMMAND),
											SerialUI::Request::BuiltIn::Exit),
			SubMenuCallbackMap(SUI_STR(SUI_SERIALUI_MODEPROG_COMMAND),
											SerialUI::Request::BuiltIn::ModeProgram),

			SubMenuCallbackMap(NULL, SerialUI::Request::BuiltIn::INVALID)
	};

	uint8_t idx=0;
	// _serPort->print(F("GOT: '"));
	// _serPort->print(userString);
	// _serPort->println('\'');
	while (builtins[idx].builtin != SerialUI::Request::BuiltIn::INVALID) {
		if (staticStringMatch(builtins[idx].cmdstr, userString, true)) {
			into->setBuiltIn(builtins[idx].builtin);
			return true;
		} else {
			_serPort->println(builtins[idx].cmdstr);
		}
		idx++;
	}

	return false;
}
#endif

size_t ChannelModeUser::printSpaces(uint8_t lenTaken, uint8_t lenRequired) {
#ifdef SERIAL_UI_ENABLE_USBSERIAL_TERMINALACCESS
	if (lenTaken >= lenRequired) {
		return 0;
	}
	uint8_t numToPrint = lenRequired - lenTaken;
	for (uint8_t i=0; i<numToPrint; i++) {
		_serPort->print(' ');
	}
	return numToPrint;
#else
	return 0;
#endif
}

void ChannelModeUser::showPrompt() {
	_serPort->println(' ');
	_serPort->print(Globals::state()->currentMenu()->key());
	_serPort->print('>');
}


void ChannelModeUser::printInputRequest(Menu::Item::Request::Request * req) {
	_serPort->print(req->key());
#ifdef SERIAL_UI_ENABLE_USBSERIAL_TERMINALACCESS
	_serPort->print(SUI_STR(" ("));
	printRequestCurrentValue(req);
	_serPort->print(SUI_STR(") ["));
	Menu::Item::Request::Type::Value reqType = req->requestType();
	switch (reqType) {
		case Menu::Item::Request::Type::Boolean:
			/* fall - through */
		case Menu::Item::Request::Type::Toggle:
			_serPort->print( SUI_STR("0-1"));
			break;

		case Menu::Item::Request::Type::OptionsList:
			/* fall -through */
		case Menu::Item::Request::Type::BoundedLongInt:
			_serPort->print( req->castAsSubType<Menu::Item::Request::BoundedLong>()->minimum());
			_serPort->print('-');
			_serPort->print( req->castAsSubType<Menu::Item::Request::BoundedLong>()->maximum());
			break;

		case Menu::Item::Request::Type::Time:

			/* fall -through */
		case Menu::Item::Request::Type::DateTime:

			/* fall -through */
		case Menu::Item::Request::Type::Float:
			/* fall -through */

		case Menu::Item::Request::Type::LongInt:

			/* fall -through */
		case Menu::Item::Request::Type::Color:

			/* fall -through */
		case  Menu::Item::Request::Type::UnsignedLongInt:
			_serPort->print(SUI_STR("num"));
			break;

		case Menu::Item::Request::Type::Character:
			_serPort->print(SUI_STR("char"));
			break;

		case Menu::Item::Request::Type::Passphrase:
			/* fall-through */
		case Menu::Item::Request::Type::String:
			_serPort->print(SUI_STR("str"));
			break;
		default:
			_serPort->print('?');
			break;



		}

#endif
	_serPort->print(SUI_STR("] ?"));

#ifdef SERIAL_UI_ENABLE_USBSERIAL_TERMINALACCESS
	if (reqType == Menu::Item::Request::Type::OptionsList) {
		Menu::Item::Request::OptionsList * optList = req->castAsSubType<Menu::Item::Request::OptionsList>();
		_serPort->println(' ');
		for (uint8_t i=0; i< optList->numOptions(); i++) {
			_serPort->print('\t');
			_serPort->print((int)(i+1));
			_serPort->print(':');
			_serPort->println(optList->optionByIndex(i));
		}
	}
#endif



}




#ifdef SERIALUI_AUTHENTICATOR_ENABLE
	// Access control
size_t ChannelModeUser::printAccessGrantRequest(Auth::Authenticator * auth) {
	size_t retSize = 0;
	Auth::Challenge chal = auth->challenge();
	if (chal) {
		retSize = _serPort->print(chal);
		retSize += _serPort->print(' ');
	}

	switch(auth->encoding()) {
	case Auth::Transmission::Type::Plain:
		retSize += _serPort->print(SUI_STR("password: "));
		break;
	case Auth::Transmission::Type::MD5:
		retSize += _serPort->print(SUI_STR("(MD5)"));
		break;
	default:
		// unsupported
		retSize += _serPort->print(SUI_STR("Use druid!"));
	}

	return retSize;

}

size_t ChannelModeUser::printAccessConfigureRequest(Auth::Authenticator * auth) {
	return _serPort->print(SUI_STR("set password: "));
}
#endif





/* **************************** PROG ************************ */

















ChannelModeProg::ChannelModeProg(Mode::Selection forMode, SourceType * port) : SerialChannel(forMode)
{

}


bool ChannelModeProg::parseBinCommandReq(Request * into) {

	uint8_t vals[] = {
		SERIAL_UI_PROGCHANFIELD_END
	};
	uint8_t rawReadBuf[20];
	// on a bin command, this should stop at the first character...
	uint8_t lenRead = this->readUntilAny(rawReadBuf, 20, vals, 1);
	if (lenRead < 2 ) {
		into->requestId = SERIAL_UI_REQUEST_NOINPUT;
		SERIALUI_DEBUG_OUTLN(F("Bin comm to short"));
		return false;
	}
	flushAllWhitespaces();

	into->requestId = rawReadBuf[0];
	/*
	_serPort->print(F("CHECK BIN COMM for itm"));
	_serPort->println((int)rawReadBuf[1]);
	*/
	Menu::Item::Item * item = Globals::menuStructure()->getItemById(rawReadBuf[1]);
	if (item) {
		into->setMenuItem(item);
		SERIALUI_DEBUG_OUT(F("Got MNU itm: "));
		SERIALUI_DEBUG_OUTLN((int)item->id());
		// _serPort->println(F("GOT MENUITEM!"));
		return true;
	}
	this->printError(F("Can't find a menu item?? "));
	SERIALUI_DEBUG_OUTLN((int)rawReadBuf[1]);
	this->printErrorStart();
	_serPort->print(F("NO ITEM W/ID "));
	_serPort->println((int)rawReadBuf[1]);
	_serPort->print(F(":"));
	for (uint8_t e=0; e<lenRead; e++) {
		_serPort->print((int)rawReadBuf[e], HEX);
		_serPort->print(',');
	}
	this->printErrorEnd();

	_serPort->flush();

	return false;

}

#define CHANNELPROG_RAWINPUTBUFSIZE		25

bool ChannelModeProg::getNextRequest(Menu::Item::ID inMenu, Request * into) {
	uint8_t vals[] = {
			SERIAL_UI_PROGCHANREQUEST_HEADER_COMMAND,
			SERIAL_UI_PROGCHANREQUEST_HEADER_REQINPUT,
			SERIAL_UI_PROGCHANREQUEST_HEADER_SYSCOMMAND,
			'\r',
			'\n'
	};

	SERIALUI_DEBUG_OUTLN(F("(PGM) Get next request"));

	uint8_t rawReadBuf[CHANNELPROG_RAWINPUTBUFSIZE];

	into->clear(); // make sure it's no longer valid

	flushAllWhitespaces();

	// on a bin command, this should stop at the first character...
	uint8_t lenRead = this->readUntilAny(rawReadBuf, CHANNELPROG_RAWINPUTBUFSIZE, vals, 5);

	if (! lenRead) {
		SERIALUI_DEBUG_OUTLN(F("Nothing read in--no input"));
		into->requestId = SERIAL_UI_REQUEST_NOINPUT;
		return false;
	}

#ifdef SERIALUI_ENABLE_DEBUG_OUTPUT
	SERIALUI_DEBUG_OUT(F("Got a buf in: "));
	for (uint8_t i=0; i<lenRead; i++) {
		SERIALUI_DEBUG_OUT((int)rawReadBuf[i]);
		SERIALUI_DEBUG_OUT(' ');
	}
#endif

	// uint8_t i;
	switch (rawReadBuf[0]) {
	case SERIAL_UI_PROGCHANREQUEST_HEADER_COMMAND:
		/* fall-through */
	case SERIAL_UI_PROGCHANREQUEST_HEADER_REQINPUT:
		/* fall-through */
	case SERIAL_UI_PROGCHANREQUEST_HEADER_SYSCOMMAND:
		/* fall-through */

		return parseBinCommandReq(into);
	default:

#ifdef SERIALUI_ENABLE_DEBUG_OUTPUT
		SERIALUI_DEBUG_OUTLN(F("UNKNOWN BYTES:"));
		for (uint8_t i=0; i<lenRead; i++) {
			if (rawReadBuf[i] >= 0x20 && rawReadBuf[i] < 0x7f) {
				SERIALUI_DEBUG_OUT((char)rawReadBuf[i]);
			} else {
				SERIALUI_DEBUG_OUT('|');
				SERIALUI_DEBUG_OUT((int)rawReadBuf[i]);
			}
		}
#endif
		/*
		this->printError(F("Unknown Hbytes:"));
		this->printErrorStart();
		for (i=0; i<lenRead; i++) {
			_serPort->print((int)rawReadBuf[i], HEX);
			_serPort->print(':');
		}
		this->printErrorEnd();

		*/

		break;

	}



	bool retVal = getBuiltinRequest((char*)rawReadBuf, into);
	return retVal;

}




size_t ChannelModeProg::printFieldStart(Tracked::State *state) {
	size_t rVal = PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_STATE_START);
	rVal += _serPort->print((char)state->type());
	rVal += _serPort->print((char)state->id());
	return rVal;

}
size_t ChannelModeProg::printFieldStart(Menu::Item::Item * itm) {
	size_t rVal = PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_ITEM_START);
	rVal += _serPort->print((char) itm->type());
	rVal += _serPort->print((char) itm->id());

	return rVal;

}



size_t ChannelModeProg::printFieldSeparator(Menu::Item::Type::Value tp) {
	switch (tp) {
		case Menu::Item::Type::State:
			return PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_STATE_SEP);
		case Menu::Item::Type::List:
			/* fall-through */
		case Menu::Item::Type::Group:
			/* fall-through */
		case Menu::Item::Type::Menu:
			return PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_ITEM_SEP);
		default:
			return 0;
	}
}



size_t ChannelModeProg::printFieldEnd(Menu::Item::Type::Value tp) {
	return PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_END);
}


size_t ChannelModeProg::printUpdateFieldStart(Menu::Item::Item * itm) {
	return PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_REQVARUPDATE_START);
}
size_t ChannelModeProg::printHelpFieldStart(Menu::Item::Item* itm) {

	Menu::Item::Request::Request * req = NULL;
	Menu::Item::TrackedView * tv = NULL;
	size_t rVal = PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_HELP_START);
	rVal += _serPort->print((char)itm->type());
	rVal += _serPort->print((char)itm->parentId());
	rVal += _serPort->print((char)itm->id());
	if (itm->type() == Menu::Item::Type::Input) {

		req = itm->castAsSubType<Menu::Item::Request::Request>();
		rVal += _serPort->print((char) (req->requestType()));
	} else if (itm->type() == Menu::Item::Type::TrackedView) {
		tv = itm->castAsSubType<Menu::Item::TrackedView>();
		rVal += _serPort->print((char) (tv->viewType()));
	}

	return rVal;
}

size_t ChannelModeProg::printHelpFieldData(Menu::Item::Item * itm) {

	Menu::Item::Type::Value itmType = itm->type();

	if (itmType == Menu::Item::Type::Input) {
		return printHelpFieldDataForRequest(itm);
	}
	if (itmType == Menu::Item::Type::TrackedView) {
		return printHelpFieldDataForTrackedView(itm);
	}

	return 0;



}

size_t ChannelModeProg::printHelpFieldDataForTrackedView(Menu::Item::Item * itm) {
	Menu::Item::TrackedView * tv = itm->castAsSubType<Menu::Item::TrackedView>();
	size_t rVal = this->printHelpFieldSeparator(itm);
	uint8_t num = tv->numAssociated();
	PROGCHANNEL_OUTFIELD(num);
	rVal++;
	for (uint8_t i=0; i<num; i++) {
		Tracked::State * st = tv->associatedByIdx(i);
		if (st) {
			PROGCHANNEL_OUTFIELD(st->id());
			rVal++;
		}

	}
	return rVal;

}
size_t ChannelModeProg::printHelpFieldDataForRequest(Menu::Item::Item * itm) {

	Menu::Item::Request::Request * req = itm->castAsSubType<Menu::Item::Request::Request>();
	size_t rVal = this->printHelpFieldSeparator(itm);
	rVal += printRequestCurrentValue(req);

	Menu::Item::Request::Type::Value reqType = req->requestType();
	switch (reqType) {
		case Menu::Item::Request::Type::Passphrase:
			/* fall-through */
		case Menu::Item::Request::Type::String:
			rVal += this->printHelpFieldSeparator(itm);
			rVal += _serPort->print( req->castAsSubType<Menu::Item::Request::String>()->maximumLength());
			break;
		case Menu::Item::Request::Type::OptionsList:
			/* fall -through */
		case Menu::Item::Request::Type::BoundedLongInt:
			rVal += this->printHelpFieldSeparator(itm);
			rVal += _serPort->print( req->castAsSubType<Menu::Item::Request::BoundedLong>()->minimum());
			rVal += this->printHelpFieldSeparator(itm);
			rVal += _serPort->print( req->castAsSubType<Menu::Item::Request::BoundedLong>()->maximum());
			break;
		default:
			return rVal;


		}
	if (reqType == Menu::Item::Request::Type::OptionsList) {
		Menu::Item::Request::OptionsList * optList = req->castAsSubType<Menu::Item::Request::OptionsList>();

		for (uint8_t i=0; i< optList->numOptions(); i++) {
			rVal += this->printHelpFieldSeparator(itm);
			rVal += _serPort->print(optList->optionByIndex(i));
		}
	}
	return rVal;
}


size_t ChannelModeProg::printHelpFieldEnd(Menu::Item::Item* itm) {
	return PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_HELP_END);
}

size_t ChannelModeProg::printHelpFieldSeparator(Menu::Item::Item* itm,
		size_t curLen) {

	return  PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_HELP_SEP);

}

void ChannelModeProg::outputRequestContextId() {
	if (Globals::state()->haveCurrentRequestContext()) {

			PROGCHANNEL_OUTFIELD(Globals::state()->currentRequestContext()->requestId);
	} else {

			PROGCHANNEL_OUTFIELD(0);
	}
}
void ChannelModeProg::printErrorStart() {
	PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_ERROR_START);
	outputRequestContextId();

}
void ChannelModeProg::printErrorEnd() {
	PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_ERROR_END);
}

/*

bool ChannelModeProg::getBuiltinRequest(char * userString, Request * into) {
	SubMenuCallbackMap builtins[] = {
			SubMenuCallbackMap(SUI_STR(SUI_SERIALUI_HELP_COMMAND),
					SerialUI::Request::BuiltIn::Help),
			SubMenuCallbackMap(SUI_STR(SUI_SERIALUI_UP_COMMAND),
							SerialUI::Request::BuiltIn::UpLevel),
			SubMenuCallbackMap(SUI_STR(SUI_SERIALUI_EXIT_COMMAND),
											SerialUI::Request::BuiltIn::Exit),
			SubMenuCallbackMap(SUI_STR(SUI_SERIALUI_MODEPROG_COMMAND),
											SerialUI::Request::BuiltIn::ModeProgram),

			SubMenuCallbackMap(SUI_STR(SUI_SERIALUI_KEEPALIVE_COMMAND),
													SerialUI::Request::BuiltIn::RefreshTracked),

			SubMenuCallbackMap(NULL, SerialUI::Request::BuiltIn::INVALID)
	};

	SERIALUI_DEBUG_OUTLN(F("Looking for builtin..."));
	uint8_t idx=0;
	while (builtins[idx].builtin != SerialUI::Request::BuiltIn::INVALID) {
		if (staticStringMatch(builtins[idx].cmdstr, userString, true)) {
			into->setBuiltIn(builtins[idx].builtin);
			flushAllWhitespaces();
			return true;
		}
		idx++;
	}

	//
		this->printError(F("Unknown Rbytes:"));
		this->printErrorStart();
		uint8_t i=0;
		while (i < 30 && userString[i]) {
			_serPort->print((int)userString[i++], HEX);
			_serPort->print(':');
		}
		this->printErrorEnd();
		//
	return false;
}
*/


size_t ChannelModeProg::printHelpListStart(Menu::Item::SubMenu * mnu) {
	size_t rVal = PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_MENULISTING_START);
	rVal += _serPort->print((char)Globals::menuStructure()->numItems());
	rVal += PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_SEP);
	if (Globals::state()->uid()) {
		rVal += _serPort->print(Globals::state()->uid());
	}
	rVal += PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_SEP);
	if (Globals::state()->greeting()) {
		rVal += _serPort->print(Globals::state()->greeting());
	}
	rVal += PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_END);
	return rVal;
}
size_t ChannelModeProg::printHelpListEnd(Menu::Item::SubMenu * mnu) {

	size_t rVal = PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_MENULISTING_END);
	rVal += _serPort->print((char)Globals::menuStructure()->numItems());
	rVal += PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_END);
	return rVal;
}

void ChannelModeProg::showPrompt() {
	return;
}

size_t ChannelModeProg::printCommandProcessingStart() {
	PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_COMMANDPROC_START);
	outputRequestContextId();
	return 2;
}

size_t ChannelModeProg::printCommandProcessingDone() {

	PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_COMMANDPROC_END);
	return 1;
}

void ChannelModeProg::printInputRequest(Menu::Item::Request::Request * req) {
	return ;
}




#ifdef SERIALUI_AUTHENTICATOR_ENABLE
	// Access control
size_t ChannelModeProg::printAccessGrantRequest(Auth::Authenticator * auth) {
	size_t retsize =
			printHelpListStart(Globals::menuStructure()->topLevelMenu());

	PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_AUTHENTICATION_START);
	PROGCHANNEL_OUTFIELD(SerialUI::Auth::Request::Type::Authenticate);
	PROGCHANNEL_OUTFIELD(auth->encoding());
	Auth::Challenge challenge = auth->challenge(Auth::Level::User);
	if (challenge) {
		retsize += _serPort->print(challenge);
	} else {
		retsize += _serPort->print(SUI_STR("password"));
	}

	PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_AUTHENTICATION_END);
	retsize += printHelpListEnd(Globals::menuStructure()->topLevelMenu());

	return retsize;
}

size_t ChannelModeProg::printAccessConfigureRequest(Auth::Authenticator * auth) {
	size_t retsize =
			printHelpListStart(Globals::menuStructure()->topLevelMenu());
	PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_AUTHENTICATION_START);
	PROGCHANNEL_OUTFIELD(SerialUI::Auth::Request::Type::Setup);
	PROGCHANNEL_OUTFIELD(SERIAL_UI_PROGCHANFIELD_AUTHENTICATION_END);
	retsize += printHelpListEnd(Globals::menuStructure()->topLevelMenu());
	return retsize + 3;
}
#endif








#endif /* SERIALUI_PLATFORM_ARDUINOSTANDARD */


} /* namespace Comm */
} /* namespace SerialUI */
