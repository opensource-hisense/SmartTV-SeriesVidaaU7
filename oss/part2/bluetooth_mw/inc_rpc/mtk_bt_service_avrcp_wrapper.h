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

/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE charGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/

#ifndef _MTK_BT_SERVICE_AVRCP_WRAPPER_H_
#define _MTK_BT_SERVICE_AVRCP_WRAPPER_H_

#include "u_rpcipc_types.h"
#include "u_bt_mw_avrcp.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef VOID (*mtkrpcapi_BtAppAvrcpCbk)(BT_AVRCP_EVENT_PARAM *param, VOID *pv_tag);

/* -----------------------------avrcp ct api -----------------------------------*/

/* FUNCTION NAME: a_mtkapi_avrcp_change_player_app_setting
 * PURPOSE:
 *      it's used to send command to the remote device to change palyer setting.
 * INPUT:
 *      addr        -- peer device address
 *      atrribute    -- inlcude number of attribute in attrs and attribute ID and BT_AVRCP_PLAYER_ATTR
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS --player app setting succesfully
 *      Others     -- setting failed
 * NOTES:
 *
 */
extern INT32 a_mtkapi_avrcp_change_player_app_setting(CHAR *addr,
    BT_AVRCP_PLAYER_SETTING *player_setting);


/* FUNCTION NAME: a_mtkapi_avrcp_send_passthrough_cmd
 * PURPOSE:
 *      it's used to send passthrough command to the remote device.
 * INPUT:
 *      addr        -- peer device address
 *      cmd_type    -- command type
 *      key_state   -- key state
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS -- send passthrough command succesfully
 *      BT_ERR_STATUS_NOT_READY -- AVRCP is not connected.
 *      Others     -- send failed
 * NOTES:
 *
 */
extern INT32 a_mtkapi_avrcp_send_passthrough_cmd(CHAR *addr,
    BT_AVRCP_CMD_TYPE cmd_type, BT_AVRCP_KEY_STATE key_state);

/* FUNCTION NAME: a_mtkapi_avrcp_send_vendor_unique_cmd
 * PURPOSE:
 *      it's used to send vendor unique command to the remote device.
 * INPUT:
 *      addr        -- peer device address
 *      key         -- vendor unique key
 *      key_state   -- key state
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS -- send vendor unique succesfully
 *      BT_ERR_STATUS_NOT_READY -- AVRCP is not connected.
 *      Others     -- send failed
 * NOTES:
 *
 */
extern INT32 a_mtkapi_avrcp_send_vendor_unique_cmd(CHAR *addr,
    UINT8 key, BT_AVRCP_KEY_STATE key_state);


/* FUNCTION NAME: a_mtkapi_avrcp_update_absolute_volume
 * PURPOSE:
 *      it's used to update absolute volume in the MW. When local volume changes
 * user should call this function to update BT MW absolute volume. It will
 * trigger VOLUME_CHANGED event if volume changes.
 * INPUT:
 *      abs_volume  -- local volume, unit: %, 0~100%
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS -- update succesfully
 *      Others     -- update failed
 * NOTES:
 *      This function can be called without AVRCP connection. When local volume
 * is changed, APP should call this function to update the volume in BT MW.
 */
extern INT32 a_mtkapi_avrcp_update_absolute_volume(const UINT8 abs_volume);

/* -----------------------------avrcp tg api -----------------------------------*/

/* FUNCTION NAME: a_mtkapi_avrcp_change_volume
 * PURPOSE:
 *      change the remote sink device's absolute volume
 * INPUT:
 *      addr        -- peer device address
 *      volume      -- the absolute volume, 0~100 => 0%~100%
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS -- set absolute volume succesfully
 *      BT_ERR_STATUS_UNSUPPORTED -- remote device don't support absolute volume
 *      BT_ERR_STATUS_NOT_READY -- AVRCP is not connected.
 *      Others     -- set failed
 * NOTES:
 *
 */
extern INT32 a_mtkapi_avrcp_change_volume(CHAR *addr, UINT8 volume);


/* FUNCTION NAME: a_mtkapi_avrcp_update_player_status
 * PURPOSE:
 *      it's used to update player status in the MW. When player status change
 *  BT WM will trigger PLAY_STATUS_CHANGED event or PLAYBACK_POS_CHANGED event.
 * INPUT:
 *      player_status  -- player status.
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS -- update succesfully
 *      Others     -- update failed
 * NOTES:
 *      This function can be called without AVRCP connection. When Player's
 *  status changes, user should call this function to update MW player status.
 */
extern INT32
a_mtkapi_avrcp_update_player_status(BT_AVRCP_PLAYER_STATUS *player_status);

/* FUNCTION NAME: a_mtkapi_avrcp_get_playback_state
 * PURPOSE:
 *      it's used to get playback state the TG avrcp
 *
 * INPUT:
 *      addr  -- -- peer device address
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS -- send cmd succesfully
 *      Others     -- update failed
 * NOTES:
 *
 */
extern INT32 a_mtkapi_avrcp_get_playback_state(CHAR* addr);

/* FUNCTION NAME: a_mtkapi_avrcp_send_playitem
 * PURPOSE:
 *      it's used to send command to the remote device to change palyer setting.
 * INPUT:
 *      addr        -- peer device address
 *      playitem    -- inlcude uid,scope and uid counter
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS --send playitem succesfully
 *      Others     -- send playitem failed
 * NOTES:
 *
 */
extern INT32 a_mtkapi_avrcp_send_playitem(CHAR *addr, BT_AVRCP_PLAYITEM *playitem);

/* FUNCTION NAME: a_mtkapi_avrcp_get_now_playinglist
 * PURPOSE:
 *      it's used to get now playing list from the remote device.
 * INPUT:
 *      addr        -- peer device address
 *      list_range    -- inlcude start and end
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS --send get now playinglist cmd succesfully
 *      Others     -- send get now playinglist cmd failed
 * NOTES:
 *
 */
extern INT32 a_mtkapi_avrcp_get_now_playing_list(CHAR *addr,
    BT_AVRCP_LIST_RANGE *list_range);

/* FUNCTION NAME: a_mtkapi_avrcp_get_folder_list
 * PURPOSE:
 *      it's used to get folder list from the remote device.
 * INPUT:
 *      addr        -- peer device address
 *      list_range    -- inlcude start and end
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS --send get folder list cmd succesfully
 *      Others     -- send get folder list cmd failed
 * NOTES:
 *
 */
extern INT32 a_mtkapi_avrcp_get_folder_list(CHAR *addr,
    BT_AVRCP_LIST_RANGE *list_range);

/* FUNCTION NAME: a_mtkapi_avrcp_get_player_list
 * PURPOSE:
 *      it's used to get player list from the remote device.
 * INPUT:
 *      addr        -- peer device address
 *      list_range    -- inlcude start and end
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS --send get player list cmd succesfully
 *      Others     -- send get player list cmd failed
 * NOTES:
 *
 */
extern INT32 a_mtkapi_avrcp_get_player_list(CHAR *addr,
    BT_AVRCP_LIST_RANGE *list_range);

/* FUNCTION NAME: a_mtkapi_avrcp_change_folder_path
 * PURPOSE:
 *      it's used to get player list from the remote device.
 * INPUT:
 *      addr        -- peer device address
 *      list_range    -- inlcude direct and uid
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS --send change folder path cmd succesfully
 *      Others     -- send get player list cmd failed
 * NOTES:
 *
 */
extern INT32 a_mtkapi_avrcp_change_folder_path(CHAR *addr,
    BT_AVRCP_FOLDER_PATH *folder_path);

/* FUNCTION NAME: a_mtkapi_avrcp_set_browsed_player
 * PURPOSE:
 *      it's used to set browsed player from the remote device.
 * INPUT:
 *      addr        -- peer device address
 *      list_range    -- inlcude direct and uid
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS -- set browsed player succesfully
 *      Others     --  set browsed player failed
 * NOTES:
 *
 */
extern INT32 a_mtkapi_avrcp_set_browsed_player(CHAR *addr, UINT16 playerId);

/* FUNCTION NAME: a_mtkapi_avrcp_set_addressed_player
 * PURPOSE:
 *      it's used to set addressed player from the remote device.
 * INPUT:
 *      addr        -- peer device address
 *      list_range    -- inlcude direct and uid
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS -- set addressed player succesfully
 *      Others     --  set addressed player failed
 * NOTES:
 *
 */
extern INT32 a_mtkapi_avrcp_set_addressed_player(CHAR *addr, UINT16 playerId);

/* FUNCTION NAME: a_mtkapi_avrcp_update_player_media_info
 * PURPOSE:
 *      it's used to update player media information in the MW. When player
 *  status change BT WM will trigger TRACK_CHANGED event. When the remote device
 *  get element attribute, BT WM will use this information to resposne it.
 * INPUT:
 *      player_media_info  -- player media information, like: songs title, ...
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS -- update succesfully
 *      Others     -- update failed
 * NOTES:
 *      This function can be called without AVRCP connection. When Player's
 *  media(song) changes, user should call this function to update MW player
 *  media information.
 */
extern INT32
a_mtkapi_avrcp_update_player_media_info(BT_AVRCP_PLAYER_MEDIA_INFO *player_media_info);

/* FUNCTION NAME: a_mtkapi_avrcp_register_callback
 * PURPOSE:
 *      it's used by user to register a AVRCP event callback. User can receive
 *  AVRCP event by this handler.
 * INPUT:
 *      avrcp_handle  -- AVRCP event handler implemented by user
 *                      NULL: disable AVRCP function
 *                      not NULL: enable AVRCP function.
 *      pv_tag        -- it will be passed to avrcp_handle.
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS -- register succesfully
 *      Others     -- register failed
 * NOTES:
 *
 */
extern INT32 a_mtkapi_avrcp_register_callback(mtkrpcapi_BtAppAvrcpCbk avrcp_handle, VOID* pv_tag);

/* FUNCTION NAME: a_mtkapi_avrcp_unregister_callback
 * PURPOSE:
 *      it's used by user to unregister a AVRCP event callback. User can receive
 *  AVRCP event by this handler.
 * INPUT:
 *      avrcp_handle  -- AVRCP event handler implemented by user
 *      pv_tag        -- it will be passed to avrcp_handle.
 * OUTPUT:
 *      N/A
 * RETURN:
 *      BT_SUCCESS -- unregister succesfully
 *      Others     -- unregister failed
 * NOTES:
 *
 */
extern INT32 a_mtkapi_avrcp_unregister_callback(mtkrpcapi_BtAppAvrcpCbk avrcp_handle, VOID* pv_tag);

/* FUNCTION NAME: a_mtkapi_avrcp_get_connected_dev_list
 * PURPOSE:
 *      it is used to get connected device list which is connected. .
 * INPUT:
 *      N/A
 * OUTPUT:
 *      dev_list  -- the connected device list
 * RETURN:
 *      BT_SUCCESS  -- get successfully
 *      others      -- get fail
 * NOTES:
 *
 */
extern INT32 a_mtkapi_avrcp_get_connected_dev_list(BT_AVRCP_CONNECTED_DEV_INFO_LIST *dev_list);

extern INT32 c_rpc_reg_mtk_bt_service_avrcp_cb_hndlrs(VOID);

#ifdef  __cplusplus
}
#endif
#endif
