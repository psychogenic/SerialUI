#ifndef SUI_PYTHON_EXTERNAL_MODULE
#define SUI_PYTHON_EXTERNAL_MODULE

#include "../SerialUIPlatform.h"

#ifdef SERIALUI_PYTHONMODULES_SUPPORT_ENABLE


#include "../menuitem/items.h"
#include "../tracked/tracked.h"
#include "pyhelper.hpp"


// class ::SerialUI::SerialUI; // forward decl


namespace SerialUI {

namespace Python {
class ExternalModule {

public:
  // ExternalModule(SerialUI * driver, DynamicString name);
  ExternalModule(void * serialUIDriver,
		  DynamicString name,
		  DynamicString path=NULL);
  ~ExternalModule();



  bool callHandlerMethod(DynamicString name);







  bool load();




  bool trigger(Menu::Item::Command * cmd);
  bool trigger(Menu::Item::Request::Request * req);


  bool triggerHeartbeat();
  bool setHearbeatPeriod(unsigned long ms);


  void updated(Menu::Item::Request::Request * req);
  void updated(Tracked::State* st);


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
  //SerialUI * driver;
  void * driver;
};


}
}

#endif /* SERIALUI_PYTHONMODULES_SUPPORT_ENABLE */

#endif /* SUI_PYTHON_EXTERNAL_MODULE */
