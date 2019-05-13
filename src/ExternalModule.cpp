#include "includes/python/ExternalModule.h"


#ifdef SERIALUI_PYTHONMODULES_SUPPORT_ENABLE

#include "includes/python/ExternalPython.h"
#include "includes/SerialUI.h"
#include "includes/SUIGlobals.h"

#define SUIDRIVER() ((SerialUI*)driver)

namespace SerialUI {
namespace Python {

ExternalModule::ExternalModule(void * serialUIDriver,
		  DynamicString name, DynamicString path) :
		module_name(name),
		module_path(path),
		is_loaded(false),
		load_attempted(false),
		hInstance(NULL),
		driver(serialUIDriver)
		// ,
		//driver(sui_driver)
{

}
ExternalModule::~ExternalModule() {
	if (! is_loaded) {
		return;
	}
	pHandler.Release();
	pModule.Release();
	delete hInstance;

}

bool ExternalModule::load() {
	if (is_loaded) {
		return true;
	}
	if (load_attempted) {
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
		return false;
	}
	SERIALUI_DEBUG_OUTLN("loaded module!");

	// dict is a borrowed reference.
	CPyObject dict = PyModule_GetDict(pModule);
	if (!dict) {
		PyErr_Print();
		SERIALUI_DEBUG_OUTLN("Couldn't get dict");
		return false;
	}
	CPyObject pHandlerName = PyDict_GetItemString(dict, "SerialUIHandler");
	if (!pHandlerName) {
		PyErr_Print();
		SERIALUI_DEBUG_OUTLN("Couldn't get SerialUIHandler name");
		return 1;
	}
	if (PyCallable_Check(pHandlerName)) {
		pHandler = PyObject_CallObject(pHandlerName, nullptr);
		PyDict_SetItem(dict, Py_BuildValue("s", "HANDLER"), pHandler);

	} else {
		SERIALUI_DEBUG_OUTLN("Couldn't instantiate SerialUIHandler");
		return 1;
	}

	PyObject* hb = PyObject_GetAttrString(pHandler, "heartbeat");
	if (hb && PyCallable_Check(hb)) {
		pHeartbeat = hb;
		SUIDRIVER()->setUserPresenceHeartbeat([]() {
			Globals::pythonModule()->triggerHeartbeat();
		});

	}

	is_loaded = true;
	return true;

}

void ExternalModule::updated(Menu::Item::Request::Request * req) {
	SUIDRIVER()->updatedLocally(*req);

}
void ExternalModule::updated(Tracked::State* st) {
	SUIDRIVER()->showTrackedState();

}



bool ExternalModule::setHearbeatPeriod(unsigned long ms) {
	SUIDRIVER()->setUserPresenceHeartbeatPeriod(ms);
	return true;
}
bool ExternalModule::triggerHeartbeat() {
	if (! pHeartbeat) {
		return false;
	}

	PyObject_CallObject(pHeartbeat, nullptr);
	return true;

}

bool ExternalModule::callHandlerMethod(DynamicString name) {

	CPyObject hm = PyObject_GetAttrString(pHandler, name);
	if (! (hm && PyCallable_Check(hm))) {
		return false;
	}
	CPyObject yo = PyObject_CallObject(hm, nullptr);
	return true;
}




bool ExternalModule::trigger(Menu::Item::Request::Request * req) {
	if (!load()) {
		return false;
	}
	SERIALUI_DEBUG_OUT("Ext mod trigger for:");
	SERIALUI_DEBUG_OUTLN(req->key());

	SUIPyObjectsStore.callTriggeredOnInputs(req->id());
	return true;

/*

	CPyObject pFunc = PyObject_GetAttrString(pHandler, req->key());
	if (pFunc && PyCallable_Check(pFunc)) {
		SERIALUI_DEBUG_OUTLN("Making call to obj");
		CPyObject args = Py_BuildValue("(ss)", req->key(), req->help());

		CPyObject pValue = PyObject_CallObject(pFunc, args);
		return true;
	} else {
		SERIALUI_DEBUG_OUTLN("Could not find method");
		return false;
	}
	*/
}

bool ExternalModule::trigger(Menu::Item::Command * cmd) {
	if (!load()) {
		return false;
	}
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
}
}


#endif /* SERIALUI_PYTHONMODULES_SUPPORT_ENABLE */
