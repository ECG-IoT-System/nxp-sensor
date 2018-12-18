/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_adc.h"
#include "fsl_power.h"

#include "clock_config.h"
#include "pin_mux.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC_BASE ADC                           /* Demo base address */
#define DEMO_ADC_CHANNEL 0                          /* Select channel */
#define DEMO_ADC_CFG_IDX 0                          /* Select configuration */
#define DEMO_ADC_TRIGGER kADC_TriggerSelectSoftware /* Software trigger */
#define ADC_BURST_EXAMPLE_NUMBER 90

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint32_t g_AdcVinn;
float g_AdcBandgap;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void ADC_Configuration(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */

/*
uint16_t * hello(void){
	int a;
	int b;
	int c;

	static uint16_t r[10];
	int i=0;

	for (i = 0; i < 10 ; i++ ){
		r[i]=i;

	}

	a = 8;
	b = 3;
	c = a + b;
	return r;
}
*/

uint8_t * adc_conv(void)
{
    uint32_t i;
    static uint32_t data[ADC_BURST_EXAMPLE_NUMBER];

    //static uint16_t d[ADC_BURST_EXAMPLE_NUMBER];
    static uint8_t d_array[ADC_BURST_EXAMPLE_NUMBER*2];

    /* Power on ADC */
    //POWER_EnableADC(true);

    /* Initialize board hardware. */
    BOARD_InitPins_adc();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    //PRINTF("\r\nADC burst example.\r\n");

    /* Configure the converter and work mode. */
    ADC_Configuration();
    //PRINTF("Configuration Done.\r\n");
    //PRINTF("Press any key to get demo channel's ADC value ...\r\n");

    /**
     * ADC single mode
     * (Vext - Vinn) / Vref = Vreg / 2^22  ==> Vext = Vreg * Vref / 2^22 + Vinn
     */
    //float fresult;
    //while (1)
    //{
        //GETCHAR();

        /* Start burst */
        ADC_Enable(DEMO_ADC_BASE, true);
        /* Do software trigger */
        ADC_DoSoftwareTrigger(DEMO_ADC_BASE);
        for (i = 0; i < ADC_BURST_EXAMPLE_NUMBER; i++)
        {
            /* Wait for convert complete */
            while (!(ADC_GetStatusFlags(DEMO_ADC_BASE) & kADC_DataReadyFlag))
            {
            }
            /* Get the result */
            //data[i] = 0x11111111;
            data[i] = ADC_GetConversionResult(DEMO_ADC_BASE);

            d_array[2*i] = data[i] >> 15;
            d_array[2*i + 1] = data[i] >> 7;


        }
        /* Stop burst */
        ADC_Enable(DEMO_ADC_BASE, false);


        return d_array;
        //PRINTF("Sample %d numbers: \r\n", ADC_BURST_EXAMPLE_NUMBER);
        //for (i = 0; i < ADC_BURST_EXAMPLE_NUMBER; i++)
        //{
            //fresult = ADC_ConversionResult2Mv(DEMO_ADC_BASE, DEMO_ADC_CHANNEL, DEMO_ADC_CFG_IDX, g_AdcBandgap,
                                           //   g_AdcVinn, data[i]);
            //PRINTF("Original: 0x%x\t Ch: %d\t Result: %f(mv)\r\n", data[i], DEMO_ADC_CHANNEL, fresult);
           // PRINTF("%.3f\n",fresult);
            //hello(); // debug - for call the subroutine
        //}
        //hello(); // debug - for call the subroutine
    //
}

static void ADC_Configuration(void)
{
    adc_config_t adcConfigStruct;
    adc_sd_config_t adcSdConfigStruct;

    /**
     * Initial ADC to default configuration.
     */
    ADC_GetDefaultConfig(&adcConfigStruct);
    adcConfigStruct.channelEnable = (1U << DEMO_ADC_CHANNEL);
    adcConfigStruct.channelConfig = (DEMO_ADC_CFG_IDX << DEMO_ADC_CHANNEL);
    adcConfigStruct.triggerSource = DEMO_ADC_TRIGGER;
    adcConfigStruct.convMode = kADC_ConvModeBurst;
    ADC_Init(DEMO_ADC_BASE, &adcConfigStruct);

    /* Initial ADC Sigma Delta(SD) configuration */
    ADC_GetSdDefaultConfig(&adcSdConfigStruct);
    ADC_SetSdConfig(DEMO_ADC_BASE, DEMO_ADC_CFG_IDX, &adcSdConfigStruct);

    /* Bandgap voltage */
    //g_AdcBandgap = (DEMO_ADC_BASE, DEMO_ADC_CFG_IDX);

    /* Calibration VINN value */
    //g_AdcVinn = ADC_GetVinnCalibrationResult(DEMO_ADC_BASE, &adcConfigStruct);
}
