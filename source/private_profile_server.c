/*! *********************************************************************************
* \addtogroup Private Profile Server
* @{
********************************************************************************** */
/*!
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
*
* \file
*
* This file is the source file for the QPP Server application
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
/* #undef CR_INTEGER_PRINTF to force the usage of the sprintf() function provided
 * by the compiler in this file. The sprintf() function is #included from
 * the <stdio.h> file. */
#ifdef CR_INTEGER_PRINTF
    #undef CR_INTEGER_PRINTF
#endif

/* Framework / Drivers */
#include "stdio.h"
#include "RNG_Interface.h"
#include "Keyboard.h"
#include "LED.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "SerialManager.h"
#include "MemManager.h"
#include "Panic.h"

/* BLE Host Stack */
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#include "gatt_db_handles.h"

/* Profile / Services */
#include "battery_interface.h"
#include "device_info_interface.h"
#include "private_profile_interface.h"

/* Connection Manager */
#include "ble_conn_manager.h"

#include "board.h"
#include "ApplMain.h"
#include "private_profile_server.h"




#include "test_adc.h"//debug
/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define mBatteryLevelReportInterval_c                (10)         /* battery level report interval in seconds  */
#define mQppsThroughputStatisticsInterval_c          (10000)       /* Throughput Statistics interval in miliseconds  */
#define mQppsTxInterval_c                            (100)         /* Qpps send data interval in miliseconds  */
#define mQppsTestDataLength                          (2*30)         /* the length of data that Qpps send every time*/
//debug1 20->120 2018.12.3 11:56PM
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef enum
{
#if gAppUseBonding_d
    fastWhiteListAdvState_c,
#endif
    fastAdvState_c,
    slowAdvState_c
}advType_t;

typedef struct advState_tag{
    bool_t      advOn;
    advType_t   advType;
}advState_t;

typedef struct appPeerInfo_tag
{
    uint8_t deviceId;
    uint8_t ntf_cfg;
    uint64_t bytsSentPerInterval;
    uint64_t bytsReceivedPerInterval;
}appPeerInfo_t;

typedef struct appTxInfo_tag
{
	uint32_t TakenSeconds;
	uint32_t RxSpeed[gAppMaxConnections_c];
	uint32_t TxSpeed[gAppMaxConnections_c];
}txInfo_t;

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
uint8_t gAppSerMgrIf;
/* Adv State */
static advState_t  mAdvState;
static bool_t      mRestartAdv;
static uint32_t    mAdvTimeout;
/* Service Data*/
static bool_t           basValidClientList[gAppMaxConnections_c] = { FALSE };
static basConfig_t      basServiceConfig = {service_battery, 2, basValidClientList, gAppMaxConnections_c};
static disConfig_t disServiceConfig = {service_device_info};
static qppsConfig_t qppServiceConfig = {service_qpps,1,1,basValidClientList, gAppMaxConnections_c};

//static uint16_t cpHandles[1] = {value_qpps_rx};

/* Application specific data*/

static txInfo_t mTxInfo;

static tmrTimerID_t mAdvTimerId;
static tmrTimerID_t mBatteryMeasurementTimerId;
static tmrTimerID_t mQppsThroughputStatisticsTimerId;
static tmrTimerID_t mQppsTxTimerId;
static appPeerInfo_t mPeerInformation[gAppMaxConnections_c];
static uint8_t printBuffer[100];


/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/* Gatt and Att callbacks */
static void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent);
static void BleApp_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent);
static void BleApp_GattServerCallback (deviceId_t deviceId, gattServerEvent_t* pServerEvent);
static void BleApp_Config(void);

/* Timer Callbacks */
static void AdvertisingTimerCallback (void *);
static void BatteryMeasurementTimerCallback (void *);
static void QppsThoughputStatisticsTimerCallback(void* pParam);
static void QppsTxTimerCallback(void* pParam);
static void QppsTxTimerCallback2(void* pParam);//debug
static void readdataTimerCallback (void *);//debug 2018.12.7 5:29PM declare readdata function

static void BleApp_Advertise(void);

static void BleApp_ReceivedDataHandler
(
    deviceId_t  deviceId,
    uint8_t*    aValue,
    uint16_t    valueLength
);

static void TxPrintCallback(void* pParam);

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief    Initializes application specific functionality before the BLE stack init.
*
********************************************************************************** */
void BleApp_Init(void)
{
    /* Initialize application support for drivers */
    BOARD_InitAdc();
    
        /* UI */
    SerialManager_Init();

    /* Register Serial Manager interface */
    Serial_InitInterface(&gAppSerMgrIf, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);

    Serial_SetBaudRate(gAppSerMgrIf, gUARTBaudRate115200_c);
}

/*! *********************************************************************************
* \brief    Starts the BLE application.
*
********************************************************************************** */
void BleApp_Start(void)
{
#if gAppUseBonding_d
    if (gcBondedDevices > 0)
    {
        mAdvState.advType = fastWhiteListAdvState_c;
    }
    else
    {
#endif
        mAdvState.advType = fastAdvState_c;
#if gAppUseBonding_d
    }
#endif

    BleApp_Advertise();
}

/*! *********************************************************************************
* \brief        Handles keyboard events.
*
* \param[in]    events    Key event structure.
********************************************************************************** */
void BleApp_HandleKeys(key_event_t events)
{ 
    switch (events)
    {
        uint8_t i;
        case gKBD_EventPressPB1_c:
        {
            for (i = 0; i < gAppMaxConnections_c; i++)
            {
              if (mPeerInformation[i].deviceId == gInvalidDeviceId_c)
                break;
            }
            if(i < gAppMaxConnections_c)
              BleApp_Start();
            break;
        }
        case gKBD_EventLongPB1_c:
        {
            for (i = 0; i < gAppMaxConnections_c; i++)
            {
              if (mPeerInformation[i].deviceId != gInvalidDeviceId_c)
                Gap_Disconnect(mPeerInformation[i].deviceId);
            }
            break;
        }
        case gKBD_EventLongPB2_c:
        {
            break;
        }
        default:
            break;
    }
}

/*! *********************************************************************************
* \brief        Handles BLE generic callback.
*
* \param[in]    pGenericEvent    Pointer to gapGenericEvent_t.
********************************************************************************** */
void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent)
{
    /* Call BLE Conn Manager */
    BleConnManager_GenericEvent(pGenericEvent);
    
    switch (pGenericEvent->eventType)
    {
        case gInitializationComplete_c:    
        {
            BleApp_Config();
        }
        break;
        
        case gAdvertisingParametersSetupComplete_c:
        {
            App_StartAdvertising(BleApp_AdvertisingCallback, BleApp_ConnectionCallback);
        }
        break;         

        default: 
            break;
    }
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief        Configures BLE Stack after initialization. Usually used for
*               configuring advertising, scanning, white list, services, et al.
*
********************************************************************************** */
static void BleApp_Config()
{
    /* Configure as GAP peripheral */
    BleConnManager_GapPeripheralConfig();

    /* Register for callbacks*/
    //GattServer_RegisterHandlesForWriteNotifications(NumberOfElements(cpHandles), cpHandles);
    App_RegisterGattServerCallback(BleApp_GattServerCallback);

    mAdvState.advOn = FALSE;
    for (uint8_t i = 0; i < gAppMaxConnections_c; i++)
    {
        mPeerInformation[i].deviceId= gInvalidDeviceId_c;
    }

    //basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    qppServiceConfig.valueone = 5;//read time
    qppServiceConfig.valuetwo = 0x11;//read ecg req
    Bas_Start(&basServiceConfig);
    Dis_Start(&disServiceConfig);
    Qpp_Start (&qppServiceConfig);
    /* Allocate application timers */
    mAdvTimerId = TMR_AllocateTimer();
    mBatteryMeasurementTimerId = TMR_AllocateTimer();
    mQppsThroughputStatisticsTimerId = TMR_AllocateTimer();
    mQppsTxTimerId =  TMR_AllocateTimer();
}

/*! *********************************************************************************
* \brief        Configures GAP Advertise parameters. Advertise will start after
*               the parameters are set.
*
********************************************************************************** */
static void BleApp_Advertise(void)
{
    switch (mAdvState.advType)
    {
#if gAppUseBonding_d
        case fastWhiteListAdvState_c:
        {
            gAdvParams.minInterval = gFastConnMinAdvInterval_c;
            gAdvParams.maxInterval = gFastConnMaxAdvInterval_c;
            gAdvParams.filterPolicy = gProcessWhiteListOnly_c;
            mAdvTimeout = gFastConnWhiteListAdvTime_c;
        }
        break;
#endif
        case fastAdvState_c:
        {
            gAdvParams.minInterval = gFastConnMinAdvInterval_c;
            gAdvParams.maxInterval = gFastConnMaxAdvInterval_c;
            gAdvParams.filterPolicy = gProcessAll_c;
            mAdvTimeout = gFastConnAdvTime_c - gFastConnWhiteListAdvTime_c;
        }
        break;

        case slowAdvState_c:
        {
            gAdvParams.minInterval = gReducedPowerMinAdvInterval_c;
            gAdvParams.maxInterval = gReducedPowerMinAdvInterval_c;
            gAdvParams.filterPolicy = gProcessAll_c;
            mAdvTimeout = gReducedPowerAdvTime_c;
        }
        break;
    }

    /* Set advertising parameters*/
    Gap_SetAdvertisingParameters(&gAdvParams);
}

/*! *********************************************************************************
* \brief        Handles BLE Advertising callback from host stack.
*
* \param[in]    pAdvertisingEvent    Pointer to gapAdvertisingEvent_t.
********************************************************************************** */
static void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent)
{
    switch (pAdvertisingEvent->eventType)
    {
        case gAdvertisingStateChanged_c:
        {
            mAdvState.advOn = !mAdvState.advOn;
            
            if (!mAdvState.advOn && mRestartAdv)
            {
                BleApp_Advertise();
                break;
            }                

            LED_StopFlashingAllLeds();
            Led1Flashing();

            if(!mAdvState.advOn)
            {
                Led2Flashing();
                Led3Flashing();
                Led4Flashing();
            }
            else
            {
                TMR_StartLowPowerTimer(mAdvTimerId,gTmrLowPowerSecondTimer_c,
                           TmrSeconds(mAdvTimeout), AdvertisingTimerCallback, NULL);  
            }
        }
        break;

        case gAdvertisingCommandFailed_c:
        {
            panic(0,0,0,0);
        }
        break;

        default:
            break;
    }
}

/*! *********************************************************************************
* \brief        Handles BLE Connection callback from host stack.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    pConnectionEvent    Pointer to gapConnectionEvent_t.
********************************************************************************** */
static void BleApp_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent)
{
	/* Connection Manager to handle Host Stack interactions */
	BleConnManager_GapPeripheralEvent(peerDeviceId, pConnectionEvent);

    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
            mPeerInformation[peerDeviceId].deviceId = peerDeviceId;
            Serial_Print(gAppSerMgrIf,"Connected with peerDeviceId = 0x",gAllowToBlock_d);
            Serial_PrintHex(gAppSerMgrIf, &peerDeviceId, 1, gAllowToBlock_d);
            Serial_Print(gAppSerMgrIf, "\r\n", gAllowToBlock_d);
            /* Advertising stops when connected */
            mAdvState.advOn = FALSE;            
        
            /* Subscribe client*/
            Bas_Subscribe(&basServiceConfig, peerDeviceId); 
			Qpp_Subscribe(peerDeviceId);
            /* UI */            
            LED_StopFlashingAllLeds();
            Led1On();                  
            /* Stop Advertising Timer*/
            mAdvState.advOn = FALSE;
            TMR_StopTimer(mAdvTimerId);
          
            /* Start battery measurements */
            if(!TMR_IsTimerActive(mBatteryMeasurementTimerId))
            {
                TMR_StartLowPowerTimer(mBatteryMeasurementTimerId, gTmrLowPowerIntervalMillisTimer_c,
                           TmrSeconds(mBatteryLevelReportInterval_c), BatteryMeasurementTimerCallback, NULL);
            }
            if(!TMR_IsTimerActive(mQppsThroughputStatisticsTimerId))
            {
                TMR_StartLowPowerTimer(mQppsThroughputStatisticsTimerId, gTmrLowPowerIntervalMillisTimer_c,
                           mQppsThroughputStatisticsInterval_c, QppsThoughputStatisticsTimerCallback, NULL);   
            }
            if(!TMR_IsTimerActive(mQppsTxTimerId))
            {
		TMR_StartLowPowerTimer(mQppsTxTimerId, gTmrLowPowerIntervalMillisTimer_c,
                           mQppsTxInterval_c, QppsTxTimerCallback, NULL);   
            }
        }
        break;
        
        case gConnEvtDisconnected_c:
        {
            /* Unsubscribe client */
            Bas_Unsubscribe(&basServiceConfig, peerDeviceId);
            /* qpps Unsubscribe client */
            Qpp_Unsubscribe();
            mPeerInformation[peerDeviceId].bytsReceivedPerInterval = 0;
            mPeerInformation[peerDeviceId].bytsSentPerInterval = 0;
            mPeerInformation[peerDeviceId].ntf_cfg = QPPS_VALUE_NTF_OFF;
            mPeerInformation[peerDeviceId].deviceId = gInvalidDeviceId_c;
            
            for (uint8_t i = 0; i < gAppMaxConnections_c; i++)
            {
                if(mPeerInformation[i].deviceId != gInvalidDeviceId_c)
                    break;
                if(i==(gAppMaxConnections_c-1))
                {
                    TMR_StopTimer(mBatteryMeasurementTimerId);      
                    TMR_StopTimer(mQppsThroughputStatisticsTimerId);
                    TMR_StopTimer(mQppsTxTimerId);   
                }
            }

            if (pConnectionEvent->eventData.disconnectedEvent.reason == gHciConnectionTimeout_c)
            {
                /* Link loss detected*/
                BleApp_Start();
            }
            else
            {
              /* Connection was terminated by peer or application */
                BleApp_Start();
            }
        }
        break;
    default:
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles GATT server callback from host stack.
*
* \param[in]    deviceId        Peer device ID.
* \param[in]    pServerEvent    Pointer to gattServerEvent_t.
********************************************************************************** */


static void BleApp_GattServerCallback (deviceId_t deviceId, gattServerEvent_t* pServerEvent)
{
    uint16_t handle;
    uint8_t status;
    switch (pServerEvent->eventType)
    {
        case gEvtAttributeWritten_c:
        {
            handle = pServerEvent->eventData.attributeWrittenEvent.handle;
           status = gAttErrCodeNoError_c;
           GattServer_SendAttributeWrittenStatus(deviceId, handle, status);
        }
        break;
       // case gEvtAttributeRead_c:
         //      {
           //        handle = pServerEvent->eventData.attributeReadEvent.handle;
             //      status = gAttErrCodeNoError_c;
                  // GattServer_SendAttributeWrittenStatus(deviceId, handle, status);
             //      BleApp_ReceivedDataHandler(deviceId, pServerEvent->eventData.attributeReadEvent.aValue, pServerEvent->eventData.attributeReadEvent.cValueLength);
            //   }
       // break;
        
        //case gEvtAttributeRead_c:
       // {
         //    handle = pServerEvent->eventData.attributeWrittenEvent.handle;

           //   if (handle == value_qpps_nx)
             //   {
               //     BleApp_ReceivedDataHandler(deviceId, pServerEvent->eventData.attributeWrittenEvent.aValue, pServerEvent->eventData.attributeWrittenEvent.cValueLength);
             //  }
       //  }
       //  break;

        case gEvtAttributeWrittenWithoutResponse_c:
        {
            handle = pServerEvent->eventData.attributeWrittenEvent.handle;
            
           // if (handle == value_qpps_rx)
            //{
             //   BleApp_ReceivedDataHandler(deviceId, pServerEvent->eventData.attributeWrittenEvent.aValue, pServerEvent->eventData.attributeWrittenEvent.cValueLength);
           // }
        }
        break;
        
        case gEvtCharacteristicCccdWritten_c:
        {
            handle = pServerEvent->eventData.attributeWrittenEvent.handle;
            if (handle == cccd_qpps_tx )
            {
                mPeerInformation[deviceId].ntf_cfg = pServerEvent->eventData.charCccdWrittenEvent.newCccd;
            }
            //debug 2018.12.7 11:38AM another notify for cccd
            if (handle == cccd_qpps_nx )
            {
                mPeerInformation[deviceId].ntf_cfg = pServerEvent->eventData.charCccdWrittenEvent.newCccd;
            }
        }
        break;
        
    default:	
        break;
    }
}


static void BleApp_ReceivedDataHandler
(
    deviceId_t deviceId,
    uint8_t*    aValue,
    uint16_t    valueLength
)
{
    mPeerInformation[deviceId].bytsReceivedPerInterval = mPeerInformation[deviceId].bytsReceivedPerInterval + valueLength;
}


/*! *********************************************************************************
* \brief        Handles advertising timer callback.
*
* \param[in]    pParam        Calback parameters.
********************************************************************************** */
static void AdvertisingTimerCallback(void * pParam)
{
    /* Stop and restart advertising with new parameters */
    Gap_StopAdvertising();

    switch (mAdvState.advType)
    {
#if gAppUseBonding_d
        case fastWhiteListAdvState_c:
        {
            mAdvState.advType = fastAdvState_c;
            mRestartAdv = TRUE;
        }
        break;
#endif
        case fastAdvState_c:
        {
            mAdvState.advType = slowAdvState_c;
            mRestartAdv = TRUE;
        }
        break;

        default:
        {
            mRestartAdv = FALSE;
        }
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles QPP tx timer callback.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
//debug-> put notify data
static void QppsTxTimerCallback(void * pParam)
{
      static uint8_t index = 0;
      uint8_t i;
      uint8_t length = mQppsTestDataLength + 8;


      uint8_t tx_data[length];//package1
      uint8_t tx_datat2[length];//package2
      uint8_t tx_datat3[length];//package3

      uint8_t tx_data2;
      uint8_t *testtry;
      uint8_t test[2];
      bleResult_t result;

      uint8_t * adc_data;
      adc_data = adc_conv();

      for(i = 0; i<8; i++)
      {
    	  tx_data[i]=0;
    	  tx_datat2[i]=0;
		  tx_datat3[i]=0;
      }

      for(i = 8; i<length; i++)
      {
    	  //tx_data[i] = 1;
    	  tx_data[i] = adc_data[i-8];//package1
    	  tx_datat2[i] = adc_data[i+length-16];//package2
		  tx_datat3[i] = adc_data[i+(2*length)-24];//package3

      }


      for (i = 0; i < gAppMaxConnections_c; i++)
      {
          if((mPeerInformation[i].deviceId != gInvalidDeviceId_c)&&(mPeerInformation[i].ntf_cfg == QPPS_VALUE_NTF_ON))
          {
              result = Qpp_SendData(mPeerInformation[i].deviceId, service_qpps, length, tx_data, tx_datat2,tx_datat3);//debug 2018.12.5 read value
              if(result == gBleSuccess_c)
                  mPeerInformation[i].bytsSentPerInterval += length;
          }
      }
      index++;

      //debug 2018.12.7 4:21PM send another notify
      //get ECG req
      tx_data2=0x05;

      for (i = 0; i < gAppMaxConnections_c; i++)
            {
                if((mPeerInformation[i].deviceId != gInvalidDeviceId_c)&&(mPeerInformation[i].ntf_cfg == QPPS_VALUE_NTF_ON))
                {
                    //result = Qpp_SendData2(mPeerInformation[i].deviceId, service_qpps, 2, tx_data2);
                    //if(result == gBleSuccess_c)
                        //mPeerInformation[i].bytsSentPerInterval += length;
                }
            }
            index++;

}



/*! *********************************************************************************
* \brief        Handles QPPS Thoughput timer callback.
*
* \param[in]    pParam        Calback parameters.
********************************************************************************** */

//debug send in different intervel (test)
static void QppsThoughputStatisticsTimerCallback(void* pParam)
{
    uint8_t i;
    mTxInfo.TakenSeconds = mQppsThroughputStatisticsInterval_c/1000;

    for (i = 0; i < gAppMaxConnections_c; i++)
    {
        if(mPeerInformation[i].deviceId != gInvalidDeviceId_c)
        {
        	mTxInfo.RxSpeed[i] = (mPeerInformation[i].bytsReceivedPerInterval/mTxInfo.TakenSeconds) * 8;
        	mTxInfo.TxSpeed[i] = (mPeerInformation[i].bytsSentPerInterval/mTxInfo.TakenSeconds) * 8;
            mPeerInformation[i].bytsReceivedPerInterval = 0;
            mPeerInformation[i].bytsSentPerInterval = 0;

            App_PostCallbackMessage(TxPrintCallback, NULL);
        }	
    }

}

//test
static void QppsTxTimerCallback2(void * pParam)
{
      static uint8_t index = 0;
      uint8_t i;
      uint8_t length = mQppsTestDataLength;
      length=60;
      uint8_t tx_data[length];
      uint8_t tx_datat2[length];
      uint8_t tx_datat3[length];
      //uint8_t tx_data[mQppsTestDataLength];
      //uint8_t tx_datat2[mQppsTestDataLength];
      //uint8_t tx_datat3[mQppsTestDataLength];
      uint8_t tx_data2;
      uint8_t *testtry;
      uint8_t test[2];
      bleResult_t result;
      //get ECG data

      //for(i = 0; i<length; i++)
      //{
        //  tx_data[i] = Getadc();//debug2 i->3 2018.12.3
      //}
      //tx_data[0] = index;

      uint8_t * adc_data;
      adc_data = adc_conv();

      for(i = 0; i<length; i++)
            {
    	  	  tx_data[i] = 2;
            }
      for(i = 0; i<length; i++)
             {
    	  	  tx_datat2[i] = 3;
             }
      for(i = 0; i<length; i++)
              {
         	   tx_datat3[i] = 4;
              }

      for (i = 0; i < gAppMaxConnections_c; i++)
      {
          if((mPeerInformation[i].deviceId != gInvalidDeviceId_c)&&(mPeerInformation[i].ntf_cfg == QPPS_VALUE_NTF_ON))
          {
             // result = Qpp_SendData(mPeerInformation[i].deviceId, service_qpps, length, tx_data2);//debug 2018.12.5 read value
              //if(result == gBleSuccess_c)
                  //mPeerInformation[i].bytsSentPerInterval += length;
          }
      }
}

static void TxPrintCallback(void * pParam)
{
	uint8_t i;

	for (i = 0; i < gAppMaxConnections_c; i++)
	{
		if(mPeerInformation[i].deviceId != gInvalidDeviceId_c)
		{
	        sprintf((char*)printBuffer, "\r\n-->QPP server, deviceId = 0x%x,RX speed =%d bps,TX speed =%d bps.\r\n ",mPeerInformation[i].deviceId,mTxInfo.RxSpeed[i],mTxInfo.TxSpeed[i]);
            Serial_Print(gAppSerMgrIf, (char*)printBuffer, gAllowToBlock_d);
		}
	}
}

/*! *********************************************************************************
* \brief        Handles battery measurement timer callback.
*
* \param[in]    pParam        Calback parameters.
********************************************************************************** */
static void BatteryMeasurementTimerCallback(void * pParam)
{
	basServiceConfig.batteryLevel = 4;
	//basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    Bas_RecordBatteryMeasurement(&basServiceConfig);
}

/*! *********************************************************************************
* @}
********************************************************************************** */
