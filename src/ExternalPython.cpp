/*
 * ExternalPython.cpp
 *
 *  Created on: May 11, 2019
 *      Author: malcalypse
 */


#include "includes/python/ExternalModule.h"

#ifdef SERIALUI_PYTHONMODULES_SUPPORT_ENABLE
#include "includes/python/ExternalPython.h"

#include "includes/SerialUI.h"
#include "includes/SUIGlobals.h"
#include "structmember.h"


#include <thread>

namespace SerialUI {
namespace Python {

void ObjectsStore::callTriggeredOnCommands(uint8_t forId) {

	ItemPyObjectSet commandsToTrip = commandsFor(forId);
	if (!commandsToTrip.size()) {
		return;
	}

	// Py_BEGIN_ALLOW_THREADS
	for (ItemPyObjectSet::iterator it = commandsToTrip.begin();
			it != commandsToTrip.end(); it++) {

		PyObject_CallMethod(*it, "triggered", "()");
	}
	// Py_END_ALLOW_THREADS
}

void ObjectsStore::callTriggeredOnInputs(uint8_t forId) {

	ItemPyObjectSet inputsToNotify = inputsFor(forId);
	if (!inputsToNotify.size()) {
		return;
	}

	for (ItemPyObjectSet::iterator it = inputsToNotify.begin();
			it != inputsToNotify.end(); it++) {

		PyObject_CallMethod(*it, "changed", "()");
	}

}

void ObjectsStore::deleteEntryFor(uint8_t forId, PyObject * obj) {

	if (commands.find(forId) != commands.end()) {
		for (ItemPyObjectSet::iterator it = commands[forId].begin();
				it != commands[forId].end(); it++) {
			if (*it == obj) {
				// Py_DECREF(obj);
			}
		}

		std::cerr << "Store ERASE command " << obj << std::endl;
		commands[forId].erase(obj);

	}
	if (inputs.find(forId) != inputs.end()) {
		for (ItemPyObjectSet::iterator it = inputs[forId].begin();
				it != inputs[forId].end(); it++) {
			if (*it == obj) {
				// Py_DECREF(obj);
			}
		}
		std::cerr << "Store ERASE input " << obj << std::endl;
		// if (! obj->ob_refcnt) {
		inputs[forId].erase(obj);
		// }
	}

}

void ObjectsStore::add(uint8_t forId, PyObject * obj,
		ItemPyObjectById & toMap) {
	if (toMap.find(forId) == toMap.end()) {
		toMap[forId] = ItemPyObjectSet();
	}

	if (toMap[forId].find(obj) == toMap[forId].end()) {
		toMap[forId].insert(obj);
		// Py_INCREF(input);
		std::cerr << "Store ADD " << obj << std::endl;
	} else {
		std::cerr << "Store already knows " << obj << std::endl;
	}
}

ItemPyObjectSet ObjectsStore::setFor(uint8_t forId, ItemPyObjectById & inMap) {

	if (inMap.find(forId) == inMap.end()) {
		return ItemPyObjectSet();
	}

	return inMap[forId];
}



ObjectsStore SUIPyObjectsStore;



}
}




#if 0

class SUIPyObjectsStoreType {

public:
	SUIPyObjectsStoreType() {}



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

	void callTriggeredOnCommands(uint8_t forId) {
		ItemPyObjectSet commandsToTrip = commandsFor(forId);
		if (! commandsToTrip.size()) {
			return;
		}

	    // Py_BEGIN_ALLOW_THREADS
		for (ItemPyObjectSet::iterator it=commandsToTrip.begin();
				it != commandsToTrip.end();
				it++)
		{

			PyObject_CallMethod(*it, "triggered", "()");
		}
	    // Py_END_ALLOW_THREADS
	}

	void callTriggeredOnInputs(uint8_t forId) {
		ItemPyObjectSet inputsToNotify = inputsFor(forId);
		if (! inputsToNotify.size()) {
			return;
		}

		for (ItemPyObjectSet::iterator it=inputsToNotify.begin();
				it != inputsToNotify.end();
				it++)
		{

			PyObject_CallMethod(*it, "changed", "()");
		}
	}

	void deleteEntryFor(uint8_t forId, PyObject * obj) {
		if (commands.find(forId) != commands.end()) {
			for (ItemPyObjectSet::iterator it = commands[forId].begin();
					it != commands[forId].end();
					it++) {
				if (*it == obj) {
					// Py_DECREF(obj);
				}
			}

			std::cerr << "Store ERASE command " << obj << std::endl;
			commands[forId].erase(obj);

		}
		if (inputs.find(forId) != inputs.end()) {
			for (ItemPyObjectSet::iterator it = inputs[forId].begin();
					it != inputs[forId].end();
					it++) {
				if (*it == obj) {
					// Py_DECREF(obj);
				}
			}
			std::cerr << "Store ERASE input " << obj << std::endl;
			// if (! obj->ob_refcnt) {
				inputs[forId].erase(obj);
			// }
		}


	}

private:

	void add(uint8_t forId, PyObject * obj, ItemPyObjectById & toMap) {
		if (toMap.find(forId) == toMap.end()){
			toMap[forId] = ItemPyObjectSet();
		}

		if (toMap[forId].find(obj) == toMap[forId].end()) {
			toMap[forId].insert(obj);
			// Py_INCREF(input);
			std::cerr << "Store ADD " << obj << std::endl;
		} else {
			std::cerr << "Store already knows " << obj << std::endl;
		}
	}

	ItemPyObjectSet setFor(uint8_t forId, ItemPyObjectById & inMap) {
		if (inMap.find(forId) == inMap.end()){
			return ItemPyObjectSet();
		}

		return inMap[forId];
	}

	ItemPyObjectById inputs;
	ItemPyObjectById commands;
};

static SUIPyObjectsStoreType SUIPyObjectsStore;



#endif







// SUIPY_DEFINE_ITEMWRAPPER(BASENAME, STRUCT MEMBER FOR SUI ITM, TYPE for member)

#define SUIPY_WRAPPER_MAXCALLBACKS		2



//
//


#define SUIPY_DEFINE_BOILERPLATEWRAPPER(basename, baseType, typeEnum, itmAccessor, itmType) \
typedef struct { \
	PyObject_HEAD \
	PyObject *key; \
	PyObject *help; \
	uint8_t id; \
	baseType * sui_item; \
	typeEnum type; \
	itmType * itmAccessor; \
	PyObject * callbacks[SUIPY_WRAPPER_MAXCALLBACKS]; \
} basename##Object; \
static PyMemberDef basename##_members[] = { \
		{ "key", T_OBJECT_EX, offsetof(basename##Object, key), 0, "key/name" }, \
		{ "help", T_OBJECT_EX, offsetof(basename##Object, help), 0, "help string" }, \
		{ "id", T_INT, offsetof(basename##Object, id), 0, "item id" }, \
		{ "type", T_INT, offsetof(basename##Object, type), 0, "item type id" }, \
		{ NULL } \
}; \
static PyObject * basename##_new(PyTypeObject *type,  \
	PyObject *args, PyObject *kwds) { \
	basename##Object *self; \
	self = (basename##Object *) type->tp_alloc(type, 0); \
	if (self != NULL) { \
		self->key = PyUnicode_FromString(""); \
		if (self->key == NULL) { \
			Py_DECREF(self); \
			return NULL; \
		} \
		self->help = PyUnicode_FromString(""); \
		if (self->help == NULL) { \
			Py_DECREF(self); \
			return NULL; \
		} \
		for (uint8_t i=0; i<SUIPY_WRAPPER_MAXCALLBACKS; i++) { \
			self->callbacks[i] = NULL; \
		}\
		self->id = 0; \
		self->sui_item = NULL; \
		self->itmAccessor = NULL; \
	} \
	return (PyObject *) self; \
}



#define SUIPY_DEFINE_ITEMWRAPPER(basename, itmAccessor, itmType) \
		SUIPY_DEFINE_BOILERPLATEWRAPPER(basename, SerialUI::Menu::Item::Item, \
				SerialUI::Menu::Item::Type::Value, itmAccessor, itmType) \
		static void basename##_dealloc(basename##Object *self) { \
					std::cerr << "DESTROYING PYobj " << (int)self->id << std::endl; \
					Py_XDECREF(self->key); \
					Py_XDECREF(self->help); \
					for (uint8_t i=0; i<SUIPY_WRAPPER_MAXCALLBACKS; i++) { \
						if (self->callbacks[i]) { \
							Py_XDECREF(self->callbacks[i]);\
						}\
					}\
					uint8_t id = self->id; \
					Py_TYPE(self)->tp_free((PyObject *) self); \
					SerialUI::Python::SUIPyObjectsStore.deleteEntryFor(id, (PyObject*)self); \
				} \






#define SUIPY_DEFINE_TRACKESTATEWRAPPER(basename, itmAccessor, itmType) \
		SUIPY_DEFINE_BOILERPLATEWRAPPER(basename, SerialUI::Tracked::State, \
				SerialUI::Tracked::Type::Value, itmAccessor, itmType) \
		static void basename##_dealloc(basename##Object *self) { \
					std::cerr << "DESTROYING PYobj " << (int)self->id << std::endl; \
					Py_XDECREF(self->key); \
					Py_XDECREF(self->help); \
					for (uint8_t i=0; i<SUIPY_WRAPPER_MAXCALLBACKS; i++) { \
						if (self->callbacks[i]) { \
							Py_XDECREF(self->callbacks[i]);\
						}\
					}\
					Py_TYPE(self)->tp_free((PyObject *) self); \
				} \




#define SUIPY_DEFINE_ITEMWRAPPERXXX(basename, itmAccessor, itmType) \
typedef struct { \
	PyObject_HEAD \
	PyObject *key; \
	PyObject *help; \
	uint8_t id; \
	SerialUI::Menu::Item::Item * sui_item; \
	SerialUI::Menu::Item::Type::Value type; \
	itmType * itmAccessor; \
	PyObject * callbacks[SUIPY_WRAPPER_MAXCALLBACKS]; \
} basename##Object; \
static PyMemberDef basename##_members[] = { \
		{ "key", T_OBJECT_EX, offsetof(basename##Object, key), 0, "key/name" }, \
		{ "help", T_OBJECT_EX, offsetof(basename##Object, help), 0, "help string" }, \
		{ "id", T_INT, offsetof(basename##Object, id), 0, "item id" }, \
		{ "type", T_INT, offsetof(basename##Object, type), 0, "type id" }, \
		{ NULL } \
}; \
 static void basename##_dealloc(basename##Object *self) { \
	std::cerr << "DESTROYING PYobj " << (int)self->id << std::endl; \
	Py_XDECREF(self->key); \
	Py_XDECREF(self->help); \
	for (uint8_t i=0; i<SUIPY_WRAPPER_MAXCALLBACKS; i++) { \
		if (self->callbacks[i]) { \
			Py_XDECREF(self->callbacks[i]);\
		}\
	}\
	uint8_t id = self->id; \
	Py_TYPE(self)->tp_free((PyObject *) self); \
	SerialUI::Python::SUIPyObjectsStore.deleteEntryFor(id, (PyObject*)self); \
} \
static PyObject * basename##_new(PyTypeObject *type,  \
	PyObject *args, PyObject *kwds) { \
	basename##Object *self; \
	self = (basename##Object *) type->tp_alloc(type, 0); \
	if (self != NULL) { \
		self->key = PyUnicode_FromString(""); \
		if (self->key == NULL) { \
			Py_DECREF(self); \
			return NULL; \
		} \
		self->help = PyUnicode_FromString(""); \
		if (self->help == NULL) { \
			Py_DECREF(self); \
			return NULL; \
		} \
		for (uint8_t i=0; i<SUIPY_WRAPPER_MAXCALLBACKS; i++) { \
			self->callbacks[i] = NULL; \
		}\
		self->id = 0; \
		self->sui_item = NULL; \
		self->itmAccessor = NULL; \
	} \
	return (PyObject *) self; \
}

#define SUIPY_DEFINE_ITEMPYTHONTYPE(basename, tpname, tpdoc)  \
PyTypeObject basename##Type = { \
PyVarObject_HEAD_INIT(NULL, 0) \
		.tp_name = tpname, \
		.tp_basicsize = sizeof(basename##Object), \
		.tp_itemsize = 0, \
		.tp_dealloc = (destructor) basename##_dealloc, \
		.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, \
		.tp_doc = tpdoc, \
		.tp_methods = basename##_methods, \
		.tp_members = basename##_members, \
		.tp_init = (initproc) basename##_init, \
		.tp_new = basename##_new, \
};



SUIPY_DEFINE_ITEMWRAPPER(SUIInputWrapper, request,
		SerialUI::Menu::Item::Request::Request
		);


/* SUIItemContainerObject
 * SerialUI container (submenu/group/etc)
 * object wrapper, c-side structure
 */
SUIPY_DEFINE_ITEMWRAPPER(SUIItemContainer, container,
		SerialUI::Menu::Item::SubMenu
		);

/* SUIItemContainerObject
 * SerialUI container (submenu/group/etc)
 * object wrapper, c-side structure
 */
SUIPY_DEFINE_ITEMWRAPPER(SUICommandWrapper, command,
		SerialUI::Menu::Item::Command
		);





SUIPY_DEFINE_TRACKESTATEWRAPPER(SUITrackedState, state_obj, SerialUI::Tracked::State);



extern PyTypeObject SUIItemContainerType; // forward decl

/*
 * **** Module-level functions ****
 */

static long pysui_doprint(PyObject* args, PyObject *printme) {
	long retVal = 0;


	// SerialUI::Globals::commChannel()->printCommandProcessingStart();
	if (PyBool_Check(printme)) {
		if (PyObject_IsTrue(printme)) {
			SerialUI::Globals::commChannel()->print((int)1);
		} else {
			SerialUI::Globals::commChannel()->print((int)0);
		}
		return 1;
	}

	if (PyNumber_Check(printme)) {
		if (PyFloat_Check(printme)) {
			return SerialUI::Globals::commChannel()->print(PyFloat_AsDouble(printme));
		}

		if (PyLong_Check(printme)) {
			return SerialUI::Globals::commChannel()->print(PyLong_AsLong(printme));
		}
		return 0;
	}

	if (PyUnicode_Check(printme)) {

		// UGH dunno how to deal with this crap
		// TODO:FIXME PyUnicode_AsASCIIString(printme);
		char * strtoprint = NULL;
		if (!PyArg_ParseTuple(args, "s", &strtoprint)) {
			std::cerr << "AAAAGH could not parse str?";
			return 0;
		}
		return SerialUI::Globals::commChannel()->print(strtoprint);
	}



	std::cerr << "Have no idea how to print this... " << std::endl;

	return 0;
}



static PyObject* pysui_print(PyObject* self, PyObject* args) {
	/*char * strtoprint = NULL;
	if (!PyArg_ParseTuple(args, "s", &strtoprint)) {
		std::cerr << "AAAAGH could not parse str?";
		return PyLong_FromLong(0);
	}
	*/

	PyObject *printme;
	PyObject *param = NULL;

	if (!PyArg_UnpackTuple(args, "printval", 1, 2, &printme, &param)) {
		return PyLong_FromLong(0);
	}


	if (PyList_Check(printme)) {
		return PyLong_FromLong(0);

	}
	// SerialUI::Globals::commChannel()->printCommandProcessingStart();
	long retVal = pysui_doprint(args, printme);
	// SerialUI::Globals::commChannel()->printCommandProcessingDone();

	return PyLong_FromLong(retVal);
#if 0
	// SerialUI::Globals::commChannel()->printCommandProcessingStart();
	if (PyBool_Check(printme)) {
		if (PyObject_IsTrue(printme)) {
			SerialUI::Globals::commChannel()->print((int)1);
		} else {
			SerialUI::Globals::commChannel()->print((int)0);
		}
		return PyLong_FromLong(1);
	}

	if (PyNumber_Check(printme)) {
		if (PyFloat_Check(printme)) {
			return PyLong_FromLong(SerialUI::Globals::commChannel()->print(PyFloat_AsDouble(printme)));
		}

		if (PyLong_Check(printme)) {
			return PyLong_FromLong(SerialUI::Globals::commChannel()->print(PyLong_AsLong(printme)));
		}
		return PyLong_FromLong(0);
	}

	if (PyUnicode_Check(printme)) {

		// UGH dunno how to deal with this crap
		// TODO:FIXME PyUnicode_AsASCIIString(printme);
		char * strtoprint = NULL;
		if (!PyArg_ParseTuple(args, "s", &strtoprint)) {
			std::cerr << "AAAAGH could not parse str?";
			return PyLong_FromLong(0);
		}
		return PyLong_FromLong(SerialUI::Globals::commChannel()->print(strtoprint));
	}



	std::cerr << "Have no idea how to print this... " << std::endl;

	return PyLong_FromLong(0);
	/*
	 if(PyArg_UnpackTuple(args, "", 1, 1, &a))
	 {
	 std::cout << "HOHOHOHO:" << PyUnicode_AsWideCharString(a, NULL);
	 }
	 */

	// return PyLong_FromLong(1);

#endif
}



static PyObject* pysui_println(PyObject* self, PyObject* args) {

	PyObject* retVal = pysui_print(self, args);
	PyObject * tmp;
	if (retVal && PyLong_Check(retVal) && (PyLong_AsLong(retVal) > 0) ) {
		SerialUI::Globals::commChannel()->print('\n');
		tmp = retVal;
		retVal = PyLong_FromLong(PyLong_AsLong(retVal) + 1);
		Py_DECREF(tmp);
	}

	return retVal;

}



static PyObject* pysui_test(PyObject* self, PyObject* args) {
	PyObject * ho = NULL;
	if (!PyArg_ParseTuple(args, "O", &ho)) {
		std::cerr << "AAAAGH could not parse str?";
		return PyLong_FromLong(0);
	}

	SUIInputWrapperObject * myObj = (SUIInputWrapperObject*)ho;

	std::cout << "GOTS the obj!" << (int)myObj->id << std::endl;

	PyObject_CallMethod(ho, "changed", "()");
	// TODO:FIXME decref the return of above?

	Py_RETURN_NONE;
}


static PyObject* pysui_top(PyObject* self, PyObject* args) {

	SerialUI::Menu::Item::SubMenu * topMen = SerialUI::Globals::menuStructure()->topLevelMenu();

	PyObject *argList = Py_BuildValue("si", topMen->key(), topMen->id());

	PyObject* retVal = PyObject_CallObject((PyObject *) &SUIItemContainerType, argList);
	Py_DECREF(argList);
	return retVal;
}


/*
 * Python object wrappers
 */

SerialUI::Menu::Item::Item * PySUI_getMenuItem(PyObject * key, uint8_t id) {

	if (id) {
		// std::cerr << "Getting menustruct itm by id " << (int)id << std::endl;
		return SerialUI::Globals::menuStructure()->itemById(id);
	}
	if (!key) {
		std::cerr << "no id, no key?" << std::endl;
		return NULL;
	}
	char * key_str = PyUnicode_AsUTF8(key);
	if (!key_str) {
		std::cerr << "can't get key str?" << std::endl;
		return NULL;
	}
	// std::cerr << "Getting menustruct itm by key:" << key_str << std::endl;
	return SerialUI::Globals::menuStructure()->itemByKey(key_str);

}



/*  ********************** SUI Input Request wrapper implementation ************** */
static int SUIInputWrapper_init(SUIInputWrapperObject *self, PyObject *args,
		PyObject *kwds) {
	static char *kwlist[] = { "key", "id", NULL };
	PyObject *key = NULL, *tmp;

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|Oi", kwlist, &key,
			&self->id)) {
		std::cerr << "SUIInputWrapper_init unhappy tuple/keyw parse" << std::endl;
		return -1;
	}

	if (key) {
		tmp = self->key;
		Py_INCREF(key);
		self->key = key;
		Py_XDECREF(tmp);
	}

  SerialUI::Menu::Item::Item * itm = PySUI_getMenuItem(self->key, self->id);
  if (! itm) {
	  std::cerr << "SUIInputWrapper_init could not getMenuItem" << std::endl;
	  return -1;
  }
  self->sui_item = itm;
  self->id = itm->id();
  self->type = itm->type();
  SerialUI::Python::SUIPyObjectsStore.addInput(self->id, (PyObject*)self);

  SerialUI::Menu::Item::Request::Request * req = SerialUI::Globals::menuStructure()->getRequestById(self->id);
  if (! req ) {
	  std::cerr << "menu item " << (int)self->id << " is not a request??" << std::endl;
	  return -1;
  }
  self->request = req;

  return 0;
}


static PyObject *
SUIInputWrapper_value(SUIInputWrapperObject *self, PyObject * args) {
	switch (self->request->requestType()) {
	case SerialUI::Menu::Item::Request::Type::Boolean:
		/* fall-through */
	case SerialUI::Menu::Item::Request::Type::Toggle:
		if (self->request->castAsSubType<SerialUI::Menu::Item::Request::Boolean>()->currentValue()) {
			Py_RETURN_TRUE;
		}
		Py_RETURN_FALSE;


	case SerialUI::Menu::Item::Request::Type::Passphrase:
		/* fall-through */
	case SerialUI::Menu::Item::Request::Type::String:
		return PyUnicode_FromString(self->request->castAsSubType<SerialUI::Menu::Item::Request::String>()->currentValue().c_str());

	case SerialUI::Menu::Item::Request::Type::BoundedLongInt:
		/* fall-through */
	case SerialUI::Menu::Item::Request::Type::LongInt:
		/* fall-through */
	case SerialUI::Menu::Item::Request::Type::Color:
		/* fall-through */
	case SerialUI::Menu::Item::Request::Type::UnsignedLongInt:
		return PyLong_FromLong(self->request->castAsSubType<SerialUI::Menu::Item::Request::Long>()->currentValue());

	case SerialUI::Menu::Item::Request::Type::OptionsList:
		return PyLong_FromLong(self->request->castAsSubType<SerialUI::Menu::Item::Request::OptionsList>()->currentValue());


	case SerialUI::Menu::Item::Request::Type::Float:
		return PyFloat_FromDouble(self->request->castAsSubType<SerialUI::Menu::Item::Request::Float>()->currentValue());

	case SerialUI::Menu::Item::Request::Type::DateTime:
		return PyLong_FromLong(self->request->castAsSubType<SerialUI::Menu::Item::Request::DateTime>()->currentValue());

	case SerialUI::Menu::Item::Request::Type::Time:
		return PyLong_FromLong(self->request->castAsSubType<SerialUI::Menu::Item::Request::Time>()->currentValue());




	default:
		Py_RETURN_NONE;

	}

}

static PyObject * SUIInputWrapper_setValueBoolean(SUIInputWrapperObject *self, PyObject * args) {

	bool ho;
	if (!PyArg_ParseTuple(args, "b", &ho)) {
		std::cerr << "unhappy tuple/keyw parse" << std::endl;
		Py_RETURN_FALSE;
	}
	self->request->castAsSubType<SerialUI::Menu::Item::Request::Boolean>()->setValue(ho ? true : false);

	SerialUI::Globals::pythonModule()->updated(self->request);
	Py_RETURN_TRUE;

}
static PyObject * SUIInputWrapper_setValueString(SUIInputWrapperObject *self, PyObject * args) {

	char * strtoSet = NULL;
	if (!PyArg_ParseTuple(args, "s", &strtoSet)) {
		std::cerr << "unhappy tuple/keyw parse" << std::endl;
		Py_RETURN_FALSE;
	}

	self->request->castAsSubType<SerialUI::Menu::Item::Request::String>()->setValue(
			strtoSet);
	SerialUI::Globals::pythonModule()->updated(self->request);
	Py_RETURN_TRUE;

}

static PyObject * SUIInputWrapper_setValueLong(SUIInputWrapperObject *self, PyObject * args) {

	long int val = 0;
	if (!PyArg_ParseTuple(args, "l", &val)) {
		std::cerr << "unhappy tuple/keyw parse" << std::endl;
		Py_RETURN_FALSE;
	}

	self->request->castAsSubType<SerialUI::Menu::Item::Request::Long>()->setValue(
			val);

	SerialUI::Globals::pythonModule()->updated(self->request);
	Py_RETURN_TRUE;

}

static PyObject * SUIInputWrapper_setValueUnsignedLong(SUIInputWrapperObject *self, PyObject * args) {
	unsigned long val = 0;
	if (!PyArg_ParseTuple(args, "k", &val)) {
		std::cerr << "unhappy tuple/keyw parse" << std::endl;
		Py_RETURN_FALSE;
	}

	self->request->castAsSubType<SerialUI::Menu::Item::Request::UnsignedLong>()->setValue(
			val);

	SerialUI::Globals::pythonModule()->updated(self->request);
	Py_RETURN_TRUE;

}
static PyObject * SUIInputWrapper_setValueBoundedLong(SUIInputWrapperObject *self, PyObject * args) {
	unsigned long val = 0;
	if (!PyArg_ParseTuple(args, "k", &val)) {
		std::cerr << "unhappy tuple/keyw parse" << std::endl;
		Py_RETURN_FALSE;
	}

	SerialUI::Menu::Item::Request::BoundedLong * bLongReq =
			self->request->castAsSubType<SerialUI::Menu::Item::Request::BoundedLong>();

	if (val < bLongReq->minimum()) {
		Py_RETURN_FALSE;
	}
	if (val > bLongReq->maximum()) {
		Py_RETURN_FALSE;

	}
	bLongReq->setValue(val);
	SerialUI::Globals::pythonModule()->updated(self->request);

	Py_RETURN_TRUE;
}

static PyObject * SUIInputWrapper_setValueFloat(SUIInputWrapperObject *self, PyObject * args) {

	float val = 0;
	if (!PyArg_ParseTuple(args, "f", &val)) {
		std::cerr << "unhappy tuple/keyw parse" << std::endl;
		Py_RETURN_FALSE;
	}

	self->request->castAsSubType<SerialUI::Menu::Item::Request::Float>()->setValue(
			val);

	SerialUI::Globals::pythonModule()->updated(self->request);
	Py_RETURN_TRUE;

}


static PyObject *
SUIInputWrapper_setValue(SUIInputWrapperObject *self, PyObject * args) {


	switch (self->request->requestType()) {
	case SerialUI::Menu::Item::Request::Type::Boolean:
		/* fall-through */
	case SerialUI::Menu::Item::Request::Type::Toggle:
		return SUIInputWrapper_setValueBoolean(self, args);


	case SerialUI::Menu::Item::Request::Type::Passphrase:
		/* fall-through */
	case SerialUI::Menu::Item::Request::Type::String:
		return SUIInputWrapper_setValueString(self, args);



	case SerialUI::Menu::Item::Request::Type::LongInt:
		return SUIInputWrapper_setValueLong(self, args);


	case SerialUI::Menu::Item::Request::Type::DateTime:
		/* fall-through */
	case SerialUI::Menu::Item::Request::Type::Time:
		/* fall-through */
	case SerialUI::Menu::Item::Request::Type::Color:
		/* fall-through */
	case SerialUI::Menu::Item::Request::Type::UnsignedLongInt:
		return SUIInputWrapper_setValueUnsignedLong(self, args);


	case SerialUI::Menu::Item::Request::Type::BoundedLongInt:
		/* fall-through */
	case SerialUI::Menu::Item::Request::Type::OptionsList:
		return SUIInputWrapper_setValueBoundedLong(self, args);



	case SerialUI::Menu::Item::Request::Type::Float:
		return SUIInputWrapper_setValueFloat(self, args);





	default:
		Py_RETURN_NONE;

	}

}
static PyObject *
SUIInputWrapper_changed(SUIInputWrapperObject *self, PyObject * args) {
	if (self->callbacks[0]) {
		std::cerr << "Have callback set, triggering that" << std::endl;
		return PyObject_CallObject(self->callbacks[0], nullptr);
	} else {

		std::cerr << "Default changed() triggered" << std::endl;
	}

	Py_RETURN_NONE;
}

static PyObject *
SUIInputWrapper_setOnChange(SUIInputWrapperObject *self, PyObject * args) {
	std::cout << "input setOnChange" << std::endl;
	PyObject * callback;
	if (PyArg_UnpackTuple(args, "ref", 1, 1, &callback)) {

		if (PyCallable_Check(callback)) {
			Py_INCREF(callback);
			if (self->callbacks[0]) {
				Py_DECREF(self->callbacks[0]);
			}
			self->callbacks[0] = callback;
			Py_RETURN_TRUE;
		}
    }
	Py_RETURN_FALSE;
}

/*
 * SUIInputWrapper methods listing
 */

static PyMethodDef SUIInputWrapper_methods[] = {
		{ "value", (PyCFunction) SUIInputWrapper_value, METH_NOARGS,
				"Return the current value of the input" },
		{ "setValue", (PyCFunction) SUIInputWrapper_setValue, METH_VARARGS,
						"Set the current value of the input" },

		{ "changed", (PyCFunction) SUIInputWrapper_changed, METH_NOARGS,
				"method triggered on change" },
		{"setOnChange", (PyCFunction)SUIInputWrapper_setOnChange,
						METH_VARARGS,
						"set method triggered on change" },


		{ NULL } /* Sentinel */
};


SUIPY_DEFINE_ITEMPYTHONTYPE(SUIInputWrapper, "SerialUI.Input", "SerialUI input object");







/*  ********************** SUI Item container wrapper ************** */
static int SUICommandWrapper_init(SUICommandWrapperObject *self, PyObject *args,
		PyObject *kwds) {
	static char *kwlist[] = { "key", "id", NULL };
	PyObject *key = NULL, *tmp;


	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|Oi", kwlist, &key,
			&self->id))
		return -1;

	if (key) {
		tmp = self->key;
		Py_INCREF(key);
		self->key = key;
		Py_XDECREF(tmp);
	}

  SerialUI::Menu::Item::Item * itm = PySUI_getMenuItem(self->key, self->id);
  if (! itm) {
	  return -1;
  }

  self->sui_item = itm;
  self->id = itm->id();
  self->type = itm->type();

  SerialUI::Python::SUIPyObjectsStore.addCommand(self->id, (PyObject*)self);

  SerialUI::Menu::Item::Command * cmd
  	  = SerialUI::Globals::menuStructure()->getCommandById(self->id);

  if (! cmd ) {
	  return -1;
  }
  self->command = cmd;

  return 0;
}


static PyObject *
SUICommandWrapper_setOnTrigger(SUIInputWrapperObject *self, PyObject * args) {
	std::cout << "command setOnTrigger" << std::endl;
	PyObject * callback;
	if (PyArg_UnpackTuple(args, "callback", 1, 1, &callback)) {

		if (PyCallable_Check(callback)) {
			Py_INCREF(callback);
			if (self->callbacks[0]) {
				Py_DECREF(self->callbacks[0]);
			}
			self->callbacks[0] = callback;
			Py_RETURN_TRUE;
		}
    }
	Py_RETURN_FALSE;
}

static PyObject *
SUICommandWrapper_triggered(SUICommandWrapperObject *self, PyObject * args) {
	if (self->callbacks[0]) {
		std::cerr << "Calling trigger override" << std::endl;
		return PyObject_CallObject(self->callbacks[0], nullptr);
	} else {
		std::cout << "Default command trigger()" << std::endl;
	}
	Py_RETURN_NONE;
}


/*
 * SUIItemContainer methods listing
 */

static PyMethodDef SUICommandWrapper_methods[] = {
		{ "triggered", (PyCFunction) SUICommandWrapper_triggered, METH_NOARGS,
				"Called when command is triggered" },
		{ "setOnTriggered", (PyCFunction) SUICommandWrapper_setOnTrigger, METH_VARARGS,
						"Override tiggered() callback" },


		{ NULL } /* Sentinel */
};

SUIPY_DEFINE_ITEMPYTHONTYPE(SUICommandWrapper, "SerialUI.Command", "SerialUI command object");






/*  ********************** SUI Item container wrapper ************** */
static int SUIItemContainer_init(SUIItemContainerObject *self, PyObject *args,
		PyObject *kwds) {
	static char *kwlist[] = { "key", "id", NULL };
	PyObject *key = NULL, *tmp;

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|Oi", kwlist, &key,
			&self->id)) {
		std::cerr << "SUIItemContainer_init unhappy tuple/keyw parse" << std::endl;
		return -1;
	}

	if (key) {
		tmp = self->key;
		Py_INCREF(key);
		self->key = key;
		Py_XDECREF(tmp);
	}

  SerialUI::Menu::Item::Item * itm = PySUI_getMenuItem(self->key, self->id);
  if (! itm) {
	  std::cerr << "SUIItemContainer_init can't find itm" << std::endl;
	  return -1;
  }
  self->sui_item = itm;
  self->id = itm->id();
  self->type = itm->type();

  SerialUI::Menu::Item::SubMenu * submen
  	  // = SerialUI::Globals::menuStructure()->getSubMenuById(self->id);
	  = SerialUI::Globals::menuStructure()->getItemContainerById(self->id);

  if (! submen ) {
	  std::cerr << "SUIItemContainer_init itm " << (int)self->id << " is not a container" << std::endl;
	  return -1;
  }
  self->container = submen;

  return 0;
}


static PyObject *
SUIItemContainer_children(SUIItemContainerObject *self, PyObject * args) {
	uint8_t numChildren = self->container->numItems();

	// PyObject ** children = new PyObject *[numChildren];
	PyObject * childList = PyList_New(0);

	if (! childList) {
		Py_RETURN_NONE;
	}

	for (uint8_t i=0; i<numChildren; i++) {
		SerialUI::Menu::Item::Item * itm = self->container->itemByIndex(i);

		PyObject *argList = Py_BuildValue("si", itm->key(), itm->id());

		PyObject * curObj = NULL;

		switch (itm->type()) {
		case SerialUI::Menu::Item::Type::Input:
			// std::cerr << "create input" << std::endl;
			curObj = PyObject_CallObject((PyObject *) &SUIInputWrapperType, argList);
			break;
		case SerialUI::Menu::Item::Type::Menu:
			/* fall-through */
		case SerialUI::Menu::Item::Type::Group:
			/* fall-through */
		case SerialUI::Menu::Item::Type::List:
			// std::cerr << "create menu" << std::endl;
			curObj = PyObject_CallObject((PyObject *) &SUIItemContainerType, argList);
			break;

		case SerialUI::Menu::Item::Type::Command:
			// std::cerr << "create command" << std::endl;
			curObj = PyObject_CallObject((PyObject *) &SUICommandWrapperType, argList);
			break;

		default:
			curObj = NULL;
			break;



		}

		if (curObj) {
			// std::cerr << "APPENDING OBJ TO LIST, (" << (int)itm->id() << ") ref:" << curObj->ob_refcnt << std::endl;
			PyList_Append(childList, curObj);
			Py_DECREF(curObj);
			// std::cerr << "APPENDED OBJ TO LIST, ref:" << curObj->ob_refcnt << std::endl;
		}

		/* Release the argument list. */
		Py_DECREF(argList);

	}

	return childList;
}


/*
 * SUIItemContainer methods listing
 */

static PyMethodDef SUIItemContainer_methods[] = {
		{ "children", (PyCFunction) SUIItemContainer_children, METH_NOARGS,
				"Return the current value of the input" },


		{ NULL } /* Sentinel */
};

SUIPY_DEFINE_ITEMPYTHONTYPE(SUIItemContainer, "SerialUI.ItemContainer", "SerialUI item container object");
























/*  ********************** SUI Item container wrapper ************** */
static int SUITrackedState_init(SUITrackedStateObject *self, PyObject *args,
		PyObject *kwds) {
	static char *kwlist[] = { "key", "id", NULL };
	PyObject *key = NULL, *tmp;

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|Oi", kwlist, &key,
			&self->id)) {
		std::cerr << "SUIItemContainer_init unhappy tuple/keyw parse"
				<< std::endl;
		return -1;
	}

	if (key) {
		tmp = self->key;
		Py_INCREF(key);
		self->key = key;
		Py_XDECREF(tmp);

		if (!self->id) {

			char * key_str = PyUnicode_AsUTF8(key);
			if (!key_str) {
				std::cerr << "can't get key str?" << std::endl;
				return -1;
			}
			self->sui_item = SerialUI::Globals::trackedStates()->itemByName(key_str);
		}
	}
	if (!self->sui_item) {

		self->sui_item = SerialUI::Globals::trackedStates()->getItemById(
				self->id);

	}

	if (!self->sui_item) {

		std::cerr << "Can't find Tracked state for id/key?" << std::endl;
		return -1;
	}

	self->state_obj = self->sui_item;
	self->id = self->sui_item->id();
	self->type = self->state_obj->type();

	return 0;
}




static PyObject *
SUITrackedState_value(SUITrackedStateObject *self, PyObject * args) {
	switch(self->state_obj->type()) {
	case SerialUI::Tracked::Type::Toggle:
		/* fall-through */
	case SerialUI::Tracked::Type::Bool:
		if ((bool)(*(self->state_obj->castAsSubType<SerialUI::Tracked::Boolean>()))) {
			Py_RETURN_TRUE;
		}
		Py_RETURN_FALSE;

	case SerialUI::Tracked::Type::Float:
		return PyFloat_FromDouble((float)(*(self->state_obj->castAsSubType<SerialUI::Tracked::Float>())));

	case SerialUI::Tracked::Type::Int:
		return PyLong_FromLong((int)(*(self->state_obj->castAsSubType<SerialUI::Tracked::Integer>())));

	case SerialUI::Tracked::Type::UInt:
		return PyLong_FromUnsignedLong((unsigned int)(*(self->state_obj->castAsSubType<SerialUI::Tracked::UnsignedInteger>())));

	case SerialUI::Tracked::Type::OptionsList:
		/* fall-through */
	case SerialUI::Tracked::Type::BoundedLongInt:
		return PyLong_FromLong((long int)(*(self->state_obj->castAsSubType<SerialUI::Tracked::BoundedLongInt>())));


	case SerialUI::Tracked::Type::Event:
		/* fall-through */
	case SerialUI::Tracked::Type::Time:
		/* fall-through */
	case SerialUI::Tracked::Type::DateTime:
		return PyLong_FromUnsignedLong((unsigned long int)(*(self->state_obj->castAsSubType<SerialUI::Tracked::DateTime>())));


	case SerialUI::Tracked::Type::String:
		return PyUnicode_FromString(((String)(*(self->state_obj->castAsSubType<SerialUI::Tracked::String>()))).c_str());

	default:
		Py_RETURN_NOTIMPLEMENTED;
	}

}


template<class ARGTYPE, class STATETYPE>
static PyObject *
SUITrackedState_setValueAs(SUITrackedStateObject *self, PyObject * args, const char * formatStr) {
	ARGTYPE ho;
		if (!PyArg_ParseTuple(args, formatStr, &ho)) {
				std::cerr << "unhappy tuple/keyw parse" << std::endl;
				Py_RETURN_FALSE;
		}
		*(self->state_obj->castAsSubType<STATETYPE>()) = ho;

		SerialUI::Globals::pythonModule()->updated(self->state_obj);

		Py_RETURN_TRUE;
}


static PyObject *
SUITrackedState_setValue(SUITrackedStateObject *self, PyObject * args) {

	switch(self->state_obj->type()) {
	case SerialUI::Tracked::Type::Toggle:
		/* fall-through */
	case SerialUI::Tracked::Type::Bool:
		return SUITrackedState_setValueAs<bool, SerialUI::Tracked::Boolean>(self, args, "b");


	case SerialUI::Tracked::Type::Float:
		return SUITrackedState_setValueAs<float, SerialUI::Tracked::Float>(self, args, "f");


	case SerialUI::Tracked::Type::Int:
		return SUITrackedState_setValueAs<int, SerialUI::Tracked::Integer>(self, args, "i");

	case SerialUI::Tracked::Type::UInt:
		return SUITrackedState_setValueAs<unsigned int, SerialUI::Tracked::UnsignedInteger>(self, args, "k");

	case SerialUI::Tracked::Type::OptionsList:
		/* fall-through */
	case SerialUI::Tracked::Type::BoundedLongInt:
		return SUITrackedState_setValueAs<long int, SerialUI::Tracked::BoundedLongInt>(self, args, "l");



	case SerialUI::Tracked::Type::Event:
		/* fall-through */
	case SerialUI::Tracked::Type::Time:
		/* fall-through */
	case SerialUI::Tracked::Type::DateTime:
		return SUITrackedState_setValueAs<unsigned long int, SerialUI::Tracked::DateTime>(self, args, "k");

	case SerialUI::Tracked::Type::String:
		return SUITrackedState_setValueAs<const char *, SerialUI::Tracked::String>(self, args, "s");
	default:
		Py_RETURN_NOTIMPLEMENTED;
	}
}


static PyMethodDef SUITrackedState_methods[] = {
		{ "value", (PyCFunction) SUITrackedState_value, METH_NOARGS,
				"Return the current value of the state" },
		{ "setValue", (PyCFunction) SUITrackedState_setValue, METH_VARARGS,
						"set the state value" },
		{ NULL } /* Sentinel */
};

SUIPY_DEFINE_ITEMPYTHONTYPE(SUITrackedState, "SerialUI.TrackedState", "SerialUI tracked state object");








static void pysui_fillsubmenu_tree(SUIItemContainerObject * containerObj, PyObject * intoDict, bool isTop=false) {

	PyObject * childList = SUIItemContainer_children(containerObj, Py_None);
	for (Py_ssize_t i = 0; i < PyList_Size(childList); i++) {
		PyObject * aChildObj = PyList_GetItem(childList, i); // PyList_GetItem does NOT INCREF
		PyObject * thisChild = NULL;
		PyObject * thisChildKey = NULL;
		if (PyObject_IsInstance(aChildObj,
				reinterpret_cast<PyObject*>(&SUIItemContainerType))) {

			SUIItemContainerObject* contObj = (SUIItemContainerObject*) aChildObj;
			thisChildKey = contObj->key;
			if (contObj->type == SerialUI::Menu::Item::Type::List) {
				thisChild = PyList_New(0);
			} else {
				thisChild = PyDict_New();
			}
			pysui_fillsubmenu_tree(contObj, thisChild);

		} else if (PyObject_IsInstance(aChildObj,
				reinterpret_cast<PyObject*>(&SUIInputWrapperType))) {
			SUIInputWrapperObject* inpObj = (SUIInputWrapperObject*) aChildObj;
			thisChildKey = inpObj->key;

			thisChild = aChildObj;

		}else if (PyObject_IsInstance(aChildObj,
				reinterpret_cast<PyObject*>(&SUICommandWrapperType))) {
			SUICommandWrapperObject* cmdObj = (SUICommandWrapperObject*) aChildObj;
			thisChildKey = cmdObj->key;
			thisChild = aChildObj;

		}
		if (thisChild) {
			if (PyList_Check(intoDict)) {
				PyList_Append(intoDict, thisChild);
			} else {

				PyDict_SetItem(intoDict, thisChildKey,
						thisChild);

			}
		}

	}


	Py_DECREF(childList);

}




static PyObject* pysui_tree(PyObject* self, PyObject* args) {
	static PyObject * myTree = NULL;

	if (myTree) {
		Py_INCREF(myTree); // caller will want to cleanup
		return myTree;
	}
	PyObject* topMen = pysui_top(self, args);
	if (! topMen) {
		Py_RETURN_NONE;
	}

	PyObject * topDict = PyDict_New();
	pysui_fillsubmenu_tree((SUIItemContainerObject*) topMen, topDict, true);
	Py_DECREF(topMen);

	myTree = topDict;
	Py_INCREF(myTree);
	return myTree;

}

static PyObject* pysui_trackers(PyObject* self, PyObject* args) {
	static PyObject * myTrackers = NULL;

	if (myTrackers) {
		std::cerr << "TRACKERS: returning cached" << std::endl;

		Py_INCREF(myTrackers); // caller will want to cleanup
		return myTrackers;
	}
	std::cerr << "TRACKERS: building new" << std::endl;
	myTrackers = PyList_New(0);
	if (!myTrackers) {
		Py_RETURN_NONE;
	}

	PyObject * curObj = NULL;

	for (uint8_t i = 0; i < SerialUI::Globals::trackedStates()->numItems();
			i++) {
		SerialUI::Tracked::State * tst =
				SerialUI::Globals::trackedStates()->itemByIndex(i);

		PyObject *argList = Py_BuildValue("si", tst->name(), tst->id());

		curObj = PyObject_CallObject((PyObject *) &SUITrackedStateType, argList);
		if (curObj) {
			// std::cerr << "APPENDING OBJ TO LIST, (" << (int)itm->id() << ") ref:" << curObj->ob_refcnt << std::endl;
			PyList_Append(myTrackers, curObj);
			Py_DECREF(curObj);
			// std::cerr << "APPENDED OBJ TO LIST, ref:" << curObj->ob_refcnt << std::endl;
		}

		/* Release the argument list. */
		Py_DECREF(argList);
	}

	Py_INCREF(myTrackers);
	return myTrackers;

}

static PyObject* pysui_setHeartbeatPeriod(PyObject* self, PyObject* args) {

	long int val = 0;
	if (!PyArg_ParseTuple(args, "l", &val)) {
		std::cerr << "unhappy tuple/keyw parse" << std::endl;
		Py_RETURN_FALSE;
	}
	if (SerialUI::Globals::pythonModule()->setHearbeatPeriod(val)) {
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}


static PyObject* pysui_flush(PyObject* self, PyObject* args) {

	// MESSES EVERYTHING UP SerialUI::Globals::commChannel()->printCommandProcessingDone();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	Py_RETURN_NONE;
}


/*
 * Module-level methods array
 */
static struct PyMethodDef methods[] = {
  { "print", pysui_print, METH_VARARGS, "Output a string" },
  { "println", pysui_println, METH_VARARGS, "Output a string \\n" },
  { "test", pysui_test, METH_VARARGS, "test me" },
  { "top", pysui_top, METH_NOARGS, "top level menu"},
  { "tree", pysui_tree, METH_NOARGS, "full menu tree"},
  { "trackers", pysui_trackers, METH_NOARGS, "list of state trackers"},
  { "flush", pysui_flush, METH_NOARGS, "flush prints/updates"},
  { "setHeartbeatPeriod", pysui_setHeartbeatPeriod, METH_VARARGS, "set heartbeat period (ms)"},
  { NULL, NULL, 0, NULL }
};

/*
 * SerialUI Python module definition
 */
static struct PyModuleDef modDef = {
	PyModuleDef_HEAD_INIT,
    .m_name = "SerialUI",
    .m_doc = "SerialUI Python extension module.",
    .m_size = -1,
    .m_methods = methods
};



PyObject* PyInit_SerialUIModule(void) {
	PyObject *mymodule;
	if (PyType_Ready(&SUIInputWrapperType) < 0) {
		return NULL;
	}
	if (PyType_Ready(&SUIItemContainerType) < 0) {
		return NULL;
	}
	if (PyType_Ready(&SUICommandWrapperType) < 0) {
		return NULL;
	}

	if (PyType_Ready(&SUITrackedStateType) < 0) {
		return NULL;
	}

	mymodule = PyModule_Create(&modDef);
	if (mymodule == NULL) {
		return NULL;
	}

	Py_INCREF(&SUIInputWrapperType);
	Py_INCREF(&SUIItemContainerType);
	Py_INCREF(&SUICommandWrapperType);
	Py_INCREF(&SUITrackedStateType);

	PyModule_AddObject(mymodule, "Input", (PyObject *) &SUIInputWrapperType);
	PyModule_AddObject(mymodule, "ItemContainer", (PyObject *) &SUIItemContainerType);
	PyModule_AddObject(mymodule, "Command", (PyObject *) &SUICommandWrapperType);
	PyModule_AddObject(mymodule, "TrackedState", (PyObject *) &SUITrackedStateType);

	return mymodule;
}





#endif /* SERIALUI_PYTHONMODULES_SUPPORT_ENABLE */



