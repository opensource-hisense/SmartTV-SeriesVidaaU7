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

/* FILE NAME:  bt_mw_a2dp_common.h
 * PURPOSE:
 *  {1. What is covered in this file - function and scope.}
 *  {2. Related documents or hardware information}
 * NOTES:
 *  {Something must be known or noticed}
 *  {1. How to use these functions - Give an example.}
 *  {2. Sequence of messages if applicable.}
 *  {3. Any design limitation}
 *  {4. Any performance limitation}
 *  {5. Is it a reusable component}
 *
 *
 *
 */
#ifndef BT_MW_A2DP_COMMON_H
#define BT_MW_A2DP_COMMON_H
/* INCLUDE FILE DECLARATIONS
 */

//#include "bluetooth.h" //need refactor
#include "bt_mw_common.h"
#include "u_bt_mw_common.h"
#include "u_bt_mw_a2dp.h"

#if defined(MTK_LINUX_A2DP_PLUS) && (MTK_LINUX_A2DP_PLUS == TRUE)
#include "mtk_bt_av.h"
#else
//#include "bt_av.h" //need refactor
#endif

/* NAMING CONSTANT DECLARATIONS
 */

typedef enum
{
    BT_MW_A2DP_STREAM_STATUS_PAUSE,
    BT_MW_A2DP_STREAM_STATUS_PLAYING,
    BT_MW_A2DP_STREAM_STATUS_MAX
} BT_MW_A2DP_STREAM_STATUS;

typedef enum
{
  BT_MW_A2DP_ROLE_SRC,
  BT_MW_A2DP_ROLE_SINK,
  BT_MW_A2DP_ROLE_MAX
} BT_MW_A2DP_ROLE;

typedef enum {
  MW_BTA_AV_CODEC_TYPE_UNKNOWN = 0x00,
  MW_BTA_AV_CODEC_TYPE_SBC = 0x01,
  MW_BTA_AV_CODEC_TYPE_AAC = 0x02,
  MW_BTA_AV_CODEC_TYPE_APTX = 0x04,
  MW_BTA_AV_CODEC_TYPE_APTXHD = 0x08,
  MW_BTA_AV_CODEC_TYPE_LDAC = 0x10
} MW_BTA_AV_CODEC_TYPE;

enum
{
    /* device manager local device API events */
    BTMW_A2DP_CONNECTED = BTMW_EVT_START(BTWM_ID_A2DP),
    BTMW_A2DP_STREAM_START,
    BTMW_A2DP_STREAM_SUSPEND,
    BTMW_A2DP_STREAM_CONFIG,
    BTMW_A2DP_CODEC_CONFIG,
    BTMW_A2DP_DISCONNECTED,
    BTMW_A2DP_CONNECTING,
    BTMW_A2DP_DISCONNECTING,
    BTMW_A2DP_PLAYER_REPORT_EVENT,
    BTMW_A2DP_DELAY,
    BTMW_A2DP_NOTIFY_APP,
    BTMW_A2DP_MAX_EVT,
};  // event




/* MACRO FUNCTION DECLARATIONS
 */

#define BT_MW_A2DP_CASE_RETURN_STR(_const,str) case _const: return str

/* DATA TYPE DECLARATIONS
 */
typedef struct
{
    UINT8 codec_type;
    INT32 sample_rate;
    INT32 channel_num;
} BTMW_A2DP_MSG_CODEC_CONFIG;

/* DATA TYPE DECLARATIONS
 */
typedef struct
{
    UINT16 delay;
} BTMW_A2DP_MSG_DEALY;

typedef struct
{
    BT_MW_A2DP_ROLE role;
    CHAR addr[MAX_BDADDR_LEN];
    union
    {
        BT_A2DP_EVENT_AUDIO_CONFIG_DATA src_codec_config;
        BTMW_A2DP_MSG_CODEC_CONFIG sink_codec_config;
        BT_A2DP_CONF config;
        BTMW_A2DP_MSG_DEALY delay;
        BT_A2DP_PLAYER_EVENT player_event;
    }data;
}tBT_MW_A2DP_MSG;


/* EXPORTED SUBPROGRAM SPECIFICATIONS
 */


INT32 bt_mw_a2dp_connect(CHAR *addr, BT_MW_A2DP_ROLE role);
INT32 bt_mw_a2dp_disconnect(CHAR *addr);

INT32 bt_mw_a2dp_register_callback(BT_A2DP_EVENT_HANDLE_CB a2dp_handle);
INT32 bt_mw_a2dp_change_thread_priority(INT32 priority);
INT32 bt_mw_a2dp_codec_enable(BT_A2DP_CODEC_TYPE codec_type, BOOL enable);
INT32 bt_mw_a2dp_get_connected_dev_list(BT_A2DP_CONNECT_DEV_INFO_LIST *dev_list);
INT32 bt_mw_a2dp_src_set_channel_allocation_for_lrmode(CHAR *addr, INT32 channel);
INT32 bt_mw_a2dp_src_set_audiomode(INT32 AudioMode);

INT32 bt_mw_a2dp_sink_enable(BOOL enable);
INT32 bt_mw_a2dp_src_enable(BOOL enable, BT_A2DP_SRC_INIT_CONFIG* p_src_init_config);

INT32 bt_mw_a2dp_src_get_dev_list(BT_A2DP_DEVICE_LIST *dev_list);
INT32 bt_mw_a2dp_src_active_sink(CHAR *addr);
INT32 bt_mw_a2dp_get_active_device(CHAR *addr);
INT32 bt_mw_a2dp_src_set_silence_device(CHAR *addr, BOOL enable);
INT32 bt_mw_a2dp_src_config_codec_info(CHAR *addr, BT_A2DP_SET_CODEC_CONFIG *p_src_set_codec_config);
INT32 bt_mw_a2dp_sink_get_dev_list(BT_A2DP_DEVICE_LIST *dev_list);

INT32 bt_mw_a2dp_set_dbg_flag(BT_A2DP_DBG_FLAG flag, BT_A2DP_DBG_PARAM *param);

INT32 bt_mw_a2dp_sink_active_src(CHAR *addr);
INT32 bt_mw_a2dp_set_link_num(INT32 sink_num);

BOOL bt_mw_a2dp_is_connecting(void);

INT32 bt_mw_a2dp_dump_dev_info(VOID);
INT32 bt_mw_a2dp_sink_set_delay_value(CHAR *addr, UINT16 value);
INT32 bt_mw_a2dp_sink_lowpower_enable(BOOL enable);
BOOL bt_mw_a2dp_src_is_in_silence_mode(CHAR *addr);

#endif /* End of BT_MW_A2DP_COMMON_H */

