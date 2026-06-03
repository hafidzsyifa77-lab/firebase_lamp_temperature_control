/*
 * lamp_firebase.c
 *
 *  Created on: 2 Jun 2026
 *      Author: hz_haze
 */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "lamp_firebase.h"

// Kita definisikan dua PIN GPIO untuk masing-masing lampu
#define LAMP_1_GPIO GPIO_NUM_5
#define LAMP_2_GPIO GPIO_NUM_18  // <== Ganti dengan pin cadanganmu (misal GPIO 18)

#define BASE_URL "https://smart-home-2-6a363-default-rtdb.firebaseio.com"

static const char *TAG = "LAMP_MODULE";
static char respon_buffer[128] = {0};
static int total_panjang_data = 0;

static esp_err_t http_event_handler(esp_http_client_event_t *evt) {
    switch(evt->event_id) {
        case HTTP_EVENT_ON_DATA:
            if (!esp_http_client_is_chunked_response(evt->client)) {
                if (total_panjang_data + evt->data_len < sizeof(respon_buffer) - 1) {
                    memcpy(respon_buffer + total_panjang_data, evt->data, evt->data_len);
                    total_panjang_data += evt->data_len;
                    respon_buffer[total_panjang_data] = '\0';
                }
            }
            break;
        default:
            break;
    }
    return ESP_OK;
}

// Fungsi ini sekarang murni mengambil teks JSON dari Firebase
void fetch_json_from_firebase(const char *path) {
    char url[128];
    snprintf(url, sizeof(url), "%s%s", BASE_URL, path);
	 
    memset(respon_buffer, 0, sizeof(respon_buffer));
    total_panjang_data = 0;
	
    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET,
        .event_handler = http_event_handler,
    };
	
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) return;
	
    esp_err_t err = esp_http_client_perform(client);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed Connecting to Firebase: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
}

// Fungsi Otak untuk mengecek dan mengatur kedua lampu sekaligus
void check_and_set_lamp(void) {
    // 1. Atur kedua PIN sebagai OUTPUT
    gpio_set_direction(LAMP_1_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(LAMP_2_GPIO, GPIO_MODE_OUTPUT);
	
    // 2. Ambil data satu folder /lamp.json (isinya: {"lamp1":x,"lamp2":y})
    fetch_json_from_firebase("/lamp.json");
	
    if (total_panjang_data > 0) {
        int status_lamp1 = 0;
        int status_lamp2 = 0;

        // 3. Cek Status Lampu 1
        if (strstr(respon_buffer, "\"lamp1\":1") != NULL) {
            status_lamp1 = 1;
        }
        
        // 4. Cek Status Lampu 2
        if (strstr(respon_buffer, "\"lamp2\":1") != NULL) {
            status_lamp2 = 1;
        }

        // 5. Eksekusi setrum ke masing-masing LED fisik
        gpio_set_level(LAMP_1_GPIO, status_lamp1);
        gpio_set_level(LAMP_2_GPIO, status_lamp2);
		
        ESP_LOGI(TAG, "Status Saklar -> LED 1: %s | LED 2: %s", 
                 status_lamp1 ? "ON" : "OFF", status_lamp2 ? "ON" : "OFF");
    }
}