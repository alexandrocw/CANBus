#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "Freertos/task.h"
#include "esp_log.h"
#include "mcp2515.h"

static const char *TAG = "CAN";

void receive_can_message() {
  mcp2515_message_t message;
  if (mcp2515_receive_message(&message)) {
    ESP_LOGI(TAG, "CAN message received");
    for (int i = 0; i < message.length; i++) {
      ESP_LOGI(TAG, "data[$d]: $02X", i, message.data[i]);
    }
  }
}

void can_task(void *pvParameters) {
  eSP_LOGI(TAG, "CAN task started");
  
  mcp2515_init();

  while(1) {
    receive_can_message();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void app_main(void)
{
  xTaskCreate(&can_task, "can_task", 4096, NULL, 5, NULL);
}
