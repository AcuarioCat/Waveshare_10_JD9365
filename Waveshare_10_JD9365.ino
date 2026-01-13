/*
 Name:		Waveshare_10_JD9365.ino
 Created:	1/13/2026 6:16:18 AM
 Author:	Acuario
*/

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include "display\i2c.h"
#include "display\gt911.h"
#include "display\display_config.h"
#include "display/Waveshare10.h"

LGFX gfx;

void touch_task(void* arg);
TaskHandle_t touchTask;
DEV_I2C_Port i2cPort;

// the setup function runs once when you press reset or power the board
void setup() {

	Serial.begin(115200);

	i2cPort = DEV_I2C_Init();

	display_init(i2cPort);
	xTaskCreatePinnedToCore(touch_task, "Touch", 2048, NULL, 3, &touchTask, 0);

	// Init Display
	if (!gfx.begin()) {
		Serial.println("gfx.begin() failed!");
	}
	gfx.fillScreen(TFT_BLACK);
	set_display_backlight(i2cPort, 255);

}


// the loop function runs over and over again until power down or reset
void loop() {
    vTaskDelay(500);
    gfx.fillScreen(TFT_RED);
    vTaskDelay(500);
    gfx.fillScreen(TFT_BLUE);
    vTaskDelay(500);
    gfx.fillScreen(TFT_GREEN);
    vTaskDelay(500);
    gfx.fillScreen(TFT_YELLOW);
    vTaskDelay(500);
    gfx.fillScreen(TFT_ORANGE);
}

#define VERTICAL false
unsigned long nextPress = 0;                // next action time
void touch_task(void* arg) {
#define MAX_TOUCH_POINTS 1

    esp_lcd_touch_handle_t tp_handle = NULL;

    uint16_t touch_x[MAX_TOUCH_POINTS] = { 0 };
    uint16_t touch_y[MAX_TOUCH_POINTS] = { 0 };
    uint16_t touch_strength[MAX_TOUCH_POINTS] = { 0 };
    uint8_t touch_cnt = 0;

    unsigned long cur_millis = millis();
    int8_t noTouchCount = 0;
    bool touched = false;

    tp_handle = touch_gt911_init(i2cPort, false, true, true);

    Serial.println("+++ Start touch task");
    while (1) {
        esp_lcd_touch_read_data(tp_handle);

        bool pressed = esp_lcd_touch_get_coordinates(tp_handle, touch_x, touch_y, touch_strength, &touch_cnt, MAX_TOUCH_POINTS);

        if (pressed && touch_cnt > 0) {
            for (int i = 0; i < touch_cnt; i++) {
                if (touch_x[i] == 1 && touch_y[i] == 1) {
                    Serial.printf("TouchA %d X:%d: y:%d\n", i, touch_x[i], touch_y[i]);
                }
                else {
                     Serial.printf("TouchB %d X:%d: y:%d\n", i, touch_x[i], touch_y[i]);
                }
            }
        }

        cur_millis = millis();
        if (cur_millis >= nextPress)
        {
            if (pressed)
            {
                nextPress = cur_millis + 500;
                int16_t t_x = touch_x[0];
                int16_t t_y = touch_y[0];

                //Swap X and Y when vertical
                if (VERTICAL)
                    Serial.printf("Vertical X:%d: y:%d\n", t_x, t_y);
                else
                    Serial.printf("Vertical X:%d: y:%d\n", t_y, 1280 - t_x);

            }
        }

        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
