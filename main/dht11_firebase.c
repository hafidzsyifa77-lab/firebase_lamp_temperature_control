/*
 * dht11_firebase.c
 *
 *  Created on: 1 Jun 2026
 *      Author: hz_haze
 */
#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include <sys/_intsup.h>
#include "dht.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include "dht11_firebase.h"

#define DHT_GPIO GPIO_NUM_4
#define BASE_URL "YOUR_FIREBASE_RTDB_URL"

static const char *TAG = "DHT_FIREBASE";

void dht11_send(const char *json_string){
	char url[128];
   
    snprintf(url, sizeof(url), "%s/sensor.json", BASE_URL);

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_PATCH, 
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) return;

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, json_string, strlen(json_string));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Successfully PATCH data: %s", json_string);
    } else {
        ESP_LOGE(TAG, "Failed PATCH: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
}

void dht11_read_and_send(void) {
float temperature = 0;
    float humidity = 0;

    if (dht_read_float_data(DHT_TYPE_DHT11, DHT_GPIO, &humidity, &temperature) == ESP_OK) {
        
        static float last_temperature = -999.0;
        static float last_humidity = -999.0;
        char json_buffer[64];

        if (temperature != last_temperature) {
            ESP_LOGW(TAG, "The temperature changes from %.1f to %.1f. Send Temperature!", last_temperature, temperature);
            
            snprintf(json_buffer, sizeof(json_buffer), "{\"temperature\": %.1f}", temperature);
            dht11_send(json_buffer);
            
            last_temperature = temperature;
        }

        if (humidity != last_humidity) {
            ESP_LOGW(TAG, "Humidity changes from %.1f to %.1f. Send Himidity!", last_humidity, humidity);
            
            snprintf(json_buffer, sizeof(json_buffer), "{\"humidity\": %.1f}", humidity);
            dht11_send(json_buffer);
            
            last_humidity = humidity; 
        }
        
        if (temperature == last_temperature && humidity == last_humidity) {
            ESP_LOGI(TAG, "Temp & Hum still same (%.1f°C | %.1f%%). Skip Firebase!", temperature, humidity);
        }

    } else {
        ESP_LOGE(TAG, "failed to read DHT11 sensor");
    }
}




