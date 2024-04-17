#pragma once

#include <cstdint>
#include <lvgl/src/lv_misc/lv_task.h>

namespace Pinetime {
  namespace SleepTracker {
    class SleepTracker;
  }

  namespace Controllers {
    class SleepActivityService;
    class HeartRateController;
    class MotionController;

    class SleepController {
    public:
      enum class States {Stopped, Running, NotEnoughData};

      void Init(
        SleepTracker::SleepTracker* sleepTracker,
        Controllers::HeartRateController& heartRateController,
        Controllers::MotionController& motionController
      );
      void Start();
      void Stop();
      void Update();
      void SleepStateUpdated(uint8_t state);

      void SetSleepTracker(SleepTracker::SleepTracker* sleepTracker);
      void SetSleepActivityService(SleepActivityService* sleepActivityService);
      void SetHeartRateController(HeartRateController* heartRateController);
      void SetMotionRateController(MotionController* motionController);
      States GetState();
      uint8_t GetCurrentSleepState();
    
    private:
      SleepTracker::SleepTracker* sleepTracker = nullptr;
      SleepActivityService* sleepActivityService = nullptr;
      HeartRateController* heartRateController = nullptr;
      MotionController* motionController = nullptr;

      States controllerState = States::Stopped;
      uint8_t sleepState = 23;

      lv_task_t* taskUpdate;
    };
  }
}
