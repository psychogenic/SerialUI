/*
 * ChannelManager.cpp
 *
 *  Created on: May 27, 2018
 *      Author: Pat Deegan
 *
 *  ChannelManager is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/comm/ChannelManager.h"
#include "includes/comm/channels.h"


namespace SerialUI {
namespace Comm {

Channel * ChannelManager::current_channel = NULL;

ChannelManager::ChannelManager() {

}


Channel * ChannelManager::newChannelFor(Mode::Selection mode, SourceType * underlyingStream) {
	if (mode == Mode::User) {
		if (underlyingStream) {
			current_channel = new ChannelModeUser(mode, underlyingStream);
		} else {
			current_channel= new ChannelModeUser(mode);
		}
	}  else {
		if (underlyingStream) {
					current_channel = new ChannelModeProg(mode, underlyingStream);
		} else {
					current_channel= new ChannelModeProg(mode);
		}
	}
	return current_channel;
}

void ChannelManager::releaseChannel(Channel * chn) {
	if (chn) {
		delete chn;
	} else if (current_channel) {
		delete current_channel;
	}
	current_channel= NULL;
}

} /* namespace Comm */
} /* namespace SerialUI */
