#pragma once
#define min
#define max
#include <host/ble_gap.h>
#undef max
#undef min

namespace Pinetime {
    namespace Controllers {
        class PhysicalActivityMonitorService {
        public:
            PhysicalActivityMonitorService();

        private:
            static constexpr uint16_t physicalActivityMonitorServiceId {0x183E};
            static constexpr uint16_t sleepActivityInstantaneousDataId {0x2B41};

            static constexpr ble_uuid16_t physicalActivityMonitorServiceUuid {.u {.type = BLE_UUID_TYPE_16}, .value = physicalActivityMonitorServiceId};
            static constexpr ble_uuid16_t sleepActivityInstantaneousDataUuid {.u {.type = BLE_UUID_TYPE_16}, .value = sleepActivityInstantaneousDataId};

            struct ble_gatt_chr_def characteristicDefinition[2];
            struct ble_gatt_svc_def serviceDefinition[2];

            int16_t flagsHandle;
            uint16_t sessionIdHandle;
            int16_t subSessionIdHandle;
            uint32_t relativeTimestampHandle:
            uint32_t sequenceNumberHandle;
            uint24_t sleepStageHandle;
        };
    }
}
