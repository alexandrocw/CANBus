#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "mcp2515.h"

static const char *TAG = "CAN";

void send_can_message(uint8_t data[], uint8_t len) {
  mcp2515_message_t message;
  message.sid = 0x123;
  message.rtr = 0;
  message.length = len;
  memcpy(message.data, data, len);
  mcp2515_send_message(&message);
}

void can_task(void *pvParameters) {
  ESP_LOGI(TAG, "CAN task started");

  mcp2515_init();

  while(1) {
    uint8_t data[] = {0x01, 0x02, 0x03};
    send_can_message(data, sizeof(data));
    ESP_LOGI(TAG, "CAN message sent");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void app_main(void)
{
  xTaskCreate(&can_task, "can_task", 4096, NULL, 5, NULL);
}
