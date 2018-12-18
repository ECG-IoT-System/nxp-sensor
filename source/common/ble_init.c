/*! *********************************************************************************
 * \addtogroup BLE
 * @{
 ********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
*
* \file
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

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ble_init.h"
#include "ble_general.h"
#include "ble_host_task_config.h"
#include "ble_controller_task_config.h"
#include "hci_transport.h"
#include "board.h"

#if !gUseHciTransportDownward_d
#include "controller_interface.h"
#ifndef CPU_QN908X
#include "fsl_xcvr.h"
#endif /* CPU_QN908X */
#include "Flash_Adapter.h"
#endif /* gUseHciTransportDownward_d */

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/
#ifndef cMCU_SleepDuringBleEvents
    #define cMCU_SleepDuringBleEvents    0
#endif

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
#ifndef CPU_QN908X
extern bool_t gEnableSingleAdvertisement;
extern bool_t gMCUSleepDuringBleEvents;
#endif /* CPU_QN908X */

bleResult_t Ble_Initialize
(
    gapGenericCallback_t gapGenericCallback
)
{
        
#if (gUseHciTransportDownward_d == 1)
    
    /* Configure HCI Transport */
    hcitConfigStruct_t hcitConfigStruct = 
    {
        .interfaceType = gHcitInterfaceType_d,
        .interfaceChannel = gHcitInterfaceNumber_d,
        .interfaceBaudrate = gHcitInterfaceSpeed_d,
        .transportInterface =  Ble_HciRecv
    };
      
    /* HCI Transport Init */
    if (gHciSuccess_c != Hcit_Init(&hcitConfigStruct))
    {
        return gHciTransportError_c;
    }
    
    /* BLE Host Tasks Init */
    if (osaStatus_Success != Ble_HostTaskInit())
    {
        return gBleOsError_c;
    }
    
    /* BLE Host Stack Init */
    return Ble_HostInitialize(gapGenericCallback, Hcit_SendPacket);
    
#elif (gUseHciTransportUpward_d == 1)
    
#ifndef CPU_QN908X
    /* BLE Radio Init */
    XCVR_Init(BLE_MODE, DR_1MBPS);    
    XCVR_SetXtalTrim( (uint8_t)gHardwareParameters.xtalTrim );
#endif /* CPU_QN908X */

    if (osaStatus_Success != Controller_TaskInit())
    {
        return gBleOsError_c;
    }

#ifndef CPU_QN908X    
    gMCUSleepDuringBleEvents = cMCU_SleepDuringBleEvents;
#endif /* CPU_QN908X */
    
    /* BLE Controller Init */
    if (osaStatus_Success != Controller_Init(Hcit_SendPacket))
    {
        return gBleOsError_c;
    }
    
    /* Configure HCI Transport */
    hcitConfigStruct_t hcitConfigStruct = 
    {
        .interfaceType = gHcitInterfaceType_d,
        .interfaceChannel = gHcitInterfaceNumber_d,
        .interfaceBaudrate = gHcitInterfaceSpeed_d,
        .transportInterface =  (hciTransportInterface_t)Hci_SendPacketToController
    };
    
    return Hcit_Init(&hcitConfigStruct);
    
#else    

#ifndef CPU_QN908X
    /* BLE Radio Init */    
    XCVR_Init(BLE_MODE, DR_1MBPS);    
    XCVR_SetXtalTrim( (uint8_t)gHardwareParameters.xtalTrim );
#endif /* CPU_QN908X */
    
    /* BLE Controller Task Init */
    if (osaStatus_Success != Controller_TaskInit())
    {
        return gBleOsError_c;
    }
     
#ifndef CPU_QN908X 
    gEnableSingleAdvertisement = TRUE;
    gMCUSleepDuringBleEvents = cMCU_SleepDuringBleEvents;
#endif /* CPU_QN908X */
        
    /* BLE Controller Init */
    if (osaStatus_Success != Controller_Init(Ble_HciRecv))
    {
        return gBleOsError_c;
    }

    /* BLE Host Tasks Init */
    if (osaStatus_Success != Ble_HostTaskInit())
    {
        return gBleOsError_c;
    }
    
    /* BLE Host Stack Init */
    return Ble_HostInitialize(gapGenericCallback, 
                (hciHostToControllerInterface_t) Hci_SendPacketToController);
    
#endif
}


/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/


/*! *********************************************************************************
* @}
********************************************************************************** */
