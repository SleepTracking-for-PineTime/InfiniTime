#pragma once

#include "displayapp/apps/Apps.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/Controllers.h"
#include "systemtask/SystemTask.h"
#include "Symbols.h"
#include <lvgl/src/lv_core/lv_obj.h>
#include <memory>
#include <vector>

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

          void Refresh();
          void StartTracking();

          struct ActivityPacket {
            uint8_t heart_rate;
            int16_t motion_x;
            int16_t motion_y;
            int16_t motion_z;
          };
        
        private:
          Controllers::HeartRateController& heartRateController;
          Controllers::MotionController& motionController;
          Pinetime::System::SystemTask& systemTask;

          lv_obj_t* btn_transferData;

          lv_obj_t* label_heartRate;
          lv_obj_t* label_motionX;
          lv_obj_t* label_motionY;
          lv_obj_t* label_motionZ;
          lv_obj_t* label_memory;

          lv_task_t* taskRefresh;

          bool tracking_started = false;
          std::vector<std::unique_ptr<ActivityPacket> > activityPackets;
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
