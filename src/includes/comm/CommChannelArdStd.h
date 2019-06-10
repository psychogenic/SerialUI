/*
 * CommChannelArdStd.h
 *
 *  Created on: May 26, 2018
 *      Author: Pat Deegan
 *
 *  CommChannelArdStd is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_COMM_COMMCHANNELARDSTD_H_
#define SERIALUI_SRC_INCLUDES_COMM_COMMCHANNELARDSTD_H_

#include "../SerialUIPlatform.h"
/*
#include "CommSink.h"
#include "CommSource.h"
#include "CommChannel.h"
*/
#include "CommChannelSerial.h"

namespace SerialUI {
namespace Comm {

#ifdef SERIALUI_CHANNELS_USE_STANDARDSTREAMS
class ChannelModeUser : public SerialChannel {
public:
	ChannelModeUser(Mode::Selection forMode, SourceType * serPort=&(SUI_PLATFORM_SOURCE_DEFAULT));



	virtual size_t printHelpListStart(Menu::Item::SubMenu * mnu) { return 0;}
	virtual size_t printHelpListEnd(Menu::Item::SubMenu * mnu) { return 0;}
	virtual bool getNextRequest(Menu::Item::ID inMenu, Request * into);

	virtual void showPrompt();
	virtual void printInputRequest(Menu::Item::Request::Request * req);

#ifdef SERIALUI_AUTHENTICATOR_ENABLE
	// Access control
	virtual size_t printAccessGrantRequest(Auth::Authenticator * auth);
	virtual size_t printAccessConfigureRequest(Auth::Authenticator * auth);
#endif

protected:


    virtual size_t printFieldStart(Tracked::State *state) { return 0;}
    virtual size_t printFieldStart(Menu::Item::Item * itm){ return 0;}

    virtual size_t printFieldSeparator(Menu::Item::Type::Value tp);
    virtual size_t printFieldEnd(Menu::Item::Type::Value tp);


    virtual size_t printUpdateFieldStart(Menu::Item::Item * itm) { return 0;}
    virtual size_t printHelpFieldStart(Menu::Item::Item * itm);

	virtual size_t printHelpFieldData(Menu::Item::Item * itm) { return 0;}

    virtual size_t printHelpFieldEnd(Menu::Item::Item * itm);
    virtual size_t printHelpFieldSeparator(Menu::Item::Item * itm, size_t curLen=0);

    virtual void printErrorStart();
    virtual void printErrorEnd();


	size_t printSpaces(uint8_t lenTaken, uint8_t lenRequired=35);
    // bool getBuiltinRequest(char * userString, Request * into);


};




class ChannelModeProg : public SerialChannel {
public:
	ChannelModeProg(Mode::Selection forMode, SourceType * serPort=&(SUI_PLATFORM_SOURCE_DEFAULT));



	virtual size_t printHelpListStart(Menu::Item::SubMenu * mnu);
	virtual size_t printHelpListEnd(Menu::Item::SubMenu * mnu);
	virtual bool getNextRequest(Menu::Item::ID inMenu, Request * into);

	virtual void showPrompt();
	virtual void printInputRequest(Menu::Item::Request::Request * req);

	virtual size_t printCommandProcessingStart();
	virtual size_t printCommandProcessingDone();


#ifdef SERIALUI_AUTHENTICATOR_ENABLE
	// Access control
	virtual size_t printAccessGrantRequest(Auth::Authenticator * auth);
	virtual size_t printAccessConfigureRequest(Auth::Authenticator * auth);
#endif
protected:

	bool parseBinCommandReq(Request * into);

    virtual size_t printFieldStart(Tracked::State *state);
    virtual size_t printFieldStart(Menu::Item::Item * itm);
    virtual size_t printFieldSeparator(Menu::Item::Type::Value tp);
    virtual size_t printFieldEnd(Menu::Item::Type::Value tp);


    virtual size_t printUpdateFieldStart(Menu::Item::Item * itm);
    virtual size_t printHelpFieldStart(Menu::Item::Item * itm);
    virtual size_t printHelpFieldEnd(Menu::Item::Item * itm);
    virtual size_t printHelpFieldSeparator(Menu::Item::Item * itm, size_t curLen=0);
	virtual size_t printHelpFieldData(Menu::Item::Item * itm);

	size_t printHelpFieldDataForRequest(Menu::Item::Item * itm);
	size_t printHelpFieldDataForTrackedView(Menu::Item::Item * itm);

    virtual void printErrorStart();
    virtual void printErrorEnd();

    void outputRequestContextId();


    // bool getBuiltinRequest(char * userString, Request * into);
};








#endif

} /* namespace Comm */
} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_COMM_COMMCHANNELARDSTD_H_ */
