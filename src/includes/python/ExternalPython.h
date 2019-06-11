/*
 * ExternalPython.h
 *
 *  Created on: May 11, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_PYTHON_EXTERNALPYTHON_H_
#define SERIALUI_SRC_INCLUDES_PYTHON_EXTERNALPYTHON_H_


#ifdef SERIALUI_PYTHONMODULES_SUPPORT_ENABLE

#include <map>
#include <set>

typedef std::set<PyObject*>	ItemPyObjectSet;

typedef std::map<uint8_t, ItemPyObjectSet> ItemPyObjectById;

#include "../auth/AuthValidator.h"
#ifdef SERIALUI_AUTHENTICATOR_ENABLE
/*
typedef struct {
	PyObject * validator;
	SerialUI::Auth::Validator * suivalidator;
	SerialUI::Auth::Storage * suxxivalidator;

} ho ;
*/
#endif


namespace SerialUI {
namespace Python {

class ObjectsStore {

public:
	ObjectsStore() : auth_storage(NULL), auth_validator(NULL){}

	void addInput(uint8_t forId, PyObject * input) {
		add(forId, input, inputs);
	}

	void addCommand(uint8_t forId, PyObject * cmd) {
		add(forId, cmd, commands);
	}

	ItemPyObjectSet inputsFor(uint8_t forId) {
		return setFor(forId, inputs);
	}
	ItemPyObjectSet commandsFor(uint8_t forId) {
		return setFor(forId, commands);
	}

	void callTriggeredOnCommands(uint8_t forId);

	void callTriggeredOnInputs(uint8_t forId);

	bool callValidatorOnInputs(uint8_t forId, TopLevelString & val);
	bool callValidatorOnInputs(uint8_t forId, float val);
	bool callValidatorOnInputs(uint8_t forId, unsigned long val);
	bool callValidatorOnInputs(uint8_t forId, long val);
	bool callValidatorOnInputs(uint8_t forId, bool val);


	void deleteEntryFor(uint8_t forId, PyObject * obj);


	/*
	void addAuthStorage(PyObject * authstore) {
		auth_storage.insert(authstore);
	}
	*/

	/*
	template<typename... Args>
	PyObject* callMethodOnValidator(const char* methName, const char* format, Args... args)
	{
		if (! auth_validator) {
			Py_RETURN_NONE;
		}
		return PyObject_CallMethod(auth_validator, methName, format, args...);
	}*/


	template<typename... Args>
	PyObject* callMethodOnStorage(const char* methName, const char* format, Args... args)
	{
		SERIALUI_DEBUG_OUT(auth_storage);
		SERIALUI_DEBUG_OUT(", ");
		SERIALUI_DEBUG_OUTLN(auth_validator);


		SERIALUI_DEBUG_OUT("calling on storage: '");
		SERIALUI_DEBUG_OUT(methName);
		if (! auth_storage) {
			SERIALUI_DEBUG_OUTLN("': no storage set!");
			Py_RETURN_NONE;
		}
		PyObject* hb = PyObject_GetAttrString(auth_storage, methName);
		if (! (hb && PyCallable_Check(hb))) {

			SERIALUI_DEBUG_OUTLN("' NO SUCH CALLABLE");
			Py_RETURN_NONE;

		}
		SERIALUI_DEBUG_OUTLN("', triggering");
		PyObject * ret = PyObject_CallMethod(auth_storage, methName, format, args...);
		if (ret) {
			SERIALUI_DEBUG_OUT("Got RESP: ");
			SERIALUI_DEBUG_OUTLN(ret);
		} else {
			SERIALUI_DEBUG_OUTLN("No RESP!  Problem calling meth on authstorage");
			if (PyErr_Occurred()) {
				PyErr_Print();
			}
		}
		return ret;
	}

	template<typename... Args>		// var args
	PyObject* callMethodOnAuthValidator(const char* methName, const char* format, Args... args)
	{
		SERIALUI_DEBUG_OUT(auth_storage);
		SERIALUI_DEBUG_OUT(", ");
		SERIALUI_DEBUG_OUTLN(auth_validator);


		SERIALUI_DEBUG_OUT("calling on validator: '");
		SERIALUI_DEBUG_OUT(methName);
		if (! auth_validator) {
			SERIALUI_DEBUG_OUTLN("', no validator set!");
			Py_RETURN_NONE;
		}

		PyObject* hb = PyObject_GetAttrString(auth_validator, methName);
		if (! (hb && PyCallable_Check(hb))) {

			SERIALUI_DEBUG_OUTLN("' NO SUCH CALLABLE");
			Py_RETURN_NONE;

		}

		SERIALUI_DEBUG_OUTLN("', triggering");
		PyObject * ret = PyObject_CallMethod(auth_validator, methName, format, args...);

		if (ret) {
			SERIALUI_DEBUG_OUT("Got RESP: ");
			SERIALUI_DEBUG_OUTLN(ret);
		} else {
			SERIALUI_DEBUG_OUTLN("No RESP!  Problem calling meth on authvalidator");
			if (PyErr_Occurred()) {
				PyErr_Print();
			}
		}
		return ret;
	}

	/*
	ItemPyObjectSet & authStorageSet() {
		return auth_storage;
	}
	*/
	// void removeAuthObject(PyObject * obj);

	void setAuthStorage(PyObject *pobj) {
		SERIALUI_DEBUG_OUT("OStore.setAuthStorage():" );
		SERIALUI_DEBUG_OUTLN(pobj);
	auth_storage = pobj;}
	PyObject * authStorage() { return auth_storage;}


	void setAuthValidator(PyObject *pobj) {
		SERIALUI_DEBUG_OUT("OStore.setAuthValidator():");
		SERIALUI_DEBUG_OUTLN(pobj);
		auth_validator = pobj;}
	PyObject * authValidator() { return auth_validator;}

private:

	void add(uint8_t forId, PyObject * obj, ItemPyObjectById & toMap);
	ItemPyObjectSet setFor(uint8_t forId, ItemPyObjectById & inMap);

	ItemPyObjectById inputs;
	ItemPyObjectById commands;
	PyObject * auth_storage;
	PyObject * auth_validator;
};

extern ObjectsStore SUIPyObjectsStore;

}
}

#endif /* SERIALUI_PYTHONMODULES_SUPPORT_ENABLE */

#endif /* SERIALUI_SRC_INCLUDES_PYTHON_EXTERNALPYTHON_H_ */
