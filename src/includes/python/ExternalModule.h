#ifndef SUI_PYTHON_EXTERNAL_MODULE
#define SUI_PYTHON_EXTERNAL_MODULE

#include "../SerialUIPlatform.h"

#ifdef SERIALUI_PYTHONMODULES_SUPPORT_ENABLE

#include "../SerialUITypes.h"
#include "../menuitem/items.h"
#include "../tracked/tracked.h"
#include "pyhelper.hpp"


// class ::SerialUI::SerialUI; // forward decl

#ifdef SERIALUI_AUTHENTICATOR_ENABLE
#include "../auth/AuthValidator.h"
#endif

#define SUI_EXTMODULE_VERSION_MAJOR	1
#define SUI_EXTMODULE_VERSION_MINOR	2
#define SUI_EXTMODULE_VERSION_PATCH	0



namespace SerialUI {
namespace Python {


typedef struct ExtModVersionStruct {
	uint8_t major;
	uint8_t minor;
	uint8_t patch;
	ExtModVersionStruct() :
		major(SUI_EXTMODULE_VERSION_MAJOR),
		minor(SUI_EXTMODULE_VERSION_MINOR),
		patch(SUI_EXTMODULE_VERSION_PATCH)
	{

	}
} ExternalModuleVersion;

class ExternalModule {
public:

  static ExternalModuleVersion version() {
	  return ExternalModuleVersion();
  }

  // ExternalModule(SerialUI * driver, DynamicString name);
  ExternalModule(void * serialUIDriver,
		  DynamicString name,
		  DynamicString path=NULL);
  ~ExternalModule();

  bool callHandlerMethod(DynamicString name);
  bool load();


  bool trigger(Menu::Item::Command * cmd);
  bool trigger(Menu::Item::Request::Request * req);

  bool isValidTrigger(Menu::Item::Request::Request * req, unsigned long val);
  bool isValidTrigger(Menu::Item::Request::Request * req, long val);
  bool isValidTrigger(Menu::Item::Request::Request * req, float val);
  bool isValidTrigger(Menu::Item::Request::Request * req, bool val);
  bool isValidTrigger(Menu::Item::Request::Request * req, TopLevelString & val);
  /*
  template<class VALTYPE>
  bool isValidTrigger(Menu::Item::Request::Request * req, VALTYPE val) {
	SERIALUI_DEBUG_OUT(F("ExternalModule::isValidTrigger() "));
	if (!load()) {
		SERIALUI_DEBUG_OUTLN(F("could not load"));
		return true; // won't deny
	}
	SERIALUI_DEBUG_OUT(" for:");
	SERIALUI_DEBUG_OUTLN(req->key());
	return SUIPyObjectsStore.callValidatorOnInputs(req->id(), val);
  }
  */

  bool triggerHeartbeat();
  bool setHearbeatPeriod(unsigned long ms);


  void updated(Menu::Item::Request::Request * req);
  void updated(Tracked::State* st);

#ifdef SERIALUI_AUTHENTICATOR_ENABLE
  Auth::Validator * authValidator();
  Auth::Storage * authStorage() ;
private:
  Auth::Validator * auth_validator;
  Auth::Storage * auth_storage;
#endif


private:
  DynamicString module_name;
  DynamicString module_path;
  DynamicString post_load_call;
  bool is_loaded;
  bool load_attempted;
  CPyObject  pModule;
  CPyObject  pHandler;
  CPyObject  pHeartbeat;
  CPyInstance * hInstance;
  void * driver;
  uint8_t reserved[64]; // space to use up for additions, to maintain binary compat of drivers
};



}
}

#endif /* SERIALUI_PYTHONMODULES_SUPPORT_ENABLE */

#endif /* SUI_PYTHON_EXTERNAL_MODULE */
