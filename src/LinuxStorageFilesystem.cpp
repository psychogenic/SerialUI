/*
 * LinuxStorageFilesystem.cpp
 *
 *  Created on: May 3, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#include "includes/SerialUIPlatform.h"

#ifdef SERIALUI_PLATFORM_LINUX

#include "includes/platform/linux/LinuxStorageFilesystem.h"

namespace SerialUI {
namespace Settings {

StorageFilesystem::StorageFilesystem(std::string fspath) :
		filepath(fspath) {

}

bool StorageFilesystem::retrieve(SerializedState into, uint16_t maxlen) {
	std::ifstream fileStream(filepath);
	if (! fileStream) {
		return false;
	}

	std::string content( (std::istreambuf_iterator<char>(fileStream) ),
	                       (std::istreambuf_iterator<char>()    ) );

	if (content.size() < maxlen) {
		maxlen = content.size();
	}
	strncpy(into, content.c_str(), maxlen);
	return true;

}
bool StorageFilesystem::save(SerializedState st) {
	std::ofstream outStream(filepath);
	if (! outStream ) {
		return false;
	}
	outStream << st;
	outStream.close();
	return true;

}

} /* namespace Settings */
} /* namespace SerialUI */

#endif /* SERIALUI_PLATFORM_LINUX */
