#include "InfiniSleep.h"

#include <components/heartrate/HeartRateController.h>
#include <components/motion/MotionController.h>

using namespace Pinetime::Applications::Screens;

namespace {
  void btnTransferDataEventHandler(lv_obj_t* obj, lv_event_t event) {
      auto* screen = static_cast<InfiniSleep*>(obj->user_data);

      if (event == LV_EVENT_CLICKED) {
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
  btn_transferData->user_data = this;
  lv_obj_set_height(btn_transferData, 100);
  lv_obj_align(btn_transferData, nullptr, LV_ALIGN_IN_TOP_MID, 0, 10);
  lv_obj_set_event_cb(btn_transferData, btnTransferDataEventHandler);

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

  taskRefresh = lv_task_create(RefreshTaskCallback, 100, LV_TASK_PRIO_MID, this);
}

InfiniSleep::~InfiniSleep() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
}

void InfiniSleep::Refresh() {
  if (!tracking_started)
    return;

  std::unique_ptr<ActivityPacket> activityPacket = std::make_unique<ActivityPacket>();

  activityPacket->heart_rate = heartRateController.HeartRate();
  if (heartRateController.State() == Controllers::HeartRateController::States::Running) {
    lv_label_set_text_fmt(label_heartRate, "Heartrate: %03d", activityPacket->heart_rate);
  } else {
    lv_label_set_text_fmt(label_heartRate, "---");
  }

  activityPacket->motion_x = motionController.X();
  lv_label_set_text_fmt(label_motionX, "Motion X: %03d", activityPacket->motion_x);

  activityPacket->motion_y = motionController.Y();
  lv_label_set_text_fmt(label_motionY, "Motion Y: %03d", activityPacket->motion_y);
  
  activityPacket->motion_z = motionController.Z();
  lv_label_set_text_fmt(label_motionZ, "Motion Z: %03d", activityPacket->motion_z);

  activityPackets.push_back(std::move(activityPacket));
}

void InfiniSleep::StartTracking() {
  if (!tracking_started) {
    heartRateController.Start();
    systemTask.PushMessage(Pinetime::System::Messages::DisableSleeping);
    
    tracking_started = true;
  } else {
    heartRateController.Stop();
    systemTask.PushMessage(Pinetime::System::Messages::EnableSleeping);

    tracking_started = false;
  }
}
