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


#include "bt_mw_a2dp_common.h"
#include "bt_mw_a2dp_snk.h"
#include "c_bt_mw_a2dp_snk.h"

#if 0
/* FUNCTION NAME: c_btm_a2dp_sink_register_player_cb
 * PURPOSE:
 *      register a player into BT MW, the A2DP streaming will playback in this
 *  player. There is a default alsa player in BT MW.
 * INPUT:
 *      func  -- the player callback function
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS  -- register successfully
 *      others      -- register fail
 * NOTES:
 *
 */
EXPORT_SYMBOL INT32 c_btm_a2dp_sink_register_player_cb(BT_A2DP_PLAYER *player)
{
    BT_CHECK_POINTER(BT_DEBUG_A2DP, player);

    return bt_mw_a2dp_sink_register_player(NULL, player);
}
#endif

/* FUNCTION NAME: c_btm_a2dp_sink_adjust_buf_time
 * PURPOSE:
 *      adjust the buffer time in player. After buffer some data and then start
 *  to play the streaming data. It is used to avoid choppy sound in bad
 *  environment.
 * INPUT:
 *      buffer_time  -- the buffer time, unit: ms
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS  -- set successfully
 *      others      -- set fail
 * NOTES:
 *
 */
EXPORT_SYMBOL INT32 c_btm_a2dp_sink_adjust_buf_time(UINT32 buffer_time)
{
    bt_mw_a2dp_sink_adjust_buffer_time( buffer_time);
    return BT_SUCCESS;
}

/* FUNCTION NAME: c_btm_a2dp_sink_get_dev_list
 * PURPOSE:
 *      it is used to get source device list which have connected before. The
 *  lastest device is the last one.
 * INPUT:
 *      N/A
 * OUTPUT:
 *      dev_list  -- the source device list
 * RETURN:
 *      BT_SUCCESS  -- get successfully
 *      others      -- get fail
 * NOTES:
 *
 */
EXPORT_SYMBOL INT32 c_btm_a2dp_sink_get_dev_list(BT_A2DP_DEVICE_LIST *dev_list)
{
    BT_CHECK_POINTER(BT_DEBUG_A2DP, dev_list);
    return bt_mw_a2dp_sink_get_dev_list(dev_list);
}


/* FUNCTION NAME: c_btm_a2dp_sink_enable
 * PURPOSE:
 *      enable A2DP sink function. Then it can connect to A2DP source device.
 * INPUT:
 *      enable  -- TRUE: enable A2DP sink function
 *                 FALSE: disable A2DP sink function
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS         -- set successfully
 *      BT_ERR_STATUS_BUSY -- there is connection, pending it.
*      others              -- set fail
 * NOTES:
 *      If there is no connection to source device, disable operation will
 * return with BT_SUCCESS.
 *      If there is connections to source device, disable will pending and BT MW
 * will disconnect all A2DP Source connections. When all A2DP source connectons
 * are disconnected, then BT MW will disable/enable sink role and report
 * BT_A2DP_EVENT_ROLE_CHANGED event to APP.
 *      Please set scan mode as 0 to protect disable operation. After this, APP
 * can restore scan mode.
 */
EXPORT_SYMBOL INT32 c_btm_a2dp_sink_enable(BOOL enable)
{
    return bt_mw_a2dp_sink_enable(enable);
}

/* FUNCTION NAME: c_btm_a2dp_sink_active_src
 * PURPOSE:
 *      active a specified SRC then it can play its streaming data.
 * INPUT:
 *      addr    -- connect to this device
 *      active  -- active or deactive, TRUE: active, FALSE: deactive
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS  -- acitve/deactive successfully
 *      BT_ERR_STATUS_NOT_READY -- a2dp role is not enable or power if off or
 *                                  role is changing or not connected.
 *      others      -- acitve/deactive fail.
 * NOTES:
 *      N/A
 */
EXPORT_SYMBOL INT32 c_btm_a2dp_sink_active_src(CHAR *addr)
{
    bt_mw_a2dp_sink_active_src(addr);
    return BT_SUCCESS;
}

EXPORT_SYMBOL INT32 c_btm_a2dp_sink_get_active_src(CHAR *addr)
{
    bt_mw_a2dp_get_active_device(addr);
    return BT_SUCCESS;
}

/* FUNCTION NAME: c_btm_a2dp_sink_set_delay_value
 * PURPOSE:
 *      set the delay value for a specified SRC; if not connected, input addr= NULL.
 * INPUT:
 *      addr    --  the device address or NULL
 *      value   --  the delay value that should be set, range about: 0~65535,unit 0.1ms
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS  -- successfully
 *      BT_ERR_STATUS_NOT_READY -- failed to get A2DP sink ex interface
 *      BT_ERR_STATUS_UNSUPPORTED  --  peer device not supported
 * NOTES:
 *      Only A2DP 1.3 version can use this API, and the local device can
 *      call this API only when support sink role.
 */
EXPORT_SYMBOL INT32 c_btm_a2dp_sink_set_delay_value(CHAR *addr, UINT16 value)
{
    FUNC_ENTRY;
    return bt_mw_a2dp_sink_set_delay_value(addr, value);
}

EXPORT_SYMBOL INT32 c_btm_a2dp_sink_lowpower_enable(BOOL enable)
{
    BT_MW_FUNC_ENTER(BT_DEBUG_A2DP, "enable = %d", enable);
    bt_mw_a2dp_sink_lowpower_enable(enable);
    return BT_SUCCESS;
}

EXPORT_SYMBOL INT32 c_btm_a2dp_sink_get_stack_delay(VOID)
{
    FUNC_ENTRY;
    return bt_mw_a2dp_sink_get_stack_delay();
}

