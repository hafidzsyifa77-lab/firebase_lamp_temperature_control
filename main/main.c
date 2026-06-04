/*
 * main.c
 *
 *  Created on: 1 Jun 2026
 *      Author: hz_haze
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h" 
#include "esp_log.h"

#include "wifi_connection.h"
#include "dht11_firebase.h"
#include "lamp_firebase.h"

void lamp_control(void *pvParameters){
	while (1) {
		check_and_set_lamp();
		vTaskDelay(pdMS_TO_TICKS(300));
	}
}

void sensor_control(void *pvParameters){
	ESP_LOGI("MAIN", "Starting Sensor Task....");
	
	while (1) {
		dht11_read_and_send();
		vTaskDelay(pdMS_TO_TICKS(180000));
	}
	vTaskDelete(NULL);
}

void app_main(void){
	wifi_connection_init();
	
	xTaskCreate(lamp_control, "Lamp_Task", 4096, NULL, 5, NULL);
	
	xTaskCreate(sensor_control, "Sensor_TASK", 4096, NULL, 4, NULL);
	
	
}