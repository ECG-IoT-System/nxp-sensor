#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"

#include "clock_config.h"
#include "pin_mux.h"
#include "getGensor.h"

int main(void)
{
	int16_t * data;
	data = getGSensor();

	int i;
	PRINTF("Test Start!! \r\n");

	for(i=0;i<10;++i){
		PRINTF("G sensor data x : %d\r\n",data[3*i]);
		PRINTF("G sensor data y : %d\r\n",data[3*i+1]);
		PRINTF("G sensor data z : %d\r\n",data[3*i+2]);
	}

}
