/*
 * State.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  State is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/SUIState.h"
#include "includes/SUIGlobals.h"
namespace SerialUI {

State::State() :
		_currentActivity(Idle), _currentMenuId(0), _currentItemId(0), _currentMenu(NULL),
		_mode(Mode::User),
		_greetingMsg(NULL),
		_uid(NULL),
		_stateflags(0)
{
	enterMenu(Globals::menuStructure()->topLevelMenu());

}

void State::processingCommand(Menu::Item::Command * cmdItem) {
	_currentActivity = Command;
	_currentItemId = cmdItem->id();
}
void State::gettingInput(Menu::Item::Request::Request * inputItem) {
	_currentActivity = InputRequest;
	_currentItemId = inputItem->id();
}

void State::enterMenu(Menu::Item::SubMenu * sub) {
	_currentMenuId = sub->id();
	_currentMenu = sub;
}

Menu::Item::Item * State::currentItem()  {
	return Globals::menuStructure()->getItemById(_currentItemId);
}


void State::setGreeting(StaticString greets)
{
	_greetingMsg = greets;
}

void State::setUID(StaticString u) {
	_uid = u;
}




} /* namespace SerialUI */
