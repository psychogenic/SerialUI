/*
 * DelegateRemote.h
 *
 *  Created on: Jan 10, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_STREAM_DELEGATEREMOTE_H_
#define SERIALUI_SRC_INCLUDES_STREAM_DELEGATEREMOTE_H_



#include "../../SUIConfig.h"
#include "DelegateBuffered.h"

namespace SUI {
namespace Delegate {

template<class BYTETYPE, SUI::Utils::BufferSize BUFSIZE>
class Remote : public Buffered<BYTETYPE, BUFSIZE> {
public:
	Remote() {};
	virtual ~Remote() {}

};



} /* namespace Delegate */
}/* namespace SUI */





#endif /* SERIALUI_SRC_INCLUDES_STREAM_DELEGATEREMOTE_H_ */
