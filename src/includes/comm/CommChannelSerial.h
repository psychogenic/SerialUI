/*
 * SerialChannel.h
 *
 *  Created on: Sep 22, 2018
 *      Author: Pat Deegan
 *
 *  SerialChannel is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_COMM_COMMCHANNELSERIAL_H_
#define SERIALUI_SRC_INCLUDES_COMM_SERIALCHANNEL_H_

#include "../SerialUIPlatform.h"
#include "CommSink.h"
#include "CommSource.h"
#include "CommChannel.h"

namespace SerialUI {
namespace Comm {


#ifdef SERIALUI_CHANNELS_USE_STANDARDSTREAMS

class SerialChannel : public Channel {
public:
	SerialChannel(Mode::Selection forMode, SourceType * serPort=&(SUI_PLATFORM_SOURCE_DEFAULT));


	virtual void poll();
	virtual bool getBoolFor(Menu::Item::ID mid, bool * into);
	virtual bool getCharFor(Menu::Item::ID mid, char * into);
	virtual bool getLongIntFor(Menu::Item::ID mid, long int * into);
	virtual bool getLongUIntFor(Menu::Item::ID mid, unsigned long int * into);
	virtual bool getFloatFor(Menu::Item::ID mid, float * into);
	virtual bool getStringFor(Menu::Item::ID mid, uint8_t maxLength, ::String * into);

	// stream stuff
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    void flushAllWhitespaces();
    virtual uint8_t readUntilEOF(char * intoBuf,
    		uint8_t maxLen, bool trim=false);
    uint8_t readUntilAny(uint8_t * intoBuf,
    		uint8_t maxLen, uint8_t * vals,
			uint8_t numvals, bool trim=false);

    virtual void flush(void);
    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write; // pull in write(str) and write(buf, size) from Print

    virtual size_t print(Tracked::State * state);
	virtual size_t print(Menu::Item::Request::Request * reqState);
	virtual size_t printHeading(Menu::Item::SubMenu * mnu);
	virtual size_t printHelp(Menu::Item::Item * itm);
	virtual size_t printListing(Menu::Item::Item * itm);
	virtual void printTracked(bool force=false);
	virtual void printError(StaticString errMsg);

	virtual void showPrompt() = 0;
	virtual void printInputRequest(Menu::Item::Request::Request * req) = 0;


	virtual size_t printCommandProcessingStart() { return 0;}
	virtual size_t printCommandProcessingDone() { return 0;}

	virtual bool getBuiltinRequest(char * userString, Request * into);

protected:
    int timedPeek();

    virtual size_t printFieldStart(Tracked::State *state) = 0;
    virtual size_t printFieldStart(Menu::Item::Item * itm) = 0;
    virtual size_t printFieldSeparator(Menu::Item::Type::Value tp) = 0;
    virtual size_t printFieldEnd(Menu::Item::Type::Value tp) = 0;


    virtual size_t printUpdateFieldStart(Menu::Item::Item * itm) = 0;
    virtual size_t printHelpFieldStart(Menu::Item::Item * itm) = 0;
    virtual size_t printHelpFieldEnd(Menu::Item::Item * itm) = 0;
    virtual size_t printHelpFieldSeparator(Menu::Item::Item * itm, size_t curLen=0) = 0;
    virtual size_t printHelpFieldData(Menu::Item::Item * itm) = 0;

    virtual void printErrorStart() = 0;
    virtual void printErrorEnd() = 0;

	virtual bool getNextRequest(Menu::Item::ID inMenu, Request * into) = 0;

	size_t printHelpItemStandardDetails(Menu::Item::Item * itm);
	size_t printHelpItem(Menu::Item::Command * cmd);
	size_t printHelpItem(Menu::Item::SubMenu * mnu);
	size_t printHelpItem(Menu::Item::TrackedView * view);
	size_t printHelpItem(Menu::Item::Request::Request * req);
	size_t printHelpItem(Menu::Item::Text * req);
	size_t printHelpItem(Menu::Item::DynamicText * req);

	size_t printRequestCurrentValue(Menu::Item::Request::Request * req);

    SourceType * _serPort;
};


#endif /* SERIALUI_PLATFORM_ARDUINOSTANDARD */
} /* namespace Comm */
} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_COMM_COMMCHANNELSERIAL_H_ */
