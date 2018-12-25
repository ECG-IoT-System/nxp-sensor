#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"

#include "clock_config.h"
#include "pin_mux.h"
#include "getGensor.h"

int main(void)
{
	//int16_t * data;
	uint8_t * gdata;
	gdata = getGSensor(); // gsensor_array_uint8 [x_H][x_L][y_H][y_L][z_H][z_L]

	PRINTF("G sensor test!\r\n");
	int i;
	for(i=0;i<3;++i){
		PRINTF("G sensor data no. %d time\r\n",i);
		PRINTF("x high byte = 0x%x\r\n", gdata[6*i]);
		PRINTF("x low byte = 0x%x\r\n", gdata[6*i+1]);
		PRINTF("y high byte = 0x%x\r\n", gdata[6*i+2]);
		PRINTF("y low byte = 0x%x\r\n", gdata[6*i+3]);
		PRINTF("z high byte = 0x%x\r\n", gdata[6*i+4]);
		PRINTF("z low byte = 0x%x\r\n\n", gdata[6*i+5]);
	}
	//get uint8_t readBuff[7];
	/* Transfer readBuff to G sensor value
	 * status0_value = readBuff[0];
     * x = ((int16_t)(((readBuff[1] * 256U) | readBuff[2]))) / 4U;
     * y = ((int16_t)(((readBuff[3] * 256U) | readBuff[4]))) / 4U;
     * z = ((int16_t)(((readBuff[5] * 256U) | readBuff[6]))) / 4U;
	 */


	/*
	int8   : -128 ~ 127
	int16  : -32768 ~ 32767
	*/

	/*
	PRINTF("Test Test Test ---- x \r\n");
	PRINTF("(gdata[1] * 256U) = %d\r\n",(gdata[1] * 256U));
	PRINTF("((gdata[1] * 256U) | gdata[2]) = %d\r\n",((gdata[1] * 256U) | gdata[2]));
	PRINTF("((int16_t)((gdata[1] * 256U) | gdata[2])) = %d\r\n",((int16_t)((gdata[1] * 256U) | gdata[2])));
	PRINTF("((int8_t)((gdata[1] * 256U) | gdata[2])) = %d\r\n\n",((int8_t)((gdata[1] * 256U) | gdata[2])));

	PRINTF("Test Test Test ---- y \r\n");
	PRINTF("(gdata[3] * 256U) = %d\r\n",(gdata[3] * 256U));
	PRINTF("((gdata[3] * 256U) | gdata[4]) = %d\r\n",((gdata[3] * 256U) | gdata[4]));
	PRINTF("((int16_t)((gdata[3] * 256U) | gdata[4])) = %d\r\n",((int16_t)((gdata[3] * 256U) | gdata[4])));
	PRINTF("((int8_t)((gdata[3] * 256U) | gdata[4])) = %d\r\n\n",((int8_t)((gdata[3] * 256U) | gdata[4])));

	PRINTF("Test Test Test ---- z \r\n");
	PRINTF("(gdata[5] * 256U) = %d\r\n",(gdata[5] * 256U));
	PRINTF("((gdata[5] * 256U) | gdata[6]) = %d\r\n",((gdata[5] * 256U) | gdata[6]));
	PRINTF("((int16_t)((gdata[5] * 256U) | gdata[6])) = %d\r\n",((int16_t)((gdata[5] * 256U) | gdata[6])));
	PRINTF("((int8_t)((gdata[5] * 256U) | gdata[6])) = %d\r\n\n",((int8_t)((gdata[5] * 256U) | gdata[6])));
	*/


	/*
	for(i=0;i<10;++i){

		PRINTF("16bit - Data Format 1\r\n");
		PRINTF("G sensor data x : 0x%x\r\n",data[3*i]);
		PRINTF("G sensor data y : 0x%x\r\n",data[3*i+1]);
		PRINTF("G sensor data z : 0x%x\r\n",data[3*i+2]);

		PRINTF("16bit - Data Format 2\r\n");
		PRINTF("G sensor data x : %d\r\n",data[3*i]);
		PRINTF("G sensor data y : %d\r\n",data[3*i+1]);
		PRINTF("G sensor data z : %d\r\n",data[3*i+2]);
	}
	*/

}
