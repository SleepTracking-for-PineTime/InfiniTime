#include "SleepActivityService.h"

int sleepActivityServiceCallback(uint16_t /*connHandle*/, uint16_t attrHandle, struct ble_gatt_access_ctxt* ctxt, void* arg) {
  auto* sleepActivityService = static_cast<Pinetime::Controllers::SleepActivityService*>(arg);
  return sleepActivityService->OnSleepStageRequested(attrHandle, ctxt);
}

Pinetime::Controllers::SleepActivityService::SleepActivityService()
 :  characteristicDefinition {{.uuid = &sleepStageCharacteristicUuid.u,
                              .access_cb = sleepActivityServiceCallback,
                              .arg = this,
                              .flags = BLE_GATT_CHR_F_READ,
                              .val_handle = &sleepStageHandle},
                              {0}},
    serviceDefinition {
      {.type = BLE_GATT_SVC_TYPE_PRIMARY, .uuid = &sleepActivityServiceUuid.u, .characteristics = characteristicDefinition},
      {0}
    } {
}

void Pinetime::Controllers::SleepActivityService::Init() {
  int res = 0;
  res = ble_gatts_count_cfg(serviceDefinition);
  ASSERT(res == 0);

  res = ble_gatts_add_svcs(serviceDefinition);
  ASSERT(res == 0);
}

int Pinetime::Controllers::SleepActivityService::OnSleepStageRequested(uint16_t attributeHandle, ble_gatt_access_ctxt* context) {
  if (attributeHandle == sleepStageHandle) {
    uint8_t buffer = currentSleepStage;
    
    int res = os_mbuf_append(context->om, &buffer, 1);
    return (res == 0) ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
  }

  return 0;
}

void Pinetime::Controllers::SleepActivityService::OnNewSleepStage(uint8_t sleepStage) {
  currentSleepStage = sleepStage;
}
