/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: QN908XC
package_id: QN9080C
mcu_data: ksdk2_0
processor_version: 2.0.0
board: QN908XCDK
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_iocon.h"
#include "pin_mux_adc.h"

#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN22_IDX                       22u   /*!< Pin number for pin 22 in a port */
#define PIN0_IDX                         0u   /*!< Pin number for pin 8 in a port */
#define PIN1_IDX                         1u
#define PORTA_IDX                        0u   /*!< Port index */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm4}
- pin_list:
  - {pin_num: '29', peripheral: FLEXCOMM0, signal: TXD, pin_signal: GPIOA16/CS2/SCT0_OUT1/CTIMER2_MAT0/FC0_TXD/FC3_MOSI/QDEC0_A, pull_control: High_Z, drive_strength: low}
  - {pin_num: '23', peripheral: SWD, signal: SWCLK, pin_signal: SWCLK/GPIOA22/SCT0_IN2/CTIMER3_MAT0/FC2_SDA_SSEL0/FC3_SSEL3/QDEC1_A, pull_control: Pull_down, drive_strength: low}
  - {pin_num: '28', peripheral: FLEXCOMM0, signal: RXD, pin_signal: GPIOA17/CS3/SD_DAC/CTIMER2_MAT1/FC0_RXD/FC3_MISO/QDEC0_B, pull_control: High_Z, drive_strength: low}
  - {pin_num: '46', peripheral: ADC, signal: 'CH, 4', pin_signal: GPIOA8/ADC4/SCT0_IN0/CTIMER1_CAP1/FC1_TXD_SCL/BLE_PTI2/SPIFI_IO2, pull_control: High_Z, drive_strength: low}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins_adc(void) {
  const uint32_t portA_pin0_config = (
    IOCON_FUNC1 |                                            /* Selects pin function 1 */
    IOCON_MODE_HIGHZ |                                       /* Selects High-Z function */
    IOCON_DRIVE_LOW                                          /* Enable low drive strength */
  );
  IOCON_PinMuxSet(IOCON, PORTA_IDX, PIN0_IDX, portA_pin0_config); /* PORTA PIN8 (coords: 46) is configured as ADC4 */

  const uint32_t portA_pin1_config = (
    IOCON_FUNC1 |                                            /* Selects pin function 1 */
    IOCON_MODE_HIGHZ |                                       /* Selects High-Z function */
    IOCON_DRIVE_LOW                                          /* Enable low drive strength */
  );
  IOCON_PinMuxSet(IOCON, PORTA_IDX, PIN1_IDX, portA_pin1_config);


  const uint32_t portA_pin16_config = (
    IOCON_FUNC4 |                                            /* Selects pin function 4 */
    IOCON_MODE_HIGHZ |                                       /* Selects High-Z function */
    IOCON_DRIVE_LOW                                          /* Enable low drive strength */
  );
  IOCON_PinMuxSet(IOCON, PORTA_IDX, PIN16_IDX, portA_pin16_config); /* PORTA PIN16 (coords: 29) is configured as FC0_TXD */
  const uint32_t portA_pin17_config = (
    IOCON_FUNC4 |                                            /* Selects pin function 4 */
    IOCON_MODE_HIGHZ |                                       /* Selects High-Z function */
    IOCON_DRIVE_LOW                                          /* Enable low drive strength */
  );
  IOCON_PinMuxSet(IOCON, PORTA_IDX, PIN17_IDX, portA_pin17_config); /* PORTA PIN17 (coords: 28) is configured as FC0_RXD */
  const uint32_t portA_pin22_config = (
    IOCON_FUNC0 |                                            /* Selects pin function 0 */
    IOCON_MODE_PULLDOWN |                                    /* Selects pull-down function */
    IOCON_DRIVE_LOW                                          /* Enable low drive strength */
  );
  IOCON_PinMuxSet(IOCON, PORTA_IDX, PIN22_IDX, portA_pin22_config); /* PORTA PIN22 (coords: 23) is configured as SWCLK */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
