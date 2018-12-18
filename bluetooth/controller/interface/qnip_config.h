/*
 * Copyright (c) 2016, NXP Semiconductors, N.V.
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
#ifndef _QNIP_CONFIG_H_
#define _QNIP_CONFIG_H_

/* Don't change following configuration */

#if defined(__ROM_FW)

/// System configuration: CFG_QN908XA, CFG_QN908XB
#if (!defined(CFG_QN908XA))
#define CFG_QN908XB
#endif

/// BLE configuration
#define CFG_BLE
#define CFG_EMB
//#define CFG_HOST
#define CFG_PRF
#define CFG_FW
#define CFG_AHITL
#define CFG_HCITL
#define CFG_ATTC
#define CFG_ATTS
#define CFG_EXT_DB
#define CFG_SEC_CON
#define CFG_SLEEP
#define CFG_CHNL_ASSESS
#define CFG_WLAN_COEX
#define CFG_NVDS
//#define CFG_PLF_DBG
//#define CFG_DBG
//#define CFG_DBG_MEM
//#define CFG_DBG_FLASH
//#define CFG_DBG_NVDS
//#define CFG_DBG_STACK_PROF
#define CFG_HW_AUDIO
#define CFG_BLE_2MBPS

#else

/// System configuration: CFG_QN908XA, CFG_QN908XB
#if (!defined(CFG_QN908XA))
#define CFG_QN908XB
#endif

/// BLE configuration
#define CFG_BLE
#define CFG_EMB
//#define CFG_HOST // GGG
//#define CFG_PRF // GGG
//#define CFG_APP // GGG
//#define CFG_AHITL // GGG
#define CFG_HCITL
//#define CFG_ATTC // GGG
//#define CFG_ATTS // GGG
//#define CFG_EXT_DB
#define CFG_SEC_CON
#define CFG_SLEEP
//#define CFG_CHNL_ASSESS // GGG
//#define CFG_WLAN_COEX // GGG
#define CFG_NVDS
//#define CFG_PLF_DBG // GGG
//#define CFG_DBG // GGG
//#define CFG_DBG_MEM // GGG
//#define CFG_DBG_FLASH
//#define CFG_DBG_NVDS // GGG
//#define CFG_DBG_STACK_PROF // GGG
//#define CFG_HW_AUDIO
//#define CFG_BLE_2MBPS // GGG

#define CFG_SIM
//#define CFG_SIM_ADV
//#define CFG_SIM_SCAN
//#define CFG_SIM_INIT
//#define CFG_SIM_DTM_TX
//#define CFG_SIM_DTM_RX
//#define CFG_SIM_LE_2MBPS
//#define CFG_LE_DATA_PACKET_LEN_EXT

#endif

#endif

