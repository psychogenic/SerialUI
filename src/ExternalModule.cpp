/*
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, psychogenic.com
 *  See LICENSE file for details, or
 *  https://devicedruid.com/
 *  and https://inductive-kickback.com/
 *
 */

#include "includes/python/ExternalModule.h"


#ifdef SERIALUI_PYTHONMODULES_SUPPORT_ENABLE

#include "includes/python/ExternalPython.h"
#include "includes/SerialUI.h"
#include "includes/SUIGlobals.h"

#define SUIDRIVER() ((SerialUI*)driver)

#define EXTMOD_CALLVALIDATOR_FOR(req, val) \
	SERIALUI_DEBUG_OUT(F("ExternalModule::isValidTrigger() ")); \
	if (!load()) {\
		SERIALUI_DEBUG_OUTLN(F("could not load"));\
		return true; /* won't deny */ \
	} \
	SERIALUI_DEBUG_OUT(" for:");\
	SERIALUI_DEBUG_OUTLN(req->key());\
	bool _retVal = SUIPyObjectsStore.callValidatorOnInputs(req->id(), val); \
	if (_retVal) { SERIALUI_DEBUG_OUTLN(F("validator says all well")); } \
	else { SERIALUI_DEBUG_OUTLN(F("validator says NAY")); }\
	return _retVal

namespace SerialUI {
namespace Python {






ExternalModule::ExternalModule(void * serialUIDriver,
		  DynamicString name, DynamicString path) :
#ifdef SERIALUI_AUTHENTICATOR_ENABLE
		auth_validator(NULL),
		auth_storage(NULL),
#endif
		module_name(name),
		module_path(path),
		post_load_call(NULL),
		is_loaded(false),
		load_attempted(false),
		hInstance(NULL),
		driver(serialUIDriver)
{
	SERIALUI_DEBUG_OUTLN(F("EXT MOD CONSTRUCTED"));

}
ExternalModule::~ExternalModule() {
	SERIALUI_DEBUG_OUTLN(F("EXT MOD D'TOR"));
	if (! is_loaded) {
		SERIALUI_DEBUG_OUTLN(F("Never loaded"));
		return;
	}
	SERIALUI_DEBUG_OUTLN(F("Releasing python handles"));
	pHandler.Release();
	pModule.Release();
	if (hInstance) {

		delete hInstance;
	}

}

bool ExternalModule::load() {

	if (is_loaded) {
		return true;
	}

	SERIALUI_DEBUG_OUTLN(F("EXT MOD LOAD"));
	if (load_attempted) {
		SERIALUI_DEBUG_OUTLN(F("already attempted to load (and failed)"));
		return false;
	}
	load_attempted = true;

	if (module_path) {

		setenv("PYTHONPATH", module_path, 1);
	} else {
		setenv("PYTHONPATH", ".", 1);
	}
	if (!hInstance) {
		hInstance = new CPyInstance();
		if (!hInstance) {
			return false;
		}
	}
	SERIALUI_DEBUG_OUT("Python EXT mod loading: ");
	SERIALUI_DEBUG_OUTLN(module_name);


	CPyObject pName = PyUnicode_FromString(module_name);
	pModule = PyImport_Import(pName);
	if (!pModule) {
		SERIALUI_DEBUG_OUTLN("Failed to load module!");

		if (PyErr_Occurred()) {
			PyErr_Print();
		}
		return false;
	}
	SERIALUI_DEBUG_OUTLN("loaded module!");

	// dict is a borrowed reference.
	CPyObject dict = PyModule_GetDict(pModule);
	if (!dict) {
		if (PyErr_Occurred()) {
			PyErr_Print();
		}
		SERIALUI_DEBUG_OUTLN("Couldn't get dict");
		return false;
	}
	CPyObject pHandlerName = PyDict_GetItemString(dict, SUI_EXTMODULE_NAME_HANDLERCLASS);
	if (!pHandlerName) {
		if (PyErr_Occurred()) {
			PyErr_Print();
		}
		SERIALUI_DEBUG_OUTLN("Couldn't get SerialUI handler");
		return 1;
	}
	if (PyCallable_Check(pHandlerName)) {
		pHandler = PyObject_CallObject(pHandlerName, nullptr);
		if (! pHandler) {
			if (PyErr_Occurred()) {
				PyErr_Print();
			}

			return false;
		}
		PyDict_SetItem(dict, Py_BuildValue("s", "HANDLER"), pHandler);

	} else {
		SERIALUI_DEBUG_OUTLN("Couldn't instantiate SerialUI handler");
		return false;
	}

	is_loaded = true;
	if (PyObject_HasAttrString(pHandler, SUI_EXTMODULE_NAME_HEARTBEAT_METHOD))
	{

		PyObject* hb = PyObject_GetAttrString(pHandler, SUI_EXTMODULE_NAME_HEARTBEAT_METHOD);
		if (hb && PyCallable_Check(hb)) {
			pHeartbeat = hb; // keep a reference to this
			SUIDRIVER()->setUserPresenceHeartbeat([]() {
				Globals::pythonModule()->triggerHeartbeat();
			});

		} else {
			PyErr_Clear(); // not implemented, doesn't matter
		}

	}

	if (PyObject_HasAttrString(pHandler, SUI_EXTMODULE_NAME_LOADCOMPLETE_METHOD))
	{
		CPyObject loadedCallback = PyObject_GetAttrString(pHandler, SUI_EXTMODULE_NAME_LOADCOMPLETE_METHOD);
		if (loadedCallback && PyCallable_Check(loadedCallback)) {
			CPyObject loadedRet = PyObject_CallObject(loadedCallback, nullptr);

		} else {
			PyErr_Clear(); // not implemented, doesn't matter
		}
	}

	return true;

}

void ExternalModule::updated(Menu::Item::Request::Request * req) {
	SERIALUI_DEBUG_OUTLN(F("ExternalModule::updated request"));
	SUIDRIVER()->updatedLocally(*req);

}
void ExternalModule::updated(Tracked::State* st) {
	SERIALUI_DEBUG_OUTLN(F("ExternalModule::updated tracked state"));
	SUIDRIVER()->showTrackedState();

}



bool ExternalModule::setHearbeatPeriod(unsigned long ms) {
	SERIALUI_DEBUG_OUTLN(F("ExternalModule::setHearbeatPeriod"));
	SUIDRIVER()->setUserPresenceHeartbeatPeriod(ms);
	return true;
}
bool ExternalModule::triggerHeartbeat() {

	if (! pHeartbeat) {
		return false;
	}
	//SERIALUI_DEBUG_OUTLN(F("ExternalModule::trigger heartbeat"));

	CPyObject retObj = PyObject_CallObject(pHeartbeat, nullptr);
	return true;

}


void ExternalModule::userEntered() {

	if (!pHandler) {
		return;
	}
	if (!PyObject_HasAttrString(pHandler,
			SUI_EXTMODULE_NAME_USERENTERED_METHOD)) {
		return;
	}
	CPyObject uEntered = PyObject_GetAttrString(pHandler,
			SUI_EXTMODULE_NAME_USERENTERED_METHOD);
	if (uEntered && PyCallable_Check(uEntered)) {

		CPyObject retObj = PyObject_CallObject(uEntered, nullptr);

	} else {
		PyErr_Clear(); // not implemented, doesn't matter
	}
}
void ExternalModule::userExit() {

	if (!pHandler) {
		return;
	}
	if (!PyObject_HasAttrString(pHandler,
			SUI_EXTMODULE_NAME_USEREXIT_METHOD)) {
		return;
	}
	CPyObject uExit = PyObject_GetAttrString(pHandler,
			SUI_EXTMODULE_NAME_USEREXIT_METHOD);

	if (uExit && PyCallable_Check(uExit)) {

		CPyObject retObj = PyObject_CallObject(uExit, nullptr);

	} else {
		PyErr_Clear(); // not implemented, doesn't matter
	}

}

bool ExternalModule::callHandlerMethod(DynamicString name) {

	SERIALUI_DEBUG_OUT(F("ExternalModule::callHandler method "));
	SERIALUI_DEBUG_OUTLN(name);
	if (!PyObject_HasAttrString(pHandler, name)) {
		SERIALUI_DEBUG_OUTLN("No such method found");
		return false;
	}
	CPyObject hm = PyObject_GetAttrString(pHandler, name);
	if (! (hm && PyCallable_Check(hm))) {
		return false;
	}
	CPyObject yo = PyObject_CallObject(hm, nullptr);
	return true;
}




bool ExternalModule::trigger(Menu::Item::Request::Request * req) {
	SERIALUI_DEBUG_OUT(F("ExternalModule::trigger() "));
	if (!load()) {
		SERIALUI_DEBUG_OUTLN(F("could not load"));
		return false;
	}
	SERIALUI_DEBUG_OUT(" for:");
	SERIALUI_DEBUG_OUTLN(req->key());

	SUIPyObjectsStore.callTriggeredOnInputs(req->id());
	return true;
}



bool ExternalModule::trigger(Menu::Item::Command * cmd) {
	SERIALUI_DEBUG_OUT(F("ExternalModule::trigger() cmd "));
	if (!load()) {
		SERIALUI_DEBUG_OUTLN(F("could not load"));
		return false;
	}
	SERIALUI_DEBUG_OUT(" for cmd ");
	SERIALUI_DEBUG_OUTLN((int)cmd->id());
	SUIPyObjectsStore.callTriggeredOnCommands(cmd->id());
	return true;
	/*
	CPyObject pFunc = PyObject_GetAttrString(pHandler, cmd->key());

	if (pFunc && PyCallable_Check(pFunc)) {
		CPyObject pValue = PyObject_CallObject(pFunc, NULL);
		return true;
	} else {
		return false;
	}
	*/

}



bool ExternalModule::isValidTrigger(Menu::Item::Request::Request * req,
		unsigned long val) {
	EXTMOD_CALLVALIDATOR_FOR(req, val);

}
bool ExternalModule::isValidTrigger(Menu::Item::Request::Request * req,
		long val) {

	EXTMOD_CALLVALIDATOR_FOR(req, val);
}
bool ExternalModule::isValidTrigger(Menu::Item::Request::Request * req,
		float val) {
	EXTMOD_CALLVALIDATOR_FOR(req, val);

}
bool ExternalModule::isValidTrigger(Menu::Item::Request::Request * req,
		bool val) {
	EXTMOD_CALLVALIDATOR_FOR(req, val);
}

bool ExternalModule::isValidTrigger(Menu::Item::Request::Request * req,
		TopLevelString & val) {

	EXTMOD_CALLVALIDATOR_FOR(req, val);
}


#ifdef SERIALUI_AUTHENTICATOR_ENABLE


class ExtModuleAuthStorageBridge : public Auth::Storage {
public:
	ExtModuleAuthStorageBridge() : Auth::Storage() {

		SERIALUI_DEBUG_OUTLN("ExtModuleAuthStorageBridge c'tor");
	}


	virtual bool configured(Auth::Level::Value forLevel=Auth::Level::User) {

		CPyObject retObj = SUIPyObjectsStore.callMethodOnStorage("configured", "(i)", (int)forLevel);

		if ((!retObj) || retObj.getObject() == Py_None) {
			return false;
		}

		if (PyBool_Check(retObj)) {
			if (PyObject_IsTrue(retObj)){
					return true;
				}
		}
		return false;
	}
	virtual Auth::Passphrase passphrase(Auth::Level::Value forLevel) {
		CPyObject retObj = SUIPyObjectsStore.callMethodOnStorage("passphrase", "(i)", (int)forLevel);

		if ((!retObj) || retObj.getObject() == Py_None) {
			return NULL;
		}
		if (PyUnicode_Check(retObj)) {
			char * strtoprint = NULL;
			if (!PyArg_ParseTuple(retObj, "s", &strtoprint)) {
				return NULL;
			}
			return strtoprint;
		}

		return NULL;

	}
	virtual bool setPassphrase(Auth::Passphrase pass, Auth::Level::Value forLevel) {
		CPyObject retObj = SUIPyObjectsStore.callMethodOnStorage(
				"setPassphrase", "(s, i)", pass, (int) forLevel);

		if ((!retObj) || retObj == Py_None) {
			return NULL;
		}
		if (PyBool_Check(retObj)) {
			if (PyObject_IsTrue(retObj)) {
				return true;
			}
		}
		return false;
	}
};

class ExtModuleAuthValidatorBridge : public Auth::Validator {
public:
	ExtModuleAuthValidatorBridge(Auth::Storage * strg) : Auth::Validator(strg) {

		SERIALUI_DEBUG_OUTLN("ExtModuleAuthValidatorBridge c'tor");
	}

	virtual Auth::Transmission::Type::Value communicationType() {
		SERIALUI_DEBUG_OUTLN("ExtModuleAuthValid.communicationType()");
		CPyObject  retObj = SUIPyObjectsStore.callMethodOnAuthValidator("communicationType", "()");
		if ((!retObj) || retObj.getObject() == Py_None) {
			SERIALUI_DEBUG_OUTLN("No ret/ret none");
			return Auth::Transmission::Type::Plain;
		}
		if (! PyNumber_Check(retObj)) {

			SERIALUI_DEBUG_OUTLN("NaN");
			return Auth::Transmission::Type::Plain;
		}
		uint8_t retVal = 0;

		if (PyLong_Check(retObj)) {
			SERIALUI_DEBUG_OUTLN("is long, converting");
			retVal = (uint8_t) PyLong_AsLong(retObj);
		} else if (PyFloat_Check(retObj)) {
			SERIALUI_DEBUG_OUTLN("is float, converting");
			retVal = (uint8_t) PyFloat_AsDouble(retObj);
		}

		switch (retVal) {

		case (uint8_t)Auth::Transmission::Type::MD5:
				return Auth::Transmission::Type::MD5;
		case (uint8_t)Auth::Transmission::Type::SHA256:
				return Auth::Transmission::Type::SHA256;
		default:

			SERIALUI_DEBUG_OUTLN((int)retVal);
			return Auth::Transmission::Type::Plain;
		}

	}
	virtual Auth::Challenge challenge(Auth::Level::Value forLevel=Auth::Level::User) {
		SERIALUI_DEBUG_OUTLN("ExtModuleAuthValid.challenge()");
		CPyObject  retObj = SUIPyObjectsStore.callMethodOnAuthValidator("challenge", "(i)", (int)forLevel);
		if ((!retObj) || retObj.getObject() == Py_None) {
			SERIALUI_DEBUG_OUTLN("No ret/ret none");
			return NULL;
		}

		if (PyUnicode_Check(retObj)) {

			SERIALUI_DEBUG_OUTLN("is unicode, converting");
			return PyUnicode_AsUTF8(retObj);


		}

		return NULL;

	}
	virtual Auth::Level::Value grantAccess(Auth::ChallengeResponse resp) {
		SERIALUI_DEBUG_OUTLN("ExtModuleAuthValid.grantAccess()");
		SERIALUI_DEBUG_OUTLN(resp);

		// first, just check that this value is sane
		CPyObject val = Py_BuildValue("(s)", resp);
		if (! val) {
			SERIALUI_DEBUG_OUTLN("Could not build a value with this 'challenge response'?");
			if (PyErr_Occurred()) {
				PyErr_Print();
			}
			return Auth::Level::NoAccess;
		}

		CPyObject retObj = SUIPyObjectsStore.callMethodOnAuthValidator("grantAccess", "(s)", resp);
		if ((!retObj) || retObj.getObject() == Py_None) {
			SERIALUI_DEBUG_OUTLN("No ret/ret none");
			return Auth::Level::NoAccess;
		}
		if (!PyLong_Check(retObj)) {
			SERIALUI_DEBUG_OUTLN("not a long");
			return Auth::Level::NoAccess;
		}

		switch (PyLong_AsLong(retObj)) {
		case (long) Auth::Level::Guest:
			return Auth::Level::Guest;
		case (long) Auth::Level::User:
			return Auth::Level::User;
		case (long) Auth::Level::Admin:
			return Auth::Level::Admin;
		default:
			return Auth::Level::NoAccess;
		}

	}


};





Auth::Validator * ExternalModule::authValidator() {
	SERIALUI_DEBUG_OUT("ExternalModule::authValidator() ");

	if (!SUIPyObjectsStore.authStorage()) {
		SERIALUI_DEBUG_OUTLN("no store storage");
		return NULL;
	}

	if (auth_validator) {
		SERIALUI_DEBUG_OUTLN("cached");
		// already setup.
		return auth_validator;
	}

	if (! (auth_storage || authStorage()) ) {
		SERIALUI_DEBUG_OUTLN("no storage!");
		return NULL;
	}
	if (!SUIPyObjectsStore.authValidator()) {
		SERIALUI_DEBUG_OUTLN("no store validator!");
		return NULL;
	}


	SERIALUI_DEBUG_OUTLN("constructing");
	auth_validator = new ExtModuleAuthValidatorBridge(authStorage());

	return auth_validator;

}
Auth::Storage * ExternalModule::authStorage()
{

	SERIALUI_DEBUG_OUT("ExternalModule::authStorage() ");

	if (auth_storage) {
		// already setup.
		SERIALUI_DEBUG_OUTLN("cached");
		return auth_storage;
	}


	if (! SUIPyObjectsStore.authStorage()) {
		SERIALUI_DEBUG_OUTLN("not configured");
		return NULL;
	}


	SERIALUI_DEBUG_OUTLN("creating");
	auth_storage = new ExtModuleAuthStorageBridge();

	return auth_storage;

}
#endif /* SERIALUI_AUTHENTICATOR_ENABLE */

}
}


#endif /* SERIALUI_PYTHONMODULES_SUPPORT_ENABLE */
