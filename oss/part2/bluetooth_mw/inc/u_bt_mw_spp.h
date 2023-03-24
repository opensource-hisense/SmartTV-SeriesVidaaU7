/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2016-2017. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */


/* FILE NAME:  u_bt_mw_spp.h
 * AUTHOR: Jigong Yin
 * PURPOSE:
 *      It provides bluetooth spp profile structure to APP.
 * NOTES:
 */
#ifndef _U_BT_MW_SPP_H_
#define _U_BT_MW_SPP_H_

#include "u_bt_mw_common.h"

/*----------------------------------------------------------------------
                    Enums and Macro declarations
----------------------------------------------------------------------*/
#define MAX_SPP_DATA_LEN 991
#define MAX_SPP_NAME_LEN 255
#define MAX_UUID_LEN     48
#define SPP_MAX_NUMBER  5

#define SPP_DEFAULT_UUID "00001101-0000-1000-8000-00805F9B34FB"

typedef enum
{
    BT_SPP_CONNECT = 0,
    BT_SPP_DISCONNECT = 1,
    BT_SPP_RECV_DATA = 2,
    BT_SPP_CONNECT_FAIL,
    BT_SPP_DISCONNECT_FAIL,
    BT_SPP_SEND_DATA,
    BT_SPP_MAX
}BT_SPP_EVENT;

/*----------------------------------------------------------------------
                    API parameter structure declarations
----------------------------------------------------------------------*/

typedef struct
{
    CHAR    bd_addr[MAX_BDADDR_LEN];
    UINT8   spp_if;
    CHAR    uuid[MAX_UUID_LEN];
    BOOL    is_used;
    BOOL    is_server;
}BT_SPP_CONNECTION_INFO;

typedef struct
{
    BT_SPP_CONNECTION_INFO spp_connection_info[SPP_MAX_NUMBER];
}BT_SPP_CONNECTION_INFO_DB;

typedef struct
{
    UINT8 spp_if;
    CHAR server_name[MAX_SPP_NAME_LEN];
    CHAR uuid[MAX_UUID_LEN];
}BT_SPP_START_SVR_PARAM;

typedef struct
{
    UINT8 spp_if;
    CHAR server_name[MAX_SPP_NAME_LEN];
    CHAR uuid[MAX_UUID_LEN];
}BT_SPP_STOP_SVR_PARAM;

typedef struct
{
    UINT8 spp_if;
    CHAR bd_addr[MAX_BDADDR_LEN];
    CHAR uuid[MAX_UUID_LEN];
}BT_SPP_CONNECT_PARAM;

typedef struct
{
    UINT8 spp_if;
    CHAR bd_addr[MAX_BDADDR_LEN];
    CHAR uuid[MAX_UUID_LEN];
}BT_SPP_DISCONNECT_PARAM;

typedef struct
{
    UINT8 spp_if;
    CHAR bd_addr[MAX_BDADDR_LEN];
    CHAR uuid[MAX_UUID_LEN];
    CHAR spp_data[MAX_SPP_DATA_LEN];
    UINT16 spp_data_len;
}BT_SPP_SEND_DATA_PARAM;

/*----------------------------------------------------------------------
                    Callback structure declarations
----------------------------------------------------------------------*/

typedef struct _BT_SPP_CBK_STRUCT
{
    CHAR bd_addr[MAX_BDADDR_LEN];
    CHAR uuid[MAX_UUID_LEN];
    UINT8 uuid_len;
    UINT8 spp_if;
    CHAR spp_data[MAX_SPP_DATA_LEN];
    UINT16 spp_data_len;
    BT_SPP_EVENT event;
}BT_SPP_CBK_STRUCT;

typedef VOID (*BtAppSppCbk)(BT_SPP_CBK_STRUCT *pt_spp_struct);

#endif /*  _U_BT_MW_SPP_H_ */

