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
	//debug 2018.12.10 4:59PM for readdata so rewrite it
	//
	/* reset all slots for valid subscribers */
	    //for(mClientId = 0; mClientId < pServiceConfig->validSubscriberListSize; mClientId++)
	    //{
	    //    pServiceConfig->aValidSubscriberList[mClientId] = FALSE;
	   // }

	    /* Record initial battery level measurement */
		readdataMeasurement(pServiceConfig);
		readEcgReqMeasurement(pServiceConfig);
	    //


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

bleResult_t Qpp_SendData (uint8_t deviceId, uint16_t serviceHandle,uint16_t length, uint8_t *testData, uint8_t *testData2,uint8_t *testData3 )
{
    uint16_t  handle;
    bleResult_t result;
    uint16_t  handleCccd;
    //uint16_t  handleCccd2;//debug
    bool_t isNotifActive;
    //uint16_t  handle2; //debug 2018.12.4 9:52PM set another handle
    length=length;

    bleUuid_t uuid=Uuid16(ecgdata);

    //FLib_MemCpy(uuid.uuid16, writetime, 16);

    /* Get handle of  characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle, gBleUuidType16_c, &uuid, &handle);
    if (result != gBleSuccess_c)
        return result;
    /* Get handle of CCCD */
        if ((result = GattDb_FindCccdHandleForCharValueHandle(handle, &handleCccd)) != gBleSuccess_c)
            return result;


        result = Gap_CheckNotificationStatus(deviceId, handleCccd, &isNotifActive);
            if ((gBleSuccess_c == result) && (TRUE == isNotifActive))
                result = GattServer_SendInstantValueNotification(deviceId, handle, length, testData);
            	result = GattServer_SendInstantValueNotification(deviceId, handle, length, testData2);
            	result = GattServer_SendInstantValueNotification(deviceId, handle, length, testData3);


    //debug 2018.12.4 9:41PM get another handle
    //FLib_MemCpy(uuid.uuid128, uuid_qpps_characteristics_nx, 16);
    //result = GattDb_FindCharValueHandleInService(serviceHandle, gBleUuidType128_c, &uuid, &handle2);
       // if (result != gBleSuccess_c)
          //  return result;
        /* Get handle of CCCD */
   // if ((result = GattDb_FindCccdHandleForCharValueHandle(handle2, &handleCccd2)) != gBleSuccess_c)
      //  return result;
	
     // if ((gBleSuccess_c == result) && (TRUE == isNotifActive))
       // result = GattServer_SendInstantValueNotification(deviceId, handle2, length, testData); //debug 2018.12.4 9:43PM send another notify
    
    return result;
}

//debug 2018.12.7 send another notify
bleResult_t Qpp_SendData2 (uint8_t deviceId, uint16_t serviceHandle,uint16_t length, uint8_t *testData)
{
    uint16_t  handle;
    bleResult_t result;
    uint16_t  handleCccd;
   // uint16_t  handleCccd2;//debug
    bool_t isNotifActive;
   // uint16_t  handle2; //debug 2018.12.4 9:52PM set another handle


    bleUuid_t uuid=Uuid16(writetime);

    //FLib_MemCpy(uuid.uuid16, ecgdata, 16);

    /* Get handle of  characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle, gBleUuidType16_c, &uuid, &handle);
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
//debug 2018.12.7 5:05PM read data update
bleResult_t readdataMeasurement (qppsConfig_t* pServiceConfig)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid;
    //bleUuid_t uuid = Uuid16(gBleSig_BatteryLevel_d);

    FLib_MemCpy(uuid.uuid128, readtime, 16);

    /* Get handle of  characteristic */
    //result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,gBleUuidType16_c, &uuid, &handle);
     result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle, gBleUuidType128_c, &uuid, &handle);

    if (result != gBleSuccess_c)
        return result;

    /* Update characteristic value and send notification */
    result = GattDb_WriteAttribute(handle, sizeof(uint8_t), &pServiceConfig->valueone);

    if (result != gBleSuccess_c)
        return result;

    //Bas_SendNotifications(pServiceConfig, handle);

    return gBleSuccess_c;
}
//debug 2018.12.10 7:48 PM ECG Request data_read
bleResult_t readEcgReqMeasurement (qppsConfig_t* pServiceConfig)
{
    uint16_t  handle;
    bleResult_t result;
    bleUuid_t uuid;
    //bleUuid_t uuid = Uuid16(gBleSig_BatteryLevel_d);

    FLib_MemCpy(uuid.uuid128, reqdata, 16);

    /* Get handle of  characteristic */
    //result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,gBleUuidType16_c, &uuid, &handle);
     result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle, gBleUuidType128_c, &uuid, &handle);

    if (result != gBleSuccess_c)
        return result;

    /* Update characteristic value and send notification */
    result = GattDb_WriteAttribute(handle, sizeof(uint8_t), &pServiceConfig->valuetwo);

    if (result != gBleSuccess_c)
        return result;

    //Bas_SendNotifications(pServiceConfig, handle);

    return gBleSuccess_c;
}

