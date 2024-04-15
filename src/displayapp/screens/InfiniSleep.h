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
    class SleepActivityService;
    class HeartRateController;
    class MotionController;
  }

  namespace Applications {
    namespace Screens {

        class InfiniSleep : public Screen {
        public:
          InfiniSleep(
            Controllers::SleepActivityService& sleepActivityService,
            Controllers::HeartRateController& heartRateController,
            Controllers::MotionController& motionController,
            System::SystemTask& systemTask
          );
          ~InfiniSleep();

          void StartTracking();
          void SleepStateUpdated(SleepTracker::SleepTracker::SleepState state);
        
        private:
          void Refresh();

          Controllers::SleepActivityService& sleepActivityService;
          Controllers::HeartRateController& heartRateController;
          Controllers::MotionController& motionController;
          Pinetime::System::SystemTask& systemTask;
          SleepTracker::VanHeesSleepTracker sleepTracker;

          lv_obj_t* btn_transferData;

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
        return new Screens::InfiniSleep(*controllers.sleepActivityService, controllers.heartRateController, controllers.motionController, *controllers.systemTask);
      };
    };
  }
}
