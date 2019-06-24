#
#  GattLib++ - GATT Library C++ Wrapper
#
#  Copyright (C) 2017  Pat Deegan, https://psychogenic.com
#
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#

cmake_minimum_required(VERSION 2.6)

find_package(PkgConfig REQUIRED)



include_directories(. ${CMAKE_CURRENT_BINARY_DIR})

set(serialui_SRCS 
	Authenticator.cpp
	AuthSimple.cpp
	AuthStorage.cpp
	AuthStoragePython.cpp
	AuthStorageStatic.cpp
	AuthValidator.cpp
	AuthValidatorEquality.cpp
	AuthValidatorPython.cpp
	BLESerial.cpp
	ChannelManager.cpp
	CommChannelArdStd.cpp
	CommChannel.cpp
	CommChannelSerial.cpp
	CommRequest.cpp
	CommSink.cpp
	CommSource.cpp
	DeSerializer.cpp
	DeSerializerJson.cpp
	ExternalModule.cpp
	ExternalPython.cpp
	ItemCommand.cpp
	ItemDynamicText.cpp
	ItemList.cpp
	ItemReqBoolean.cpp
	ItemReqBoundedLong.cpp
	ItemReqCharacter.cpp
	ItemReqDateTime.cpp
	ItemReqEvent.cpp
	ItemReqFloat.cpp
	ItemReqLong.cpp
	ItemReqOptions.cpp
	ItemReqString.cpp
	ItemReqTime.cpp
	ItemRequest.cpp
	ItemReqUnsignedLong.cpp
	ItemText.cpp
	ItemTrackedView.cpp
	LinuxBLESerial.cpp
	LinuxMain.cpp
	LinuxPrint.cpp
	LinuxStorageFilesystem.cpp
	LinuxStream.cpp
	Menu.cpp
	MenuItem.cpp
	MenuStructure.cpp
	NRF52BLESerial.cpp
	Serializer.cpp
	SerializerJson.cpp
	SerialUI.cpp
	SubGroup.cpp
	SubMenu.cpp
	SUIGlobals.cpp
	SUILinuxSupport.cpp
	SUIPlatArduino.cpp
	SUIPlatNRF52Arduino.cpp
	SUIState.cpp
	SUIWString.cpp
	TrackedString.cpp
	TrackedVariable.cpp
	Tracking.cpp )

#set(serialui_LIBS ${GOBJECT_LDFLAGS} ${GIO_LDFLAGS} ${GLIB_LDFLAGS} ${PYTHON_LDFLAGS})
set(serialui_LIBS )

add_library(serialui SHARED ${serialui_SRCS})
target_link_libraries(serialui ${serialui_LIBS})

if (SERIALUI_DEV)
# forget it
else()
install(TARGETS serialui LIBRARY DESTINATION lib)
endif()
