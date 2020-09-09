#include "blinker.h"

void pulse_led(){

	int volt;

	dac_output_enable(DAC_CHANNEL_2);

	for (volt = 0; volt  < 255; volt++)
	{
		dac_output_voltage(DAC_CHANNEL_2, volt);
		ets_delay_us(8000);
	}

	for (volt = 255; volt  > 0; volt--)
	{
		dac_output_voltage(DAC_CHANNEL_2, volt);
		ets_delay_us(8000);
	}		 
}