/*! *********************************************************************************
* \addtogroup Private Profile Service
* @{
********************************************************************************** */
/*!
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * \file private_profile_service.c
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
#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"
#include "private_profile_interface.h"
#include "SerialManager.h"
#include "gatt_db_handles.h"
/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/*! Battery Service - Subscribed Client*/
deviceId_t mQpp_SubscribedClientId;

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

bleResult_t Qpp_Start (qppsConfig_t *pServiceConfig)
{

    return gBleSuccess_c;
}

bleResult_t Qpp_Stop (qppsConfig_t *pServiceConfig)
{
    mQpp_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Qpp_Subscribe(deviceId_t clientDeviceId)
{
    mQpp_SubscribedClientId = clientDeviceId;
    return gBleSuccess_c;
}

bleResult_t Qpp_Unsubscribe()
{
    mQpp_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Qpp_SendData (uint8_t deviceId, uint16_t serviceHandle,uint16_t length, uint8_t *testData)
{
    uint16_t  handle;
    bleResult_t result;
    uint16_t  handleCccd;
    bool_t isNotifActive;
    
    bleUuid_t uuid;
    FLib_MemCpy(uuid.uuid128, uuid_qpps_characteristics_tx, 16);

    /* Get handle of  characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle, gBleUuidType128_c, &uuid, &handle);    
    if (result != gBleSuccess_c)
        return result;
        /* Get handle of CCCD */
    if ((result = GattDb_FindCccdHandleForCharValueHandle(handle, &handleCccd)) != gBleSuccess_c)
        return result;
	
    result = Gap_CheckNotificationStatus(deviceId, handleCccd, &isNotifActive);
    if ((gBleSuccess_c == result) && (TRUE == isNotifActive))
        result = GattServer_SendInstantValueNotification(deviceId, handle, length, testData); 
    
    return result;
}
