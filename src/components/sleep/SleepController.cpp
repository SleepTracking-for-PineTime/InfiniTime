#include "SleepController.h"

#include <components/sleep/SleepTracker.h>
#include <components/heartrate/HeartRateController.h>
#include <components/motion/MotionController.h>
#include <components/ble/SleepActivityService.h>

using namespace Pinetime::Controllers;

void UpdateCallBack(lv_task_t *task) {
  static_cast<SleepController*>(task->user_data)->Update();
}

void SleepController::Init(
  SleepTracker::SleepTracker* sleepTracker,
  Controllers::HeartRateController& heartRateController,
  Controllers::MotionController& motionController) {
  SetSleepTracker(sleepTracker);
  SetHeartRateController(&heartRateController);
  SetMotionRateController(&motionController);
}

void SleepController::Start() {
  //heartRateController->Start();
  taskUpdate = lv_task_create(UpdateCallBack, 1000, LV_TASK_PRIO_MID, this);

  controllerState = States::Running;
}

void SleepController::Stop() {
  //heartRateController->Stop();
  lv_task_del(taskUpdate);

  controllerState = States::Stopped;
  sleepState = 23;
  sleepActivityService->OnNewSleepStage(sleepState);
}

void SleepController::Update() {
  sleepTracker->UpdateAccel(motionController->X(), motionController->Y(), motionController->Z());
}

void SleepController::SleepStateUpdated(uint8_t state) {
  sleepState = state;

  if (sleepActivityService != nullptr) {
    sleepActivityService->OnNewSleepStage(state);
  }
}

void SleepController::SetSleepTracker(SleepTracker::SleepTracker* sleepTracker) {
  this->sleepTracker = sleepTracker;
  sleepTracker->Init([this](SleepTracker::SleepTracker::SleepState state) {
    SleepStateUpdated(static_cast<uint8_t>(state));
  });
}

void SleepController::SetSleepActivityService(SleepActivityService* sleepActivityService) {
  this->sleepActivityService = sleepActivityService;
}

void SleepController::SetHeartRateController(HeartRateController* heartRateController) {
  this->heartRateController = heartRateController;
}

void SleepController::SetMotionRateController(MotionController* motionController) {
  this->motionController = motionController;
}

SleepController::States SleepController::GetState() {
  return controllerState;
}

uint8_t Pinetime::Controllers::SleepController::GetCurrentSleepState() {
  return sleepState;
}
