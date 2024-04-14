#include "SleepTracker.h"

namespace Pinetime {
    namespace SleepTracker {
        void SleepTracker::Init(std::function<void(SleepState)> state_update_callback) {
            callback = state_update_callback;
        }

        uint8_t SleepTracker::GetCurrentSleepStage() {
          return static_cast<uint8_t>(state);
        }

        void SleepTracker::AnnounceUpdate(SleepState state) {
            if (callback)
                callback(state);
        }

        void VanHeesSleepTracker::UpdateAccel(float x, float y, float z) {
            // update averages
            accel_avgs[0] = ema(x, accel_avgs[0]);
            accel_avgs[1] = ema(y, accel_avgs[1]);
            accel_avgs[2] = ema(z, accel_avgs[2]);

            // estimate arm angle and update history
            arm_angle_hist++;
            arm_angle_hist[0] = ang(accel_avgs[0], accel_avgs[1], accel_avgs[2]);

            // check change in arm angle with some interval
            if (iteration == (fs*seconds_per_update)) {
                // average arm angle in this new window
                float arm_angle_mean = 0;
                for (int i = 0; i < fs*seconds_per_update; i++) {
                    arm_angle_mean += arm_angle_hist[i];
                }
                arm_angle_mean = arm_angle_mean/(fs*seconds_per_update);

                if (!std::isnan(arm_angle_mean_d)) {
                    // change in arm angle since last window
                    float arm_angle_change = fabsf(arm_angle_mean - arm_angle_mean_d);

                    // keep history of changes in arm angle for some longer durationnnn
                    arm_angle_change_hist++;
                    arm_angle_change_hist[0] = arm_angle_change;

                    if (dly > 0) {
                        // dont announce any guesses on state until arm_angle_change_hist has been
                        // filled at least once
                        dly--;
                    } else {
                        // if arm angle has not changed significantly between two windows for the last
                        // <classification_hist_size> windows, classify as sleep. otherwise classify as awake
                        SleepState new_state = SleepState::Sleep;
                        for (unsigned int i = 0; i < classification_hist_size; i++) {
                            if (arm_angle_change_hist[i] > arm_angle_threshold) {
                                new_state = SleepState::Awake;
                                break;
                            }
                        }

                        if (new_state != state) {
                            AnnounceUpdate(new_state);
                        }

                        state = new_state;
                    }
                }

                arm_angle_mean_d = arm_angle_mean;
                iteration = 0;
            }

            iteration++;
        }

        float VanHeesSleepTracker::ema(float x, float y) {
            return y + eta*(x - y);
        }

        float VanHeesSleepTracker::ang(float x, float y, float z) {
            return atanf(z / sqrtf(powf(x, 2) + powf(y, 2))) * 180/(float)std::numbers::pi;
        }
    };
};
