/*
 * externalPythonValidation.h
 *
 *  Created on: Jun 10, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_EXTERNALPYTHONVALIDATION_H_
#define SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_EXTERNALPYTHONVALIDATION_H_
#include "../../SerialUIPlatform.h"

#ifdef SERIALUI_PYTHONMODULES_SUPPORT_ENABLE

#define ITEMPYTHONOVERRIDE_VALIDATION_DECL(thistype) \
	/* override to support python validation */ \
	virtual bool externalValidation(thistype & v)

#define ITEMPYTHONOVERRIDE_VALIDATION(parentclass, thisclass, thistype) \
bool thisclass::externalValidation(thistype & v) { \
	if (! this->parentclass::externalValidation(v)) {\
		return false;\
	}\
	Python::ExternalModule * pymod = Globals::pythonModule();\
	if (pymod && !pymod->isValidTrigger(this, v)) {\
		return false;\
	}\
	return true;\
}

#else


#define ITEMPYTHONOVERRIDE_VALIDATION_DECL(thistype)

#define ITEMPYTHONOVERRIDE_VALIDATION(parentclass, thisclass, thistype)

#endif /* SERIALUI_PYTHONMODULES_SUPPORT_ENABLE */



#endif /* SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_EXTERNALPYTHONVALIDATION_H_ */
