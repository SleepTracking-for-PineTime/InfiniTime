#pragma once

#include "components/sleep/SleepTracker.h"
#include "displayapp/apps/Apps.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/Controllers.h"
#include "systemtask/SystemTask.h"
#include "Symbols.h"
#include <lvgl/src/lv_core/lv_obj.h>

namespace Pinetime {
  namespace Controllers {
    class SleepController;
  }

  namespace Applications {
    namespace Screens {

        class InfiniSleep : public Screen {
        public:
          InfiniSleep(Controllers::SleepController& sleepController);
          ~InfiniSleep();

          void ToggleTracking();
          void SleepStateUpdated(SleepTracker::SleepTracker::SleepState state);
        
        private:
          void Refresh();
          void SleepTrackerStarted();
          void SleepTrackerStopped();

          Controllers::SleepController& sleepController;

          lv_obj_t* btn_transferData;

          lv_obj_t* label_btnStartStop;
          lv_obj_t* label_heartRate;
          lv_obj_t* label_motionX;
          lv_obj_t* label_motionY;
          lv_obj_t* label_motionZ;
          lv_obj_t* label_memory;
          lv_obj_t* label_state;

          lv_task_t* taskRefresh;

          bool tracking_started = false;
        };
    }

    template <>
    struct AppTraits<Apps::InfiniSleep> {
      static constexpr Apps app = Apps::InfiniSleep;
      static constexpr const char* icon = Screens::Symbols::bed;

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::InfiniSleep(controllers.sleepController);
      };
    };
  }
}
