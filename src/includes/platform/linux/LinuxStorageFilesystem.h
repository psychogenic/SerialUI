/*
 * LinuxStorageFilesystem.h
 *
 *  Created on: May 3, 2019
 *      Author: malcalypse
 */

#ifndef SERIALUI_SRC_INCLUDES_PLATFORM_LINUX_LINUXSTORAGEFILESYSTEM_H_
#define SERIALUI_SRC_INCLUDES_PLATFORM_LINUX_LINUXSTORAGEFILESYSTEM_H_

#include "../../settings/StateStorage.h"
#include "LinuxExtIncludes.h"

namespace SerialUI {
namespace Settings {

class StorageFilesystem : public Storage {
public:
	StorageFilesystem(std::string filepath);
	virtual bool retrieve(SerializedState into, uint16_t maxlen);
	virtual bool save(SerializedState st);

private:
	std::string filepath;
};

} /* namespace Settings */
} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_PLATFORM_LINUX_LINUXSTORAGEFILESYSTEM_H_ */
