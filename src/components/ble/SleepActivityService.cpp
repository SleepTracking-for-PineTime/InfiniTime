#include "SleepActivityService.h"

#include <components/sleep/SleepController.h>
#include "components/ble/NimbleController.h"

int sleepActivityServiceCallback(uint16_t /*connHandle*/, uint16_t attrHandle, struct ble_gatt_access_ctxt* ctxt, void* arg) {
  auto* sleepActivityService = static_cast<Pinetime::Controllers::SleepActivityService*>(arg);
  return sleepActivityService->OnSleepStageRequested(attrHandle, ctxt);
}

Pinetime::Controllers::SleepActivityService::SleepActivityService(NimbleController& nimble, SleepController& sleepController)
  : nimble {nimble},
    sleepController {sleepController},
    characteristicDefinition {{.uuid = &sleepStageCharacteristicUuid.u,
                              .access_cb = sleepActivityServiceCallback,
                              .arg = this,
                              .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
                              .val_handle = &sleepStageHandle},
                              {0}},
    serviceDefinition {
      {.type = BLE_GATT_SVC_TYPE_PRIMARY, .uuid = &sleepActivityServiceUuid.u, .characteristics = characteristicDefinition},
      {0}
    } {
  // TODO: Bad design, as controller and service depend on each other
  // It might be possible to let NimbleController be the bridge between the controller and the service
  sleepController.SetSleepActivityService(this);
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

  if (sleepStageNoficationEnabled) {
    uint16_t connectionHandle = nimble.connHandle();

    if (connectionHandle == 0 || connectionHandle == BLE_HS_CONN_HANDLE_NONE) {
      return;
    }

    auto* om = ble_hs_mbuf_from_flat(&sleepStage, 1);
    ble_gattc_notify_custom(connectionHandle, sleepStageHandle, om);
  }
}

void Pinetime::Controllers::SleepActivityService::SubscribeNotification(uint16_t attributeHandle) {
  if (attributeHandle == sleepStageHandle)
    sleepStageNoficationEnabled = true;
}

void Pinetime::Controllers::SleepActivityService::UnsubscribeNotification(uint16_t attributeHandle) {
  if (attributeHandle == sleepStageHandle)
    sleepStageNoficationEnabled = false;
}

bool Pinetime::Controllers::SleepActivityService::IsSleepStageNotificationSubscribed() const {
  return sleepStageNoficationEnabled;
}
