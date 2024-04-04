#include "InfiniSleep.h"

#include <components/heartrate/HeartRateController.h>
#include <components/motion/MotionController.h>

using namespace Pinetime::Applications::Screens;

namespace {
  void btnTransferDataEventHandler(lv_obj_t* obj, lv_event_t event) {
      auto* screen = static_cast<InfiniSleep*>(obj->user_data);

      if (event == LV_EVENT_PRESSED) {
          screen->StartTracking();
      }
  }
}

InfiniSleep::InfiniSleep(
  Controllers::HeartRateController& heartRateController,
  Controllers::MotionController& motionController,
  System::SystemTask& systemTask)
  : heartRateController {heartRateController}, motionController {motionController}, systemTask {systemTask} {
  btn_transferData = lv_btn_create(lv_scr_act(), nullptr);
  lv_obj_set_height(btn_transferData, 100);
  lv_obj_align(btn_transferData, nullptr, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_event_cb(btn_transferData, btnTransferDataEventHandler);
}

InfiniSleep::~InfiniSleep() {
  lv_obj_clean(lv_scr_act());
}

void InfiniSleep::StartTracking() {
}
