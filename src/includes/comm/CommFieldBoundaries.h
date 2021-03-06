/*
 * CommFieldBoundaries.h
 *
 *  Created on: Sep 23, 2018
 *      Author: Pat Deegan
 *
 *  CommFieldBoundaries is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_COMM_COMMFIELDBOUNDARIES_H_
#define SERIALUI_SRC_INCLUDES_COMM_COMMFIELDBOUNDARIES_H_

#define SERIAL_UI_PROGCHANFIELD_END					0xfe
#define SERIAL_UI_PROGCHANFIELD_SEP					0xf5


#define SERIAL_UI_PROGCHANFIELD_AUTHENTICATION_START	0xac
#define SERIAL_UI_PROGCHANFIELD_AUTHENTICATION_SEP		SERIAL_UI_PROGCHANFIELD_SEP
#define SERIAL_UI_PROGCHANFIELD_AUTHENTICATION_END		SERIAL_UI_PROGCHANFIELD_END



#define SERIAL_UI_PROGCHANFIELD_COMMANDPROC_START	0xc0

#define SERIAL_UI_PROGCHANFIELD_COMMANDPROC_END		SERIAL_UI_PROGCHANFIELD_END


#define SERIAL_UI_PROGCHANFIELD_MENULISTING_START	0xf0
#define SERIAL_UI_PROGCHANFIELD_MENULISTING_END		0xff


#define SERIAL_UI_PROGCHANFIELD_HELP_START			0xfb
#define SERIAL_UI_PROGCHANFIELD_HELP_SEP			0xab
#define SERIAL_UI_PROGCHANFIELD_HELP_END			SERIAL_UI_PROGCHANFIELD_END

#define SERIAL_UI_PROGCHANFIELD_ERROR_START			0xe0
#define SERIAL_UI_PROGCHANFIELD_ERROR_END			SERIAL_UI_PROGCHANFIELD_END


#define SERIAL_UI_PROGCHANFIELD_ITEM_START			0xf1
#define SERIAL_UI_PROGCHANFIELD_ITEM_SEP			SERIAL_UI_PROGCHANFIELD_SEP
#define SERIAL_UI_PROGCHANFIELD_ITEM_END			SERIAL_UI_PROGCHANFIELD_END

#define SERIAL_UI_PROGCHANFIELD_STATE_START			0xf2
#define SERIAL_UI_PROGCHANFIELD_STATE_SEP			SERIAL_UI_PROGCHANFIELD_SEP
#define SERIAL_UI_PROGCHANFIELD_STATE_END			SERIAL_UI_PROGCHANFIELD_END


#define SERIAL_UI_PROGCHANFIELD_REQVARUPDATE_START	0xf3
#define SERIAL_UI_PROGCHANFIELD_REQVARUPDATE_SEP	SERIAL_UI_PROGCHANFIELD_SEP
#define SERIAL_UI_PROGCHANFIELD_REQVARUPDATE_END	SERIAL_UI_PROGCHANFIELD_END

#define SERIAL_UI_PROGCHANREQUEST_HEADER_COMMAND	0xc0
#define SERIAL_UI_PROGCHANREQUEST_HEADER_REQINPUT	0xc1
#define SERIAL_UI_PROGCHANREQUEST_HEADER_SYSCOMMAND	0xc2







#endif /* SERIALUI_SRC_INCLUDES_COMM_COMMFIELDBOUNDARIES_H_ */
