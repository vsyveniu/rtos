#include "blinker.h"

/* void led_on(int led_num)
{
	gpio_set_direction(led_num, GPIO_MODE_OUTPUT);
    gpio_set_level(led_num, 1);
}

void led_off(int led_num)
{
    gpio_set_level(led_num, 0);
} */

void ledc_channel_setup(ledc_timer_t timer_num, int32_t frequency, ledc_channel_t channel, int gpio){
	
	ledc_timer_config_t ledc_timer = {
 		.speed_mode = LEDC_SPEED_MODE,
 		.timer_num = timer_num,
 		.duty_resolution = LEDC_DUTY_RESOLUTION,
 		.freq_hz = frequency,
 	};

	ledc_timer_config(&ledc_timer);		

	ledc_channel_config_t ledc_channel = {
		.channel = channel,
		.speed_mode = LEDC_SPEED_MODE,
		.gpio_num = gpio,
		.duty = 0,
		.timer_sel = timer_num,
		.hpoint = 0,
	};

	ledc_channel_config(&ledc_channel);
	ledc_fade_func_install(0);
}