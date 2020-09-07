#include "power_up.h"

void power_up()
{
	gpio_set_direction(EN_OLED, GPIO_MODE_OUTPUT);
	gpio_set_level(EN_OLED, 1);

	vTaskDelay(2000 / portTICK_PERIOD_MS);
}

