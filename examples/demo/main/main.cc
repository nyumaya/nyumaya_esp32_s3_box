/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "esp_log.h"
#include "esp_system.h"
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "main_functions.h"
#include "bsp_board.h"
#include "bsp_storage.h"

void detection_task(void) {

	setup();
	while (true) {
		loop();
	}
}

void mic_task(void) {

	setup_mic();
	while (true) {
		mic_loop();
	}
}


extern "C" void app_main() {

	ESP_ERROR_CHECK(bsp_board_init());
	ESP_ERROR_CHECK(bsp_board_power_ctrl(POWER_MODULE_AUDIO, true));

	xTaskCreate((TaskFunction_t)&detection_task, "detection", 8 * 1024, NULL, 8, NULL);
	xTaskCreate((TaskFunction_t)&mic_task, "microphone", 8 * 1024, NULL, 8, NULL);
	vTaskDelete(NULL);
}
