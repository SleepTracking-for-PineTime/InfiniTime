#pragma once

#include "displayapp/apps/Apps.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/Controllers.h"
#include "systemtask/SystemTask.h"
#include "Symbols.h"
#include <lvgl/src/lv_core/lv_obj.h>

namespace Pinetime {
  namespace Controllers {
    class HeartRateController;
    class MotionController;
  }

  namespace Applications {
    namespace Screens {

        class InfiniSleep : public Screen {
        public:
          InfiniSleep(
            Controllers::HeartRateController& heartRateController,
            Controllers::MotionController& motionController,
            System::SystemTask& systemTask
          );
          ~InfiniSleep();

          void StartTracking();
        
        private:
          Controllers::HeartRateController& heartRateController;
          Controllers::MotionController& motionController;
          Pinetime::System::SystemTask& systemTask;

          lv_obj_t* btn_transferData;
        };
    }

    template <>
    struct AppTraits<Apps::InfiniSleep> {
      static constexpr Apps app = Apps::InfiniSleep;
      static constexpr const char* icon = Screens::Symbols::bed;

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::InfiniSleep(controllers.heartRateController, controllers.motionController, *controllers.systemTask);
      };
    };
  }
}
