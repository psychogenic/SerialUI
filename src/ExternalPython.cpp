/*
 * ExternalPython.cpp
 *
 *  Created on: May 11, 2019
 *      Author: Pat Deegan, https://psychogenic.com
 *
 * CPython objects and types.
 *
 * If we want to represent and export "MyClass"
 * and have instances of this class created, we
 *
 *  1) Define a structure, using one of the
 *  	SUIPY_DEFINE_*()
 *  below to create a "MyClassObject".  This structure
 *  holds info related to specific instances of this
 *  type.  The SUIPY_DEFINE_* macros are also used
 *  to generate some standard functions.
 *
 *  After creating actually implementing member methods,
 *  as functions with a specific signature, we create
 *  an array of PyMethodDef, holding
 *  pointers to all the instance methods, we can
 *  define the type itself:
 *
 *
 *  2) Define a Python Type, using
 *    SUIPY_DEFINE_ITEMPYTHONTYPE() (below)
 *  to create a "MyClassType"
 *
 *  This type points to all the common stuff,
 *  alloc/dealloc functions, member functions etc.
 *
 *
 *  3) Hold on to our type, and add it to our
 *  main module
 *     Py_INCREF(&XYZType);
 *     PyModule_AddObject(mymodule, "XYZ", (PyObject *) &XYZType);
 *
 *  and that's pretty much it.
 *
 */


#include "includes/python/ExternalModule.h"

#ifdef SERIALUI_PYTHONMODULES_SUPPORT_ENABLE
#include "includes/python/ExternalPython.h"

#include "includes/SerialUI.h"
#include "includes/SUIGlobals.h"
#include "structmember.h"



#include "includes/auth/AuthStoragePython.h"
#include "includes/auth/Authenticator.h"

#include "includes/auth/AuthValidatorPython.h"

#include <thread>
#include <sstream>
#include <string>

#define SUIPY_CALLOBJECT_REPORTERROR() \
		if (PyErr_Occurred()) { \
				SERIALUI_DEBUG_OUTLN("Callobject returned error"); \
				PyErr_Print(); \
			}


#define OBSTORE_CALLVALIDATORONINPUTS(forId, vformat, val) \
		ItemPyObjectSet inputsToNotify = inputsFor(forId); \
			if (!inputsToNotify.size()) { \
				SERIALUI_DEBUG_OUTLN("No python-side inputs for this req");\
				return true; \
			}\
			for (ItemPyObjectSet::iterator it = inputsToNotify.begin(); \
					it != inputsToNotify.end(); it++) { \
				PyObject* theCb = PyObject_GetAttrString(*it, "isValid"); \
				if (! (theCb && PyCallable_Check(theCb)) ) { \
					SERIALUI_DEBUG_OUTLN("isValid not callable on this obj?"); \
				} else { \
					SERIALUI_DEBUG_OUTLN("Calling isValid() on python obj..."); \
					PyObject * obj = PyObject_CallMethod(*it, "isValid", vformat, val); \
					if (obj) {\
						if (obj != Py_None) { \
							if (PyBool_Check(obj)) { \
								if (! PyObject_IsTrue(obj)) { \
									SERIALUI_DEBUG_OUTLN("declared invalid!"); \
									return false; \
								} \
							} else { \
								SERIALUI_DEBUG_OUTLN("Call returned something non-boolean, ignoring"); \
							}\
						Py_DECREF(obj); \
						} else { \
							SERIALUI_DEBUG_OUTLN("Call returned noneness??"); \
						} \
					} else { \
						if (PyErr_Occurred()) { \
							SERIALUI_DEBUG_OUTLN("Call returned nothing at all?? error set"); \
							PyErr_Print(); \
						} else { \
							SERIALUI_DEBUG_OUTLN("Call returned nothing at all?? NO ERROR SET?? UGH"); \
						} \
					} \
				}\
			}\
			return true

namespace SerialUI {
namespace Python {

/*
 * ObjectsStore -- we want to keep track of all the
 * menu-item related objects created on the fly, down there,
 * so we can trigger callbacks as appropriate from up here.
 *
 * The ObjectsStore holds these instances.
 */
void ObjectsStore::callTriggeredOnCommands(uint8_t forId) {

	ItemPyObjectSet commandsToTrip = commandsFor(forId);
	if (!commandsToTrip.size()) {
		return;
	}

	// Py_BEGIN_ALLOW_THREADS
	for (ItemPyObjectSet::iterator it = commandsToTrip.begin();
			it != commandsToTrip.end(); it++) {

		/*
		PyObject* theCb = PyObject_GetAttrString(*it, "triggered");

		if (! (theCb && PyCallable_Check(theCb)) ) {
			SERIALUI_DEBUG_OUTLN("No triggered found for this command?");
		} else {
			// uuugh
		}
		*/

		PyObject * retObj = PyObject_CallMethod(*it, "triggered", "()");
		if (retObj) {
			Py_DECREF(retObj);
		} else {
			SUIPY_CALLOBJECT_REPORTERROR();
		}
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

		PyObject * retObj = PyObject_CallMethod(*it, "changed", "()");
		if (retObj) {
			Py_DECREF(retObj);
		} else {
			SUIPY_CALLOBJECT_REPORTERROR();
		}
	}

}


bool ObjectsStore::callValidatorOnInputs(uint8_t forId, bool val) {
	SERIALUI_DEBUG_OUTLN("callValidatorOnInputs(BOOL)");
	OBSTORE_CALLVALIDATORONINPUTS(forId, "(i)", (val ? 1 : 0));
}


bool ObjectsStore::callValidatorOnInputs(uint8_t forId, long val) {
	SERIALUI_DEBUG_OUTLN("callValidatorOnInputs(LONG)");
	OBSTORE_CALLVALIDATORONINPUTS(forId, "(l)", val);
}

bool ObjectsStore::callValidatorOnInputs(uint8_t forId, float val) {
	SERIALUI_DEBUG_OUTLN("callValidatorOnInputs(FLOAT)");
	OBSTORE_CALLVALIDATORONINPUTS(forId, "(f)", val);
}
bool ObjectsStore::callValidatorOnInputs(uint8_t forId, unsigned long val) {
	SERIALUI_DEBUG_OUTLN("callValidatorOnInputs(ULONG)");
	OBSTORE_CALLVALIDATORONINPUTS(forId, "(k)", val);
}

bool ObjectsStore::callValidatorOnInputs(uint8_t forId,
		TopLevelString & val) {
	SERIALUI_DEBUG_OUTLN("callValidatorOnInputs(STR)");
	OBSTORE_CALLVALIDATORONINPUTS(forId, "(s)", val.c_str());
}

void ObjectsStore::deleteEntryFor(uint8_t forId, PyObject * obj) {

	if (commands.find(forId) != commands.end()) {
		/*
		for (ItemPyObjectSet::iterator it = commands[forId].begin();
				it != commands[forId].end(); it++) {
			if (*it == obj) {
				// Py_DECREF(obj);
			}
		}
		*/

		SERIALUI_DEBUG_OUTLN("Store ERASE command " << obj);
		commands[forId].erase(obj);

	}
	if (inputs.find(forId) != inputs.end()) {
		/*
		for (ItemPyObjectSet::iterator it = inputs[forId].begin();
				it != inputs[forId].end(); it++) {
			if (*it == obj) {
				// Py_DECREF(obj);
			}
		}
		*/
		SERIALUI_DEBUG_OUTLN("Store ERASE input " << obj);
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
		SERIALUI_DEBUG_OUTLN("Store ADD " << obj );
	} else {
		SERIALUI_DEBUG_OUTLN("Store already knows " << obj);
	}
}

ItemPyObjectSet ObjectsStore::setFor(uint8_t forId, ItemPyObjectById & inMap) {

	if (inMap.find(forId) == inMap.end()) {
		return ItemPyObjectSet();
	}

	return inMap[forId];
}


// gobal python object store
ObjectsStore SUIPyObjectsStore;



} /* namespace Python */
} /* namespace SerialUI */

#define SUIPY_WRAPPER_MAXCALLBACKS		2

#define SUIPYSTRINGIFY(x) #x
#define SUIPYTOSTRING(x) SUIPYSTRINGIFY(x)




#if 0
/*
 * Compilation on the pi is super bitchy...
 * i.e. "non-trivial designated initializers not supported"
 * so the secret seems to be to:
 *  - use .member = value
 *  - for each single value in PyTypeObject
 *  - in the order defined, exactly.
 */
#define SUIPY_DEFINE_ITEMPYTHONTYPE2(basename, tpname, tpdoc)  \
PyTypeObject basename##Type = { \
PyVarObject_HEAD_INIT(NULL, 0) \
		tp_name:tpname, \
		tp_basicsize:sizeof(basename##Object), \
		tp_itemsize:0, \
		tp_dealloc: (destructor) basename##_dealloc, \
		tp_flags: Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, \
		tp_doc: tpdoc, \
		tp_methods: basename##_methods, \
		tp_members: basename##_members, \
		tp_init: (initproc) basename##_init, \
		tp_new: basename##_new \
};
#endif

/* SUIPY_DEFINE_ITEMPYTHONTYPE(XYZ...)
 * define a custom PyTypeObject (i.e. a type, or class), XYZType
 *
 * This declaration has to include aaaalll members,
 * in the right order, to pass muster on the RPi compiler (ugh)
 *
 * Compilation on the pi is super bitchy...
 * i.e. "non-trivial designated initializers not supported"
 * so the secret seems to be to:
 *  - use .member = value
 *  - for each single value in PyTypeObject
 *  - in the order defined, exactly.
 *
 */
#define SUIPY_DEFINE_ITEMPYTHONTYPE(basename, tpname, tpdoc)  \
PyTypeObject basename##Type = { \
		PyVarObject_HEAD_INIT(NULL, 0) \
		.tp_name = tpname, \
		.tp_basicsize = sizeof(basename##Object), \
		.tp_itemsize = 0, \
		.tp_dealloc =  (destructor) basename##_dealloc, \
		.tp_print = 0, \
		.tp_getattr = 0, \
		.tp_setattr = 0, \
		.tp_as_async = 0, \
		.tp_repr = basename##_SUIRepr, \
		.tp_as_number = 0, \
		.tp_as_sequence = 0, \
		.tp_as_mapping = 0, \
		.tp_hash = 0, \
		.tp_call = 0, \
		.tp_str = 0, \
		.tp_getattro = 0, \
		.tp_setattro = 0, \
		.tp_as_buffer = 0, \
		.tp_flags =  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, \
		.tp_doc =  tpdoc, \
		.tp_traverse = 0, \
		.tp_clear = 0, \
		.tp_richcompare = 0, \
		.tp_weaklistoffset = 0, \
		.tp_iter = 0, \
		.tp_iternext = 0, \
		.tp_methods =  basename##_methods, \
		.tp_members =  basename##_members, \
		.tp_getset = 0, \
		.tp_base = 0, \
		.tp_dict = 0, \
		.tp_descr_get = 0, \
		.tp_descr_set = 0, \
		.tp_dictoffset = 0, \
		.tp_init =  (initproc) basename##_init, \
		.tp_alloc = 0, \
		.tp_new =  (newfunc)basename##_new, \
		.tp_free = (freefunc)0, \
		.tp_is_gc = 0, \
		.tp_bases = 0, \
		.tp_mro = 0, \
		.tp_cache = 0, \
		.tp_subclasses = 0, \
		.tp_weaklist = 0, \
		.tp_del = 0, \
		.tp_version_tag = 0, \
		.tp_finalize = 0 \
};













/*
 * SUIPY_DEFINE_BOILERPLATEWRAPPER
 *
 * Creates an XYZObject that holds all the data
 * for a given instance of this new type (see below)
 *
 * Called as SUIPY_DEFINE_BOILERPLATEWRAPPER(XYZname, ...)
 *
 * And will create a corresponding XYZObject.
 *
 * generic pyobject definition for our various menu items.
 * Assumes whatever we're wrapping has:
 *  - id (u8)
 *  - key (string)
 *  - help (string)
 * and provides a basic XYZ_members[] and XYZ_new
 */
#define SUIPY_DEFINE_BOILERPLATEWRAPPER(basename, baseType, typeEnum, itmAccessor, itmType) \
typedef struct { \
	PyObject_HEAD \
	PyObject *key; \
	PyObject *help; \
	uint8_t id; \
	baseType * sui_item; \
	typeEnum type; \
	uint8_t valuetype; \
	itmType * itmAccessor; \
	PyObject * callbacks[SUIPY_WRAPPER_MAXCALLBACKS]; \
} basename##Object; \
static PyMemberDef basename##_members[] = { \
		{ "key", T_OBJECT_EX, offsetof(basename##Object, key), 0, "key/name" }, \
		{ "help", T_OBJECT_EX, offsetof(basename##Object, help), 0, "help string" }, \
		{ "id", T_INT, offsetof(basename##Object, id), 0, "item id" }, \
		{ "type", T_INT, offsetof(basename##Object, type), 0, "item type id" }, \
		{ "valuetype", T_INT, offsetof(basename##Object, valuetype), 0, "item valuetype id" }, \
		{ NULL } \
}; \
static PyObject * basename##_new(PyTypeObject *type,  \
	PyObject *args, PyObject *kwds) { \
	SERIALUI_DEBUG_OUTLN("ALLOC PYobj "); \
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
		self->valuetype = 0; \
		self->sui_item = NULL; \
		self->itmAccessor = NULL; \
	} \
	return (PyObject *) self; \
}



/*
 * SUIPY_DEFINE_ITEMWRAPPER
 * Extension to SUIPY_DEFINE_BOILERPLATEWRAPPER
 * Provides a XYZ_dealloc, with cleanup
 * for items in the SUIPyObjectsStore when PyObjects are destroyed.
 */
#define SUIPY_DEFINE_ITEMWRAPPER(basename, itmAccessor, itmType) \
		SUIPY_DEFINE_BOILERPLATEWRAPPER(basename, SerialUI::Menu::Item::Item, \
				SerialUI::Menu::Item::Type::Value, itmAccessor, itmType) \
		static void basename##_dealloc(basename##Object *self) { \
					SERIALUI_DEBUG_OUTLN("DESTROYING PYobj (item) " << (int)self->id); \
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
				}

/* SUIPY_DEFINE_INPUTWRAPPER
 * Extension to SUIPY_DEFINE_ITEMWRAPPER to add a cleaner
 * repr() method specific to inputs.
 *
 */
#define SUIPY_DEFINE_INPUTWRAPPER(basename, itmAccessor, itmType) \
		SUIPY_DEFINE_ITEMWRAPPER(basename, itmAccessor, itmType) \
static PyObject* basename##_SUIRepr(PyObject*o) { \
	basename##Object * myo = (basename##Object *)o; \
	std::ostringstream s; \
	/* s << "<SerialUI.Input " << SUIPYTOSTRING(basename) << " id=" << (int)myo->id; */ \
	s << "<SerialUI.Input id=" << (int)myo->id; \
	char * key_str = PyUnicode_AsUTF8(myo->key); \
	if (key_str) { \
		s << " key='" << key_str <<"'"; \
	}\
	s << ">"; \
	return PyUnicode_FromString(s.str().c_str()); \
}


/* SUIPY_DEFINE_COMMANDWRAPPER
 *
 * Extension to SUIPY_DEFINE_ITEMWRAPPER to add a cleaner
 * repr() method specific to commands.
 *
 */
#define SUIPY_DEFINE_COMMANDWRAPPER(basename, itmAccessor, itmType) \
		SUIPY_DEFINE_ITEMWRAPPER(basename, itmAccessor, itmType) \
static PyObject* basename##_SUIRepr(PyObject*o) { \
	basename##Object * myo = (basename##Object *)o; \
	std::ostringstream s; \
	/* s << "<SerialUI.Command " << SUIPYTOSTRING(basename) << ' ' << (int)myo->id; */ \
	s << "<SerialUI.Command id=" << (int)myo->id; \
	char * key_str = PyUnicode_AsUTF8(myo->key); \
	if (key_str) { \
		s << " '" << key_str <<"'"; \
	}\
	s << ">"; \
	return PyUnicode_FromString(s.str().c_str()); \
}


/* SUIPY_DEFINE_ITEMCONTAINERWRAPPER
 *
 * Extension to SUIPY_DEFINE_ITEMWRAPPER to add a cleaner
 * repr() method specific to menus/groups.
 *
 */
#define SUIPY_DEFINE_ITEMCONTAINERWRAPPER(basename, itmAccessor, itmType) \
		SUIPY_DEFINE_ITEMWRAPPER(basename, itmAccessor, itmType) \
static PyObject* basename##_SUIRepr(PyObject*o) { \
	basename##Object * myo = (basename##Object *)o; \
	std::ostringstream s; \
	/* s << "<SerialUI.ItemContainer " << SUIPYTOSTRING(basename) << " id=" << (int)myo->id; */ \
	s << "<SerialUI.ItemContainer id=" << (int)myo->id; \
	char * key_str = PyUnicode_AsUTF8(myo->key); \
	if (key_str) { \
		s << " key='" << key_str <<"'"; \
	}\
	s << ">"; \
	return PyUnicode_FromString(s.str().c_str()); \
}




/*
 * SUIPY_DEFINE_TRACKESTATEWRAPPER
 * Extension to basic boilerplate (SUIPY_DEFINE_BOILERPLATEWRAPPER)
 * to add dealloc and repr to tracked states.
 */
#define SUIPY_DEFINE_TRACKESTATEWRAPPER(basename, itmAccessor, itmType) \
		SUIPY_DEFINE_BOILERPLATEWRAPPER(basename, SerialUI::Tracked::State, \
				SerialUI::Tracked::Type::Value, itmAccessor, itmType) \
		static void basename##_dealloc(basename##Object *self) { \
					SERIALUI_DEBUG_OUTLN("DESTROYING PYobj (tracker) " << (int)self->id); \
					Py_XDECREF(self->key); \
					Py_XDECREF(self->help); \
					for (uint8_t i=0; i<SUIPY_WRAPPER_MAXCALLBACKS; i++) { \
						if (self->callbacks[i]) { \
							Py_XDECREF(self->callbacks[i]);\
						}\
					}\
					Py_TYPE(self)->tp_free((PyObject *) self); \
				} \
		static PyObject* basename##_SUIRepr(PyObject*o) { \
					basename##Object * myo = (basename##Object *)o; \
					std::ostringstream s; \
					/* s << "<SerialUI.TrackedState " << SUIPYTOSTRING(basename) << " id=" << (int)myo->id; */ \
					s << "<SerialUI.TrackedState id=" << (int)myo->id; \
					char * key_str = PyUnicode_AsUTF8(myo->key); \
					if (key_str) { \
						s << " key='" << key_str <<"'"; \
					}\
					s << ">"; \
					return PyUnicode_FromString(s.str().c_str()); \
				}






/*
 * SUIInputWrapperObject
 * SerialUI input request object
 * wrapper, c-side structure and members.
 */

SUIPY_DEFINE_INPUTWRAPPER(SUIInputWrapper, request,
		SerialUI::Menu::Item::Request::Request
		);


/* SUIItemContainerObject
 * SerialUI container (submenu/group/etc)
 * object wrapper, c-side structure
 */
SUIPY_DEFINE_ITEMCONTAINERWRAPPER(SUIItemContainer, container,
		SerialUI::Menu::Item::SubMenu
		);

/* SUIItemContainerObject
 * SerialUI container (submenu/group/etc)
 * object wrapper, c-side structure
 */
SUIPY_DEFINE_COMMANDWRAPPER(SUICommandWrapper, command,
		SerialUI::Menu::Item::Command
		);



/*
 * SUITrackedStateObject
 * SerialUI tracked states object wrapper.
 */

SUIPY_DEFINE_TRACKESTATEWRAPPER(SUITrackedState, state_obj, SerialUI::Tracked::State);



extern PyTypeObject SUIItemContainerType; // forward decl
/*
 * **** Module-level functions ****
 */


/*
 * pysui_doprint does the dirty work of "printing"
 * to our remote client
 */
static long pysui_doprint(PyObject* args, PyObject *printme) {
	long retVal = 0;


	// BOOLEAN
	if (PyBool_Check(printme)) {
		if (PyObject_IsTrue(printme)) {

			SerialUI::Globals::commChannel()->print((int)1);
		} else {
			SerialUI::Globals::commChannel()->print((int)0);
		}
		return 1;
	}

	// NUMERIC VALUES
	if (PyNumber_Check(printme)) {
		if (PyFloat_Check(printme)) {
			return SerialUI::Globals::commChannel()->print(PyFloat_AsDouble(printme));
		}

		if (PyLong_Check(printme)) {
			return SerialUI::Globals::commChannel()->print(PyLong_AsLong(printme));
		}
		return 0;
	}

	// STRINGS
	if (PyUnicode_Check(printme)) {

		// UGH dunno how to deal with this crap
		// TODO:FIXME PyUnicode_AsASCIIString(printme);
		char * strtoprint = NULL;
		if (!PyArg_ParseTuple(args, "s", &strtoprint)) {
			SERIALUI_DEBUG_OUTLN("AAAAGH could not parse str?");
			return 0;
		}
		return SerialUI::Globals::commChannel()->print(strtoprint);
	}

	// what else is there in the world?

	SERIALUI_DEBUG_OUTLN("Have no idea how to print this... ");

	return 0;
}


/*
 * pysui_print
 * actual SerialUI.print(...) call.
 * Will send off booleans (as int), numbers and strings...
 * pretty much it for now.
 */
static PyObject* pysui_print(PyObject* self, PyObject* args) {

	PyObject *printme;
	PyObject *param = NULL;

	if (!PyArg_UnpackTuple(args, "printval", 1, 2, &printme, &param)) {
		return PyLong_FromLong(0);
	}


	if (PyList_Check(printme)) {
		return PyLong_FromLong(0);

	}
	long retVal = pysui_doprint(args, printme);

	return PyLong_FromLong(retVal);
}


/* pysui_println
 *
 * SerialUI.println(...) call.  Using print, above, then
 * appends a '\n' on success.
 *
 */
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


/*
 * pysui_test
 * DEADBEEF?  probably should remove
 */
static PyObject* pysui_test(PyObject* self, PyObject* args) {
	PyObject * ho = NULL;
	if (!PyArg_ParseTuple(args, "O", &ho)) {
		SERIALUI_DEBUG_OUTLN("AAAAGH could not parse str?");
		return PyLong_FromLong(0);
	}

	SUIInputWrapperObject * myObj = (SUIInputWrapperObject*)ho;

	SERIALUI_DEBUG_OUTLN("GOTS the obj!" << (int)myObj->id);

	PyObject_CallMethod(ho, "changed", "()");
	// TODO:FIXME decref the return of above?

	Py_RETURN_NONE;
}


/*
 * pysui_top
 *
 * SerialUI.top() which returns the top level menu--the
 * container that holds all other items in a SerialUI
 * interface.
 */
static PyObject* pysui_top(PyObject* self, PyObject* args) {

	SerialUI::Menu::Item::SubMenu * topMen = SerialUI::Globals::menuStructure()->topLevelMenu();

	PyObject *argList = Py_BuildValue("si", topMen->key(), topMen->id());

	PyObject* retVal = PyObject_CallObject((PyObject *) &SUIItemContainerType, argList);
	SUIPY_CALLOBJECT_REPORTERROR();

	Py_DECREF(argList);
	return retVal;
}


/*
 * Python object wrappers
 */

SerialUI::Menu::Item::Item * PySUI_getMenuItem(PyObject * key, uint8_t id) {

	if (id) {
		return SerialUI::Globals::menuStructure()->itemById(id);
	}
	if (!key) {
		SERIALUI_DEBUG_OUTLN("no id, no key?");
		return NULL;
	}
	char * key_str = PyUnicode_AsUTF8(key);
	if (!key_str) {
		SERIALUI_DEBUG_OUTLN("can't get key str?");
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
		SERIALUI_DEBUG_OUTLN("SUIInputWrapper_init unhappy tuple/keyw parse");
		PyErr_SetString(PyExc_RuntimeError, "pass key and/or id");
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
	  SERIALUI_DEBUG_OUTLN("SUIInputWrapper_init could not getMenuItem");
		PyErr_SetString(PyExc_RuntimeError, "can't find this menu item");
	  return -1;
  }
  self->sui_item = itm;
  self->id = itm->id();
  self->type = itm->type();

  SERIALUI_DEBUG_OUTLN("Init request obj for id " << (int)self->id);
  SerialUI::Python::SUIPyObjectsStore.addInput(self->id, (PyObject*)self);

  SerialUI::Menu::Item::Request::Request * req = SerialUI::Globals::menuStructure()->getRequestById(self->id);
  if (! req ) {
	  SERIALUI_DEBUG_OUTLN("menu item " << (int)self->id << " is not a request??");

	  PyErr_SetString(PyExc_RuntimeError, "item not a request");
	  return -1;
  }
  self->request = req;
  self->valuetype = (uint8_t)(req->requestType());

  return 0;
}

static PyObject *
SUIInputWrapper_options(SUIInputWrapperObject *self, PyObject * args) {
	switch (self->request->requestType()) {
	case SerialUI::Menu::Item::Request::Type::OptionsList:
		break;
	default:
		PyErr_SetString(PyExc_RuntimeError, "options only available for OptionsList items");
		Py_RETURN_FALSE;
	}
	PyObject * optsList = PyList_New(0);
	if (! optsList) {
		PyErr_SetString(PyExc_RuntimeError, "can't alloc new list?");
		Py_RETURN_FALSE;
	}

	SerialUI::Menu::Item::Request::OptionsList * optsItem =
			self->request->castAsSubType<SerialUI::Menu::Item::Request::OptionsList>();

	for (long int i=optsItem->minimum(); i <= optsItem->maximum(); i++) {
		SerialUI::StaticString thisOpt = optsItem->optionByIndex(i - 1);
		if (! thisOpt) {
			SERIALUI_DEBUG_OUTLN("No option " << i);
		} else {
			PyObject * optStr = PyUnicode_FromString(thisOpt);
			if (optStr) {
				PyList_Append(optsList, optStr);
				Py_DECREF(optStr);
			} else {
				PyErr_SetString(PyExc_RuntimeError, "couldn't get string for option?");
			}
		}



	}

	return optsList;


}
static PyObject *
SUIInputWrapper_setOptions(SUIInputWrapperObject *self, PyObject * args) {

	SerialUI::StaticString optStrings[SERIALUI_MENUITEM_OPTLIST_MAXOPTIONS];
	switch (self->request->requestType()) {
	case SerialUI::Menu::Item::Request::Type::OptionsList:
		break;
	default:
		PyErr_SetString(PyExc_RuntimeError, "setOptions only available for OptionsList items");
		Py_RETURN_FALSE;
	}


	PyObject * listOfItems;
	if (!PyArg_ParseTuple(args, "O", &listOfItems)) {
		SERIALUI_DEBUG_OUTLN("unhappy tuple/keyw parse");
		Py_RETURN_FALSE;
	}
	if (! PyList_Check(listOfItems) ) {
		PyErr_SetString(PyExc_RuntimeError, "must pass a LIST of strings");
		Py_RETURN_FALSE;
	}
	Py_ssize_t numOpts = PyList_Size(listOfItems);

	if (numOpts > SERIALUI_MENUITEM_OPTLIST_MAXOPTIONS) {

		PyErr_SetString(PyExc_RuntimeError, "Exceeds number of options permissible");
		Py_RETURN_FALSE;
	}

	for (uint8_t i=0; i<SERIALUI_MENUITEM_OPTLIST_MAXOPTIONS; i++) {
		optStrings[i] = NULL;
	}
	for (Py_ssize_t i=0; i<numOpts; i++) {
		PyObject* itm = PyList_GetItem(listOfItems, i);
		if ( (! itm) ) {
			PyErr_SetString(PyExc_RuntimeError, "Could not get item from passed list of options");
			Py_RETURN_FALSE;
		}
		if (! PyUnicode_Check(itm) ) {
			PyErr_SetString(PyExc_RuntimeError, "list must only contain unicode items");
			Py_RETURN_FALSE;
		}

		optStrings[i] = PyUnicode_AsUTF8(itm);

	}

	SerialUI::Menu::Item::Request::OptionsList * optsItem =
			self->request->castAsSubType<SerialUI::Menu::Item::Request::OptionsList>();

	optsItem->setOptions(
			optStrings[0],
			optStrings[1],
			optStrings[2],
			optStrings[3],
			optStrings[4],
			optStrings[5]
	);


	SerialUI::Globals::pythonModule()->updated(self->request);
	Py_RETURN_TRUE;
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

	case SerialUI::Menu::Item::Request::Type::Event:
		return PyLong_FromLong(self->request->castAsSubType<SerialUI::Menu::Item::Request::Event>()->currentValue());

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
		SERIALUI_DEBUG_OUTLN("unhappy tuple/keyw parse");
		Py_RETURN_FALSE;
	}
	self->request->castAsSubType<SerialUI::Menu::Item::Request::Boolean>()->setValue(ho ? true : false);

	SerialUI::Globals::pythonModule()->updated(self->request);
	Py_RETURN_TRUE;

}
static PyObject * SUIInputWrapper_setValueString(SUIInputWrapperObject *self, PyObject * args) {

	char * strtoSet = NULL;
	if (!PyArg_ParseTuple(args, "s", &strtoSet)) {
		SERIALUI_DEBUG_OUTLN("unhappy tuple/keyw parse");
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
		SERIALUI_DEBUG_OUTLN("unhappy tuple/keyw parse");
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
		SERIALUI_DEBUG_OUTLN("unhappy tuple/keyw parse");
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
		SERIALUI_DEBUG_OUTLN("unhappy tuple/keyw parse");
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
		SERIALUI_DEBUG_OUTLN("unhappy tuple/keyw parse");
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
SUIInputWrapper_isValid(SUIInputWrapperObject *self, PyObject * args) {
	SERIALUI_DEBUG_OUT("input isValid() ");
	if (self->callbacks[1]) {
		SERIALUI_DEBUG_OUTLN("Have callback set, triggering that");
		PyObject * retObj = PyObject_CallObject(self->callbacks[1], args);
		if (retObj) {
			return retObj;
		} else {
			SUIPY_CALLOBJECT_REPORTERROR();
			Py_RETURN_TRUE;
		}
	}
	SERIALUI_DEBUG_OUTLN("Default isValid() triggered");

	Py_RETURN_TRUE;

}

static PyObject *
SUIInputWrapper_changed(SUIInputWrapperObject *self, PyObject * args) {
	SERIALUI_DEBUG_OUT("input changed() ");
	if (self->callbacks[0]) {
		SERIALUI_DEBUG_OUTLN("Have callback set, triggering that");

		CPyObject argList = Py_BuildValue("(O)", self);
		PyObject * retObj = PyObject_CallObject(self->callbacks[0], argList);
		if (retObj) {
			return retObj;
		} else {
			SUIPY_CALLOBJECT_REPORTERROR();
			Py_RETURN_NONE;
		}
	}

	SERIALUI_DEBUG_OUTLN("Default changed() triggered");

	Py_RETURN_NONE;
}


static PyObject *
SUIInputWrapper_setValidator(SUIInputWrapperObject *self, PyObject * args) {
	SERIALUI_DEBUG_OUTLN("input setValidator");
	PyObject * callback;
	if (PyArg_UnpackTuple(args, "ref", 1, 1, &callback)) {

		if (PyCallable_Check(callback)) {
			Py_INCREF(callback);
			if (self->callbacks[1]) {
				Py_DECREF(self->callbacks[1]);
			}
			self->callbacks[1] = callback;
			Py_RETURN_TRUE;
		}
    }
	Py_RETURN_FALSE;
}

static PyObject *
SUIInputWrapper_setOnChange(SUIInputWrapperObject *self, PyObject * args) {
	SERIALUI_DEBUG_OUTLN("input setOnChange");
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
	    {"isValid", (PyCFunction) SUIInputWrapper_isValid, METH_VARARGS,
				"Confirm whether the passed value is actually acceptable" },
		{"setValidator", (PyCFunction)SUIInputWrapper_setValidator,
						METH_VARARGS, "set method triggered to validate changes" },
		{ "changed", (PyCFunction) SUIInputWrapper_changed,
						METH_NOARGS, "method triggered on change" },
		{"setOnChange", (PyCFunction)SUIInputWrapper_setOnChange,
						METH_VARARGS, "set method triggered on change" },
		{ "options", (PyCFunction) SUIInputWrapper_options,
										METH_NOARGS, "options list" },
		{"setOptions", (PyCFunction)SUIInputWrapper_setOptions,
										METH_VARARGS, "set options for options list" },


		{ NULL } /* Sentinel */
};


SUIPY_DEFINE_ITEMPYTHONTYPE(SUIInputWrapper,  "SerialUI.Input", "SerialUI input object");







/*  ********************** SUI Item container wrapper ************** */
static int SUICommandWrapper_init(SUICommandWrapperObject *self, PyObject *args,
		PyObject *kwds) {
	static char *kwlist[] = { "key", "id", NULL };
	PyObject *key = NULL, *tmp;


	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|Oi", kwlist, &key,
			&self->id)) {


		PyErr_SetString(PyExc_RuntimeError, "pass key and/or id");
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

		PyErr_SetString(PyExc_RuntimeError, "no such item");
	  return -1;
  }

  self->sui_item = itm;
  self->id = itm->id();
  self->type = itm->type();

  SERIALUI_DEBUG_OUTLN("Init cmd obj for id " << (int)self->id);
  SerialUI::Python::SUIPyObjectsStore.addCommand(self->id, (PyObject*)self);

  SerialUI::Menu::Item::Command * cmd
  	  = SerialUI::Globals::menuStructure()->getCommandById(self->id);

  if (! cmd ) {

		PyErr_SetString(PyExc_RuntimeError, "no such command");
	  return -1;
  }
  self->command = cmd;

  return 0;
}


static PyObject *
SUICommandWrapper_setOnTrigger(SUICommandWrapperObject *self, PyObject * args) {
	SERIALUI_DEBUG_OUTLN("command setOnTrigger");
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
		SERIALUI_DEBUG_OUTLN("Calling trigger override");

		CPyObject argList = Py_BuildValue("(O)", self);
		PyObject * retObj = PyObject_CallObject(self->callbacks[0], argList);
		if (retObj) {
			return retObj;
		} else {
			SUIPY_CALLOBJECT_REPORTERROR();
			Py_RETURN_NONE;
		}
	} else {
		SERIALUI_DEBUG_OUTLN("Default command trigger()");
	}
	Py_RETURN_NONE;
}

static PyObject *
SUICommandWrapper_dotrigger(SUICommandWrapperObject *self, PyObject * args) {
	if (self->command) {
		SERIALUI_DEBUG_OUTLN("Actually triggering command");
		self->command->call(SerialUI::Globals::state()->currentMenu());
		Py_RETURN_TRUE;
	}

	Py_RETURN_FALSE;

}


/*
 * SUIItemContainer methods listing
 */

static PyMethodDef SUICommandWrapper_methods[] = {
		{ "triggered", (PyCFunction) SUICommandWrapper_triggered, METH_NOARGS,
				"Called when command is triggered" },
		{ "setOnTriggered", (PyCFunction) SUICommandWrapper_setOnTrigger, METH_VARARGS,
						"Override triggered() callback" },
		{ "trigger", (PyCFunction) SUICommandWrapper_dotrigger, METH_NOARGS,
				"Manually trigger this command" },
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
		SERIALUI_DEBUG_OUTLN("SUIItemContainer_init unhappy tuple/keyw parse");

		PyErr_SetString(PyExc_RuntimeError, "pass key and/or id");
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
	  SERIALUI_DEBUG_OUTLN("SUIItemContainer_init can't find itm");
		PyErr_SetString(PyExc_RuntimeError, "no such item");
	  return -1;
  }
  self->sui_item = itm;
  self->id = itm->id();
  self->type = itm->type();


  SERIALUI_DEBUG_OUTLN("Init itm container obj for id " << (int)self->id);

  SerialUI::Menu::Item::SubMenu * submen
  	  // = SerialUI::Globals::menuStructure()->getSubMenuById(self->id);
	  = SerialUI::Globals::menuStructure()->getItemContainerById(self->id);

  if (! submen ) {
	  SERIALUI_DEBUG_OUTLN("SUIItemContainer_init itm " << (int)self->id << " is not a container");


	PyErr_SetString(PyExc_RuntimeError, "no such submenu/container");
	return -1;
  }
  self->container = submen;

  return 0;
}


static PyObject *
SUIItemContainer_children(SUIItemContainerObject *self, PyObject * args) {
	uint8_t numChildren = self->container->numItems();

	SERIALUI_DEBUG_OUTLN("ItemContainer.children()");
	// PyObject ** children = new PyObject *[numChildren];
	PyObject * childList = PyList_New(0);

	if (! childList) {
		PyErr_SetString(PyExc_RuntimeError, "can't alloc new list");
		Py_RETURN_NONE;
	}

	ItemPyObjectSet objSet;
	for (uint8_t i=0; i<numChildren; i++) {
		SerialUI::Menu::Item::Item * itm = self->container->itemByIndex(i);

		PyObject *argList = Py_BuildValue("si", itm->key(), itm->id());
		PyObject * curObj = NULL;

		switch (itm->type()) {
		case SerialUI::Menu::Item::Type::Input:
			// std::cerr << "create input" << std::endl;
			objSet = SerialUI::Python::SUIPyObjectsStore.inputsFor(itm->id());
			if (objSet.size()) {
				SERIALUI_DEBUG_OUTLN("Already have an input constructed for this item, re-using that.");
				curObj = *(objSet.begin());
				Py_INCREF(curObj); // will be doing a decref, below
			} else {
				SERIALUI_DEBUG_OUTLN("Creating new SerialUI.Input for item");
				curObj = PyObject_CallObject((PyObject *) &SUIInputWrapperType, argList);
			}
			break;
		case SerialUI::Menu::Item::Type::Menu:
			/* fall-through */
		case SerialUI::Menu::Item::Type::Group:
			/* fall-through */
		case SerialUI::Menu::Item::Type::List:

			curObj = PyObject_CallObject((PyObject *) &SUIItemContainerType, argList);
			break;

		case SerialUI::Menu::Item::Type::Command:

			objSet = SerialUI::Python::SUIPyObjectsStore.commandsFor(itm->id());
			if (objSet.size()) {
				SERIALUI_DEBUG_OUTLN("Already have a command constructed for this item, re-using that.");
				curObj = *(objSet.begin());
				Py_INCREF(curObj); // will be doing a decref, below
			} else {
				SERIALUI_DEBUG_OUTLN("Creating new SerialUI.Command for item");
				curObj = PyObject_CallObject((PyObject *) &SUICommandWrapperType, argList);
			}
			break;

		default:
			curObj = NULL;
			break;



		}

		if (curObj) {
			PyList_Append(childList, curObj);
			Py_DECREF(curObj); // list now owns the obj

		}
		SUIPY_CALLOBJECT_REPORTERROR();

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






















#if 0
typedef struct {
	PyObject_HEAD
	SerialUI::Auth::Storage * auth_storage;
	uint8_t id;
} SUIAuthStorageObject;
static PyObject * SUIAuthStorage_new(PyTypeObject *type,
	PyObject *args, PyObject *kwds) {
		static uint8_t storage_idx=0;
		SUIAuthStorageObject *self;
		self = (SUIAuthStorageObject *) type->tp_alloc(type, 0);
		if (!self) {
			return NULL;
		}
		self->auth_storage = NULL;
		self->id = storage_idx++;
		SERIALUI_DEBUG_OUTLN("Creating authstore " << (int)self->id);

		return (PyObject *) self;
}

static PyMemberDef SUIAuthStorage_members[] = {
		{ "id", T_INT, offsetof(SUIAuthStorageObject, id), 0, "internal id" },
		{ NULL }
};
static void SUIAuthStorage_dealloc(SUIAuthStorageObject *self) { \
					SERIALUI_DEBUG_OUTLN("DESTROYING authstore " << (int)self->id);
					uint8_t id = self->id;
					SerialUI::Python::SUIPyObjectsStore.setAuthStorage(NULL);
					// SerialUI::Python::SUIPyObjectsStore.removeAuthObject((PyObject *) self);
					Py_TYPE(self)->tp_free((PyObject *) self);
}
static int SUIAuthStorage_init(SUIAuthStorageObject *self, PyObject *args,
		PyObject *kwds) {

	SERIALUI_DEBUG_OUTLN("init authstore " << (int)self->id);
	if (SerialUI::Python::SUIPyObjectsStore.authStorage()) {
		// forcing singleton
		SERIALUI_DEBUG_OUTLN("already have this setup... fail");
		return -1;
	}

	self->auth_storage = new SerialUI::Auth::StoragePython();
	if (! self->auth_storage) {
		SERIALUI_DEBUG_OUTLN("could not allocate Auth::StoragePython... fail");
		return -1;
	}

	SerialUI::Python::SUIPyObjectsStore.setAuthStorage((PyObject*)self);

	SERIALUI_DEBUG_OUTLN("authstore init success");
	return 0;

}
static PyObject* SUIAuthStorage_SUIRepr(PyObject*o) { \
	SUIAuthStorageObject * myo = (SUIAuthStorageObject *)o;
	std::ostringstream s;
	s << "<SerialUI.AuthStorage id=" << (int)myo->id;
	s << ">";
	return PyUnicode_FromString(s.str().c_str());
}

/* storage member methods */
static PyObject * SUIAuthStorage_configured(SUIAuthStorageObject *self, PyObject * args) {
	SERIALUI_DEBUG_OUTLN("authstore configured() default -- ret false");
	Py_RETURN_FALSE;
}
static PyObject * SUIAuthStorage_passphrase(SUIAuthStorageObject *self, PyObject * args) {
	SERIALUI_DEBUG_OUTLN("authstore passphrase() default -- ret '' ");
	return PyUnicode_FromString("");

}
static PyObject * SUIAuthStorage_setPassphrase(SUIAuthStorageObject *self, PyObject * args) {
	SERIALUI_DEBUG_OUTLN("authstore setPassphrase() default -- ret false");
	Py_RETURN_FALSE;
}
static PyMethodDef SUIAuthStorage_methods[] = {
		{ "configured", (PyCFunction) SUIAuthStorage_configured, METH_VARARGS,
			"Return whether provided level is configured" },
		{ "passphrase", (PyCFunction) SUIAuthStorage_passphrase, METH_VARARGS,
					"Return (possibly encoded) passphrase value for given level" },
		{ "setPassphrase", (PyCFunction) SUIAuthStorage_setPassphrase, METH_VARARGS,
								"Store given passphrase for access level" },
		{ NULL } /* Sentinel */
};


SUIPY_DEFINE_ITEMPYTHONTYPE(SUIAuthStorage, "SerialUI.AuthStorage", "SerialUI Authentication Storage");

#endif

/*

basename, basetype, storeGetter, storeSetter
SUIAuthStorage, SerialUI::Auth::Storage
SerialUI::Python::SUIPyObjectsStore.authStorage
SerialUI::Python::SUIPyObjectsStore.setAuthStorage
*/
#define SUI_DEFINE_AUTHOBJ(basename, basetype, storeGetter, storeSetter) \
typedef struct { \
	PyObject_HEAD \
	basetype * auth_obj; \
	uint8_t id; \
} basename##Object; \
static PyObject * basename##_new(PyTypeObject *type, \
	PyObject *args, PyObject *kwds) { \
		static uint8_t obj_idx=0; \
		basename##Object *self; \
		self = (basename##Object *) type->tp_alloc(type, 0); \
		if (!self) { \
			return NULL; \
		} \
		self->auth_obj = NULL; \
		self->id = obj_idx++; \
		SERIALUI_DEBUG_OUTLN("Creating auth obj " << (int)self->id); \
		return (PyObject *) self; \
} \
static PyMemberDef basename##_members[] = { \
		{ "id", T_INT, offsetof(basename##Object, id), 0, "internal id" }, \
		{ NULL } \
}; \
static void basename##_dealloc(basename##Object *self) { \
					SERIALUI_DEBUG_OUTLN("DESTROYING auth obj " << (int)self->id); \
					uint8_t id = self->id; \
					if (self->auth_obj) { \
						delete self->auth_obj; \
					}\
					storeSetter(NULL); \
					Py_TYPE(self)->tp_free((PyObject *) self); \
}


SUI_DEFINE_AUTHOBJ(SUIAuthStorage, SerialUI::Auth::Storage,
				SerialUI::Python::SUIPyObjectsStore.authStorage,
				SerialUI::Python::SUIPyObjectsStore.setAuthStorage);
static int SUIAuthStorage_init(SUIAuthStorageObject *self, PyObject *args,
		PyObject *kwds) {

	SERIALUI_DEBUG_OUTLN("init auth obj " << (int)self->id);
	if (SerialUI::Python::SUIPyObjectsStore.authStorage()) {
		// forcing singleton
		SERIALUI_DEBUG_OUTLN("already have this setup... fail");

		PyErr_SetString(PyExc_RuntimeError, "only supports 1 auth storage obj at a time");
		return -1;
	}

	self->auth_obj = new SerialUI::Auth::StoragePython();
	if (! self->auth_obj) {
		SERIALUI_DEBUG_OUTLN("could not allocate Auth::StoragePython... fail");

		PyErr_SetString(PyExc_RuntimeError, "failed to allocate");
		return -1;
	}

	SerialUI::Python::SUIPyObjectsStore.setAuthStorage((PyObject*)self);

	SERIALUI_DEBUG_OUTLN("authstore init success");
	return 0;

}
static PyObject* SUIAuthStorage_SUIRepr(PyObject*o) { \
	SUIAuthStorageObject * myo = (SUIAuthStorageObject *)o;
	std::ostringstream s;
	s << "<SerialUI.AuthStorage id=" << (int)myo->id;
	s << ">";
	return PyUnicode_FromString(s.str().c_str());
}

/* storage member methods */
static PyObject * SUIAuthStorage_configured(SUIAuthStorageObject *self, PyObject * args) {
	SERIALUI_DEBUG_OUTLN("authstore configured() default -- ret false");
	Py_RETURN_TRUE;
}
static PyObject * SUIAuthStorage_passphrase(SUIAuthStorageObject *self, PyObject * args) {
	SERIALUI_DEBUG_OUTLN("authstore passphrase() default -- ret 'notset...' ");
	return PyUnicode_FromString("notset please override!");

}
static PyObject * SUIAuthStorage_setPassphrase(SUIAuthStorageObject *self, PyObject * args) {
	SERIALUI_DEBUG_OUTLN("authstore setPassphrase() default -- ret false");
	Py_RETURN_FALSE;
}
static PyMethodDef SUIAuthStorage_methods[] = {
		{ "configured", (PyCFunction) SUIAuthStorage_configured, METH_VARARGS,
			"Return whether provided level is configured" },
		{ "passphrase", (PyCFunction) SUIAuthStorage_passphrase, METH_VARARGS,
					"Return (possibly encoded) passphrase value for given level" },
		{ "setPassphrase", (PyCFunction) SUIAuthStorage_setPassphrase, METH_VARARGS,
								"Store given passphrase for access level" },
		{ NULL } /* Sentinel */
};


SUIPY_DEFINE_ITEMPYTHONTYPE(SUIAuthStorage, "SerialUI.AuthStorage", "SerialUI Authentication Storage");














SUI_DEFINE_AUTHOBJ(SUIAuthValidator, SerialUI::Auth::Validator,
				SerialUI::Python::SUIPyObjectsStore.authValidator,
				SerialUI::Python::SUIPyObjectsStore.setAuthValidator);
static int SUIAuthValidator_init(SUIAuthValidatorObject *self, PyObject *args,
		PyObject *kwds) {

	SERIALUI_DEBUG_OUTLN("init auth obj " << (int)self->id);
	if (SerialUI::Python::SUIPyObjectsStore.authValidator()) {
		// forcing singleton
		SERIALUI_DEBUG_OUTLN("already have this setup... fail");

		PyErr_SetString(PyExc_RuntimeError, "only supports 1 auth validator at a time");
		return -1;
	}

	SUIAuthStorageObject * storageObj = NULL;
	PyObject * authStore;
	if (!PyArg_ParseTuple(args, "O", &authStore)) {
		SERIALUI_DEBUG_OUTLN("unhappy tuple/keyw parse");

		PyErr_SetString(PyExc_RuntimeError, "pass auth storage object to constructor");
		return -1;
	}

	if ((
			(authStore->ob_type == &SUIAuthStorageType)
			||
			(PyType_IsSubtype(authStore->ob_type, &SUIAuthStorageType)

	))) {
		storageObj = (SUIAuthStorageObject *)authStore;
	} else {
		SERIALUI_DEBUG_OUTLN("Not a subtype of auth store??");

		PyErr_SetString(PyExc_RuntimeError, "invalid type: pass AuthStore derivative to validator");
		return -1;
		// storageObj = (SUIAuthStorageObject *)SerialUI::Python::SUIPyObjectsStore.authStorage();

	}


	if (! storageObj) {

		SERIALUI_DEBUG_OUTLN("must have a corresponding auth storage first... fail");

		PyErr_SetString(PyExc_RuntimeError, "no storage object passed/found");
		return -1;
	}


	self->auth_obj = new SerialUI::Auth::ValidatorPython(storageObj->auth_obj);
	if (! self->auth_obj) {
		SERIALUI_DEBUG_OUTLN("could not allocate Auth::StoragePython... fail");

		PyErr_SetString(PyExc_RuntimeError, "could not allocate");
		return -1;
	}

	SerialUI::Python::SUIPyObjectsStore.setAuthValidator((PyObject*)self);

	SERIALUI_DEBUG_OUTLN("authvalidator init success");
	return 0;

}
static PyObject* SUIAuthValidator_SUIRepr(PyObject*o) { \
	SUIAuthValidatorObject * myo = (SUIAuthValidatorObject *)o;
	std::ostringstream s;
	s << "<SerialUI.AuthValidator id=" << (int)myo->id;
	s << ">";
	return PyUnicode_FromString(s.str().c_str());
}

static PyObject *
SUIAuthValidator_communicationType(SUIAuthValidatorObject *self, PyObject * args) {

	SERIALUI_DEBUG_OUTLN("calling extpy built-in authvalidator.communicationType(), no override");

	return PyLong_FromLong((long)SerialUI::Auth::Transmission::Type::Plain);

}

static PyObject *
SUIAuthValidator_challenge(SUIAuthValidatorObject *self, PyObject * args) {
	SERIALUI_DEBUG_OUTLN("calling extpy built-in authvalidator.challenge(), no override");
	Py_RETURN_NONE;

}
static PyObject *
SUIAuthValidator_grantAccess(SUIAuthValidatorObject *self, PyObject * args) {
	SERIALUI_DEBUG_OUTLN("calling extpy built-in authvalidator.grantAccess(), should override");
	return PyLong_FromLong((long)SerialUI::Auth::Level::NoAccess);

}
static PyMethodDef SUIAuthValidator_methods[] = {
		{ "communicationType",
				(PyCFunction) SUIAuthValidator_communicationType, METH_NOARGS,
				"passphrase comm type" },
		{ "challenge",
				(PyCFunction) SUIAuthValidator_challenge, METH_VARARGS,
				"challenge for level" },
		{ "grantAccess",
				(PyCFunction) SUIAuthValidator_grantAccess, METH_VARARGS,
				"level granted for response" },
		{ NULL } /* Sentinel */
};



SUIPY_DEFINE_ITEMPYTHONTYPE(SUIAuthValidator, "SerialUI.AuthValidator", "SerialUI Authentication Validator");




















/*  ********************** SUI Item container wrapper ************** */
static int SUITrackedState_init(SUITrackedStateObject *self, PyObject *args,
		PyObject *kwds) {
	static char *kwlist[] = { "key", "id", NULL };
	PyObject *key = NULL, *tmp;

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|Oi", kwlist, &key,
			&self->id)) {
		SERIALUI_DEBUG_OUTLN("SUIItemContainer_init unhappy tuple/keyw parse");
		PyErr_SetString(PyExc_RuntimeError, "Pass key and/or id");
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
				SERIALUI_DEBUG_OUTLN("can't get key str?");
				PyErr_SetString(PyExc_RuntimeError, "can't get key string?");
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

		SERIALUI_DEBUG_OUTLN("Can't find Tracked state for id/key?");
		PyErr_SetString(PyExc_RuntimeError, "can't find matching tracked state");
		return -1;
	}

	self->state_obj = self->sui_item;
	self->id = self->sui_item->id();
	self->type = self->state_obj->type();
	self->valuetype = (uint8_t)self->type;

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
			SERIALUI_DEBUG_OUTLN("unhappy tuple/keyw parse");
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

	SERIALUI_DEBUG_OUT("tree() ");
	if (myTree) {
		SERIALUI_DEBUG_OUTLN("return cached.");
		Py_INCREF(myTree); // caller will want to cleanup
		return myTree;
	}
	SERIALUI_DEBUG_OUTLN("building...");
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

static void pysui_ensureItemTreeIsCached(PyObject* self, PyObject* args) {
	CPyObject itemTree = pysui_tree(self, args);

}

static PyObject* pysui_foreachItemUnder(PyObject* self, PyObject* args) {

	PyObject * containerOfInterest;
	PyObject * callback;
	SERIALUI_DEBUG_OUTLN("foreachItemUnder...");
	if (!PyArg_ParseTuple(args, "OO", &containerOfInterest, &callback)) {
		SERIALUI_DEBUG_OUTLN("foreachItemUnder: unhappy tuple/keyw parse");
		Py_RETURN_FALSE;
	}

	if (containerOfInterest->ob_type != &SUIItemContainerType) {

		PyErr_SetString(PyExc_RuntimeError, "pass a SerialUI.ItemContainer or derivative to search in as first arg");

		Py_RETURN_FALSE;
	}

	if (! PyCallable_Check(callback) ) {

		PyErr_SetString(PyExc_RuntimeError, "pass a callback as second arg");

		Py_RETURN_FALSE;
	}

	/*
	SERIALUI_DEBUG_OUTLN("calling crawler callback for this container");

	CPyObject cbContArgs = Py_BuildValue("(O)", containerOfInterest);
	CPyObject shouldContinue = PyObject_CallObject(callback, cbContArgs);

	SERIALUI_DEBUG_OUTLN("checking if should continue");
	if (! (shouldContinue && PyObject_IsTrue(shouldContinue)) ) {
		SERIALUI_DEBUG_OUTLN("NOT TRUE--no continue");
		Py_RETURN_TRUE;
	}
	*/

	// since we're looping over the elements, the caller may want to
	// modify the items, say by setting a callback, so we need to ensure
	// we persist them
	pysui_ensureItemTreeIsCached(self, args);

	SERIALUI_DEBUG_OUTLN("Getting this container's children");

	SUIItemContainerObject * container = (SUIItemContainerObject *)containerOfInterest;

	CPyObject containerChildrenList = SUIItemContainer_children(container, args);
	if (! containerChildrenList) {

		PyErr_SetString(PyExc_RuntimeError, "could not get container children?");

		Py_RETURN_FALSE;
	}

	for (Py_ssize_t i = 0; i < PyList_Size(containerChildrenList); i++) {
		PyObject * aChildObj = PyList_GetItem(containerChildrenList, i); // PyList_GetItem does NOT INCREF
		CPyObject retVal;

		CPyObject cbItemArgs = Py_BuildValue("(O)", aChildObj);

		if (aChildObj->ob_type == &SUIItemContainerType) {
			SERIALUI_DEBUG_OUT("is container...");
			retVal = PyObject_CallObject(callback, cbItemArgs);
			if (retVal && PyObject_IsTrue(retVal)) {
				SERIALUI_DEBUG_OUT("will recurse");
				CPyObject argList = Py_BuildValue("(OO)", aChildObj ,  callback);
				retVal = pysui_foreachItemUnder(self, argList);
			} else {
				SERIALUI_DEBUG_OUT("no recursion");
			}

		} else {

			SERIALUI_DEBUG_OUTLN("is menu item (command/input), calling callback");
			retVal = PyObject_CallObject(callback, cbItemArgs);
			if (! (retVal && PyObject_IsTrue(retVal))) {
				Py_RETURN_FALSE;
			}
		}

	}
	Py_RETURN_TRUE;
}

static PyObject* pysui_foreachItem(PyObject* self, PyObject* args) {

	PyObject * callback;
	if (!PyArg_ParseTuple(args, "O", &callback)) {
		SERIALUI_DEBUG_OUTLN("foreachItem: unhappy tuple/keyw parse");
		Py_RETURN_FALSE;
	}


	if (! PyCallable_Check(callback) ) {
		PyErr_SetString(PyExc_RuntimeError, "pass a callback as arg");
		Py_RETURN_FALSE;
	}

	CPyObject topMen = pysui_top(self, args);
	if (! topMen) {
		PyErr_SetString(PyExc_RuntimeError, "could not get top menu??");
		Py_RETURN_FALSE;
	}

	CPyObject argList = Py_BuildValue("(OO)", topMen.getObject(),  callback);
	return pysui_foreachItemUnder(self, argList);

}

static PyObject* pysui_trackers(PyObject* self, PyObject* args) {
	static PyObject * myTrackers = NULL;

	if (myTrackers) {
		SERIALUI_DEBUG_OUTLN("TRACKERS: returning cached");

		Py_INCREF(myTrackers); // caller will want to cleanup
		return myTrackers;
	}
	SERIALUI_DEBUG_OUTLN("TRACKERS: building new");
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
		} else {
			SUIPY_CALLOBJECT_REPORTERROR();
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
		SERIALUI_DEBUG_OUTLN("unhappy tuple/keyw parse");
		Py_RETURN_FALSE;
	}
	if (SerialUI::Globals::pythonModule()->setHearbeatPeriod(val)) {
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}


static PyObject* pysui_setAuthValidator(PyObject* self, PyObject* args) {

	static SerialUI::Auth::Authenticator * py_authenticator = NULL;
	PyObject * obj = NULL;
	if (!PyArg_ParseTuple(args, "O", &obj)) {
		SERIALUI_DEBUG_OUTLN("unhappy tuple/keyw parse");
		Py_RETURN_FALSE;
	}

	if (obj == Py_None) {
		// clearing out.

		SerialUI::Globals::setAuthenticator(NULL);

		if (py_authenticator) {
			delete py_authenticator;
			py_authenticator = NULL;
		}

		Py_RETURN_TRUE;

	}

	if (!
		  (
		    (obj->ob_type == &SUIAuthValidatorType)
			||
			(PyType_IsSubtype(obj->ob_type, &SUIAuthValidatorType))
			)
		) {
		SERIALUI_DEBUG_OUTLN("invalid type passed");

		PyErr_SetString(PyExc_RuntimeError, "pass a SerialUI.AuthValidator derivative");

		Py_RETURN_FALSE;
	}
	SUIAuthValidatorObject * authValidator = (SUIAuthValidatorObject*)obj;

	py_authenticator = new SerialUI::Auth::Authenticator(authValidator->auth_obj);

	SerialUI::Globals::setAuthenticator(py_authenticator);

	Py_RETURN_TRUE;

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
  // { "test", pysui_test, METH_VARARGS, "test me" },
  { "top", pysui_top, METH_NOARGS, "top level menu"},
  { "tree", pysui_tree, METH_NOARGS, "full menu tree"},
  { "trackers", pysui_trackers, METH_NOARGS, "list of state trackers"},
  { "flush", pysui_flush, METH_NOARGS, "flush prints/updates"},
  { "setAuthValidator", pysui_setAuthValidator, METH_VARARGS, "set validator to use for auth"},
  { "setHeartbeatPeriod", pysui_setHeartbeatPeriod, METH_VARARGS, "set heartbeat period (ms)"},
  { "foreachItem", pysui_foreachItem, METH_VARARGS, "call a method for each item in menu tree"},
  { "foreachItemUnder", pysui_foreachItemUnder, METH_VARARGS, "call a method for each item within container"},
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


#define PYSUI_READYTYPEORFAIL(tp) \
		if (PyType_Ready(&(tp)) < 0) { return NULL; }

#define PYSUI_ADDTYPE(tp, name, tomodule) \
		Py_INCREF(&(tp));\
		PyModule_AddObject(tomodule, name, (PyObject *) &(tp))

PyObject* PyInit_SerialUIModule(void) {
	PyObject *mymodule;


	PYSUI_READYTYPEORFAIL(SUIInputWrapperType);
	PYSUI_READYTYPEORFAIL(SUIItemContainerType);
	PYSUI_READYTYPEORFAIL(SUICommandWrapperType);
	PYSUI_READYTYPEORFAIL(SUITrackedStateType);
	PYSUI_READYTYPEORFAIL(SUIAuthStorageType);
	PYSUI_READYTYPEORFAIL(SUIAuthValidatorType);

	mymodule = PyModule_Create(&modDef);
	if (mymodule == NULL) {
		return NULL;
	}

	PYSUI_ADDTYPE(SUIInputWrapperType, "Input", mymodule);
	PYSUI_ADDTYPE(SUIItemContainerType, "ItemContainer", mymodule);
	PYSUI_ADDTYPE(SUICommandWrapperType, "Command", mymodule);
	PYSUI_ADDTYPE(SUITrackedStateType, "TrackedState", mymodule);
	PYSUI_ADDTYPE(SUIAuthStorageType, "AuthStorage", mymodule);
	PYSUI_ADDTYPE(SUIAuthValidatorType, "AuthValidator", mymodule);
	// PYSUI_ADDTYPE(, "", mymodule);

	/*
	Py_INCREF(&SUIInputWrapperType);
	Py_INCREF(&SUIItemContainerType);
	Py_INCREF(&SUICommandWrapperType);
	Py_INCREF(&SUITrackedStateType);
	Py_INCREF(&SUIAuthStorageType);


	PyModule_AddObject(mymodule, "Input", (PyObject *) &SUIInputWrapperType);
	PyModule_AddObject(mymodule, "ItemContainer", (PyObject *) &SUIItemContainerType);
	PyModule_AddObject(mymodule, "Command", (PyObject *) &SUICommandWrapperType);
	PyModule_AddObject(mymodule, "TrackedState", (PyObject *) &SUITrackedStateType);
	PyModule_AddObject(mymodule, "AuthStorage", (PyObject *) &SUIAuthStorageType);
	*/

	return mymodule;
}





#endif /* SERIALUI_PYTHONMODULES_SUPPORT_ENABLE */



