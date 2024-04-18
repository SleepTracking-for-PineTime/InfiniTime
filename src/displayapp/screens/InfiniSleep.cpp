#include "InfiniSleep.h"

#include <components/sleep/SleepController.h>
#include <FreeRTOS.h>

using namespace Pinetime::Applications::Screens;

namespace {
  void btnTransferDataEventHandler(lv_obj_t* obj, lv_event_t event) {
      auto* screen = static_cast<InfiniSleep*>(obj->user_data);

      if (event == LV_EVENT_CLICKED) {
        screen->ToggleTracking();
      }
  }
}

InfiniSleep::InfiniSleep(Controllers::SleepController& sleepController)
  : sleepController {sleepController} {
  btn_transferData = lv_btn_create(lv_scr_act(), nullptr);
  btn_transferData->user_data = this;
  lv_obj_set_height(btn_transferData, 100);
  lv_obj_align(btn_transferData, nullptr, LV_ALIGN_IN_TOP_MID, 0, 10);
  lv_obj_set_event_cb(btn_transferData, btnTransferDataEventHandler);

  label_btnStartStop = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_align_mid(label_btnStartStop, btn_transferData, LV_ALIGN_CENTER, 0, 0);

  label_heartRate = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_fmt(label_heartRate, "Heartrate: ---");
  lv_obj_align_mid(label_heartRate, nullptr, LV_ALIGN_CENTER, 0, 20);

  label_motionX = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_fmt(label_motionX, "Motion X: ---");
  lv_obj_align_mid(label_motionX, label_heartRate, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);

  label_motionY = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_fmt(label_motionY, "Motion Y: ---");
  lv_obj_align_mid(label_motionY, label_heartRate, LV_ALIGN_OUT_BOTTOM_MID, 0, 50);

  label_motionZ = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_fmt(label_motionZ, "Motion Z: ---");
  lv_obj_align_mid(label_motionZ, label_heartRate, LV_ALIGN_OUT_BOTTOM_MID, 0, 70);

  label_memory = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_fmt(label_memory, "Free Memory: %d", xPortGetFreeHeapSize());
  lv_obj_align_mid(label_memory, btn_transferData, LV_ALIGN_OUT_TOP_MID, 0, 0);

  label_state = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_fmt(label_state, "Sleep State: ---");
  lv_obj_align_mid(label_state, btn_transferData, LV_ALIGN_OUT_TOP_MID, 0, 20);

  taskRefresh = lv_task_create(RefreshTaskCallback, 100, LV_TASK_PRIO_MID, this);

  if (sleepController.GetState() == Controllers::SleepController::States::Running) {
    SleepTrackerStarted();
  } else {
    SleepTrackerStopped();
  }
}

InfiniSleep::~InfiniSleep() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
}

void InfiniSleep::Refresh() {
  lv_label_set_text_fmt(label_memory, "Free Memory: %d", xPortGetFreeHeapSize());
}

void InfiniSleep::SleepStateUpdated(SleepTracker::SleepTracker::SleepState state) {
  if (SleepTracker::SleepTracker::SleepState::Awake == state) {
    lv_label_set_text_fmt(label_state, "Sleep State: Awake");
  } else {
    lv_label_set_text_fmt(label_state, "Sleep State: Asleep");
  }
}

void InfiniSleep::ToggleTracking() {
  Refresh();

  if (!tracking_started) {
    sleepController.Start();

    SleepTrackerStarted();
  } else {
    sleepController.Stop();

    SleepTrackerStopped();
  }
}

void InfiniSleep::SleepTrackerStarted() {
  tracking_started = true;
  lv_label_set_text_fmt(label_btnStartStop, "Stop");
}

void InfiniSleep::SleepTrackerStopped() {
  tracking_started = false;
  lv_label_set_text_fmt(label_btnStartStop, "Start");
}
