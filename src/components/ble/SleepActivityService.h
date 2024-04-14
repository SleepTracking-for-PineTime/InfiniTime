#pragma once

#define min // workaround: nimble's min/max macros conflict with libstdc++
#define max
#include <host/ble_gap.h>
#undef max
#undef min

namespace Pinetime {
  namespace Controllers {

    class SleepActivityService {
    public:
      SleepActivityService();

      void Init();
      int OnSleepStageRequested(uint16_t attributeHandle, ble_gatt_access_ctxt* context);
      void OnNewSleepStage(uint8_t sleepStage);

    private:
      // 00060000-78fc-48fe-8e23-433b3a1942d0
      static constexpr ble_uuid128_t BaseUuid() {
          return CharUuid(0x00, 0x00);
      }

      // 0006yyxx-78fc-48fe-8e23-433b3a1942d0
      static constexpr ble_uuid128_t CharUuid(uint8_t x, uint8_t y) {
          return ble_uuid128_t {.u = {.type = BLE_UUID_TYPE_128},
                                .value = {0xd0, 0x42, 0x19, 0x3a, 0x3b, 0x43, 0x23, 0x8e, 0xfe, 0x48, 0xfc, 0x78, y, x, 0x06, 0x00}};
      }

      ble_uuid128_t sleepActivityServiceUuid {BaseUuid()};

      ble_uuid128_t sleepStageCharacteristicUuid {CharUuid(0x00, 0x01)};

      const struct ble_gatt_chr_def characteristicDefinition[2];
      const struct ble_gatt_svc_def serviceDefinition[2];

      uint16_t sleepStageHandle;

      uint8_t currentSleepStage = 23; // 23 = Undefined
    };
  }
}
