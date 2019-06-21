/*
 * State.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  State is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_STATE_H_
#define SERIALUIV3_SRC_INCLUDES_STATE_H_

#include "SerialUITypes.h"
#include "menuitem/ItemCommand.h"
#include "menuitem/ItemRequest.h"
#include "menuitem/SubMenu.h"
#include "comm/CommRequest.h"

namespace SerialUI {


typedef union GeneralStateFlagsU {
	struct {
		bool user_present :1;
		bool user_check_performed :1;
		bool response_transmitted :1;
		bool echo_commands :1;
		bool menu_manual_override :1;
		bool user_just_arrived: 1;
		bool always_heartbeat: 1;
		bool should_exit :1;
	};
	uint8_t flags;
	GeneralStateFlagsU(uint8_t v) :
			flags(v) {
	}


} GeneralStateFlags;

class State {
public:
	typedef enum {
		INVALID = 0x00,
		Idle = 0x01,
		Command = 0x02,
		InputRequest = 0x03,

	} Activity;
	State();

	inline void setIdle() { _currentActivity = Idle;}

	void processingCommand(Menu::Item::Command * cmdItem);
	void gettingInput(Menu::Item::Request::Request * inputItem);
	void enterMenu(Menu::Item::SubMenu * sub);

	inline Activity current() { return _currentActivity;}
	inline Menu::Item::SubMenu * currentMenu() { return _currentMenu;}

	inline bool haveCurrentRequestContext() { return _currentRequestContext != NULL;}
	inline void setCurrentRequestContext(Comm::Request * rc) { _currentRequestContext = rc;}
	inline Comm::Request * currentRequestContext() { return _currentRequestContext; }
	inline void clearCurrentRequestContext() { _currentRequestContext = NULL; }

	Menu::Item::Item * currentItem() ;


	inline void setMode(Mode::Selection setTo) { _mode = setTo;}
	inline Mode::Selection mode() { return _mode;}
	void setGreeting(StaticString greets);
	StaticString greeting() { return _greetingMsg;}
	void setUID(StaticString u);
	StaticString uid() { return _uid;}

	GeneralStateFlags & flags() { return _stateflags;}
	bool userPresent() { return _stateflags.user_present;}
	void setShouldExit(bool setTo=true) { _stateflags.should_exit = setTo;}
private:
	Activity _currentActivity;
	Menu::Item::ID _currentMenuId;
	Menu::Item::ID _currentItemId;
	Menu::Item::SubMenu * _currentMenu;
	Comm::Request * _currentRequestContext;
	Mode::Selection _mode;
	StaticString _greetingMsg;
	StaticString _uid;
	GeneralStateFlags _stateflags;




};

} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_STATE_H_ */
