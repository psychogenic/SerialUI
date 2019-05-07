/*
 * ChannelManager.h
 *
 *  Created on: May 27, 2018
 *      Author: Pat Deegan
 *
 *  ChannelManager is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_COMM_CHANNELMANAGER_H_
#define SERIALUI_SRC_INCLUDES_COMM_CHANNELMANAGER_H_
#include "../SerialUIPlatform.h"
#include "../SerialUITypes.h"
#include "CommChannel.h"
namespace SerialUI {
namespace Comm {

class ChannelManager {
public:
	static Channel * newChannelFor(Mode::Selection mode, SourceType * underlyingStream=NULL);
	static void releaseChannel(Channel * ch=NULL);

	inline static Channel * currentChannel() { return current_channel;}
private:
	static Channel * current_channel;
	ChannelManager();
};

} /* namespace Comm */
} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_COMM_CHANNELMANAGER_H_ */
