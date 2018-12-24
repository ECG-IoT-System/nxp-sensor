/*! *********************************************************************************
* \defgroup Private profile Service
* @{
********************************************************************************** */
/*!
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* All rights reserved.
*
* \file private_profile_interface.h
* This file is the interface file for the QPP Service
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

#ifndef _PRIVATE_PROFILE_INTERFACE_H_
#define _PRIVATE_PROFILE_INTERFACE_H_

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! QPP Service - Configuration */
typedef struct qppsConfig_tag
{
    uint16_t    serviceHandle;
    uint8_t     valueone ;  //debug 2018.12.5 2:00PM  read value
    uint8_t     valuetwo ;  //debug 2018.12.10 7:42PM read second value
    bool_t*     aValidSubscriberList;//debug 2018.12.9 8:57PM
    uint8_t     validSubscriberListSize;//debug 2018.12.9 8:57PM


} qppsConfig_t;


typedef struct tmcConfig_tag
{
    uint16_t    hService;
    uint16_t    hnxData; //debug 2018.12.4 for new GATT
    uint16_t    hTxData;
    uint16_t    hTxCccd; 
    uint16_t    hRxData;
} qppConfig_t;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public prototypes
************************************************************************************* 
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!**********************************************************************************
* \brief        Starts QPP Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Qpp_Start(qppsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops QPP Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Qpp_Stop(qppsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the QPP service
*
* \param[in]    pClient  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Qpp_Subscribe(deviceId_t clientDeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the QPP service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Qpp_Unsubscribe(void);

/*!**********************************************************************************
* \brief        Qpps SendData to Qppc. 
*
* \param[in]    deviceId        Peer device ID.
* \param[in]    serviceHandle   Service handle.
* \param[in]    length          Length of TestData to send .
* \param[in]    testData        TestData to send .
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Qpp_SendData (uint8_t deviceId, uint16_t serviceHandle, uint16_t length, uint8_t *testData);

#ifdef __cplusplus
}
#endif 

//debug 2018.12.7  4:25PM  declare for another notify
bleResult_t Qpp_SendData2 (uint8_t deviceId, uint16_t serviceHandle, uint16_t length, uint8_t *testData);

#ifdef __cplusplus
}
#endif



/*! *********************************************************************************
* @}
********************************************************************************** */

bleResult_t readdataMeasurement (qppsConfig_t* pServiceConfig);//debug 2018.12.7 5:06PM declare read function

#ifdef __cplusplus
}
#endif

//debug 2018.12.10 7:47PM declare read data
bleResult_t readEcgReqMeasurement (qppsConfig_t* pServiceConfig);

#ifdef __cplusplus
}
#endif

#endif /* _PRIVATE_PROFILE_INTERFACE_H_ */
