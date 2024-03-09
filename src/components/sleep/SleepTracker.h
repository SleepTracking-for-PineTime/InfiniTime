#pragma once

#include <cstdint>
#include <cmath>
#include <numbers>

#include "utility/CircularBuffer.h"

namespace Pinetime {
    namespace SleepTracker {
        class SleepTracker {
            public:
                virtual void UpdateAccel(float x, float y, float z) = 0;
                virtual ~SleepTracker() = default;
                void Init(void (*state_update_callback)(uint8_t));

            private:
                void (*callback)(uint8_t) = nullptr;

            protected:
                void AnnounceUpdate(uint8_t state);
        };

        class VanHeesSleepTracker : public SleepTracker {
            public:
                void UpdateAccel(float x, float y, float z) override;
                ~VanHeesSleepTracker() override = default;

            private:
                static constexpr int fs = 10;
                static constexpr float eta = 0.005;
                static constexpr int seconds_per_update = 5;
                static constexpr size_t classification_hist_size = 60;
                static constexpr float arm_angle_threshold = 5.0;

                int iteration = 0;
                int dly = classification_hist_size;
                float accel_avgs[3] = {0};
                Utility::CircularBuffer<float, classification_hist_size> arm_angle_change_hist = {0};
                Utility::CircularBuffer<float, (size_t)fs*seconds_per_update> arm_angle_hist = {0};
                float arm_angle_mean_d = NAN;
                uint8_t state = 255;

                static float ema(float x, float y);
                static float ang(float x, float y, float z);
        };
    };
};
