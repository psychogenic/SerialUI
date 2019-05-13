/*
 * ExternalPython.h
 *
 *  Created on: May 11, 2019
 *      Author: malcalypse
 */

#ifndef SERIALUI_SRC_INCLUDES_PYTHON_EXTERNALPYTHON_H_
#define SERIALUI_SRC_INCLUDES_PYTHON_EXTERNALPYTHON_H_

#include "ExternalModule.h"


#ifdef SERIALUI_PYTHONMODULES_SUPPORT_ENABLE

#include <map>
#include <set>

typedef std::set<PyObject*>	ItemPyObjectSet;
typedef std::map<uint8_t, ItemPyObjectSet> ItemPyObjectById;

namespace SerialUI {
namespace Python {

class ObjectsStore {

public:
	ObjectsStore() {}

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

	void deleteEntryFor(uint8_t forId, PyObject * obj);
private:

	void add(uint8_t forId, PyObject * obj, ItemPyObjectById & toMap);
	ItemPyObjectSet setFor(uint8_t forId, ItemPyObjectById & inMap);

	ItemPyObjectById inputs;
	ItemPyObjectById commands;
};

extern ObjectsStore SUIPyObjectsStore;

}
}

#endif /* SERIALUI_PYTHONMODULES_SUPPORT_ENABLE */

#endif /* SERIALUI_SRC_INCLUDES_PYTHON_EXTERNALPYTHON_H_ */
