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


/* FILE NAME:  bt_mw_hid.c
 * AUTHOR: zwei chen
 * PURPOSE:
 *      It provides hid  interface to c_bt_mw_hid.c.
 * NOTES:
 */


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

//#include "bluetooth.h" //need refactor
#include "bt_mw_common.h"
#include "bt_mw_hidh.h"
//#include "bluetooth_sync.h"
#include "linuxbt_hid_if.h"
#include "linuxbt_gap_if.h"
//#include "linuxbt_common.h" //need refactor
#include "bt_mw_gap.h"
#include "bt_mw_message_queue.h"


BtAppHidhCbk   HidhCbk = NULL;
static BTMW_HID_CONNECTED_DEVICE hid_connected_devices[MAX_HID_DEV_NUM];


/****************************************************
 * FUNCTION NAME: bt_mw_hidh_msg_handle
 * PURPOSE:
 *      The function is used for handle mw thread hidh event
 */
static VOID bt_mw_hidh_msg_handle(tBTMW_MSG *p_msg);

static VOID bt_mw_hidh_nty_handle(tBTMW_MSG *p_msg);

static INT32 bt_mw_hidh_find_device(CHAR *bd_addr, UINT8 *index);

/****************************************************
 * FUNCTION NAME: bt_hidh_init_profile
 * PURPOSE:
 *      The function is used to init hid profile
 * INPUT:
 *      None
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
void bt_hidh_init_profile(void)
{
    FUNC_ENTRY;
    linuxbt_hid_init();
}


/****************************************************
 * FUNCTION NAME: bluetooth_hid_deinit
 * PURPOSE:
 *      The function is used to deinit hid profile
 * INPUT:
 *      None
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
void bt_hidh_deinit_profile(void)
{
    FUNC_ENTRY;
    linuxbt_hid_deinit();
}


/****************************************************
 * FUNCTION NAME: bluetooth_hidh_init
 * PURPOSE:
 *      The function is used to init hid profile
 * INPUT:
 *      None
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hidh_init(BtAppHidhCbk hidhcbk)
{
    FUNC_ENTRY;
    profile_operator_t hidh_op = {0};

    BT_DBG_NORMAL(BT_DEBUG_HID, "bluetooth_hidh_init : hidhcbk =%p", hidhcbk);
    HidhCbk = hidhcbk;
    linuxbt_hdl_register(BTWM_ID_HIDH, bt_mw_hidh_msg_handle);
    bt_mw_nty_hdl_register(BTWM_ID_HIDH, bt_mw_hidh_nty_handle);

    hidh_op.init = bt_hidh_init_profile;
    hidh_op.deinit = bt_hidh_deinit_profile;
    hidh_op.notify_acl_state = NULL;
    hidh_op.facotry_reset = NULL;

    bt_mw_gap_register_profile(BTWM_ID_HIDH, &hidh_op);
    return BT_SUCCESS;
}


/****************************************************
 * FUNCTION NAME: btaudio_handle_hid_disconnect_status_check
 * PURPOSE:
 *      The function is used to check disconnet status
 * INPUT:
 *      i4device_index: first or second
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 btaudio_handle_hid_disconnect_status_check(INT32 i4device_index)
{
    FUNC_ENTRY;
/*
    UINT32 ui4_loop = 10;
    if (i4device_index >= MAX_HID_DEV_NUM)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "input index Error:%d.", i4device_index);
        return BT_ERR_STATUS_FAIL;
    }

    BT_CHECK_POINTER(BT_DEBUG_HID, g_pt_hid_status_list);

    do
    {
        BT_DBG_NORMAL(BT_DEBUG_HID, "ui4_loop:%ld, hid_connect_status:%d!",
            (long)ui4_loop, g_pt_hid_status_list->device_list[i4device_index].status);
        usleep(500*1000);
        ui4_loop --;
    }
    while (g_pt_hid_status_list->device_list[i4device_index].status && (0 < ui4_loop));

    if (g_pt_hid_status_list->device_list[i4device_index].status)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "can't disconnect hid device:%s, status:%d.",
            g_pt_hid_status_list->device_list[i4device_index].bdAddr,
            g_pt_hid_status_list->device_list[i4device_index].status);
        return BT_ERR_STATUS_FAIL;
    }
*/
    return BT_SUCCESS;
}


/****************************************************
 * FUNCTION NAME: bluetooth_hid_connect
 * PURPOSE:
 *      The function is used to connect  hid device
 * INPUT:
 *      pbt_addr:device address
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_connect(CHAR *pbt_addr)
{
    FUNC_ENTRY;
    return linuxbt_hid_connect_int_handler(pbt_addr);
}



/****************************************************
 * FUNCTION NAME: bluetooth_hid_disconnect
 * PURPOSE:
 *      The function is used to disconnect  hid device
 * INPUT:
 *      pbt_addr:device address
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_disconnect(CHAR *pbt_addr)
{
    FUNC_ENTRY;
    INT32 ret = BT_SUCCESS;

    BT_DBG_INFO(BT_DEBUG_HID, "pbt_addr:%s", pbt_addr);
    ret = linuxbt_hid_disconnect_handler(pbt_addr);

    return ret;
}


/****************************************************
 * FUNCTION NAME: bluetooth_hid_set_output_report
 * PURPOSE:
 *      The function is used to set output report
 * INPUT:
 *      pbt_addr:device address
 *      preport_data: output report data
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_set_output_report(char *pbt_addr, char *preport_data)
{
    FUNC_ENTRY;
    return linuxbt_hid_set_output_report_handler(pbt_addr, preport_data);
}


/****************************************************
 * FUNCTION NAME: bluetooth_hid_get_input_report
 * PURPOSE:
 *      The function is used to get input report
 * INPUT:
 *      pbt_addr:device address
 *      reportId: report id
 *     bufferSize : size of the buffer for input data
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_get_input_report(char *pbt_addr, int reportId, int bufferSize)
{
    FUNC_ENTRY;
    return linuxbt_hid_get_input_report_handler(pbt_addr, reportId, bufferSize);
}

/****************************************************
 * FUNCTION NAME: bluetooth_hid_get_output_report
 * PURPOSE:
 *      The function is used to get output report
 * INPUT:
 *      pbt_addr:device address
 *      reportId: report id
 *     bufferSize : size of the buffer for output data
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_get_output_report(char *pbt_addr, int reportId, int bufferSize)
{
    FUNC_ENTRY;
    return linuxbt_hid_get_output_report_handler(pbt_addr, reportId, bufferSize);
}

/****************************************************
 * FUNCTION NAME: bluetooth_hid_set_input_report
 * PURPOSE:
 *      The function is used to set input report
 * INPUT:
 *      pbt_addr:device address
 *      preport_data: input report data
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_set_input_report(char *pbt_addr, char *preport_data)
{
    FUNC_ENTRY;
    return linuxbt_hid_set_input_report_handler(pbt_addr, preport_data);
}

/****************************************************
 * FUNCTION NAME: linuxbt_hid_get_feature_report_handler
 * PURPOSE:
 *      The function is used to get feature report
 * INPUT:
 *      pbt_addr:device address
 *      reportId: report id
 *     bufferSize : size of the buffer for feature data
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_get_feature_report(char *pbt_addr, int reportId, int bufferSize)
{
    FUNC_ENTRY;
    return linuxbt_hid_get_feature_report_handler(pbt_addr, reportId, bufferSize);
}

/****************************************************
 * FUNCTION NAME: bluetooth_hid_set_feature_report
 * PURPOSE:
 *      The function is used to set feature report
 * INPUT:
 *      pbt_addr:device address
 *      preport_data: input report data
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_set_feature_report(char *pbt_addr, char *preport_data)
{
    FUNC_ENTRY;
    return linuxbt_hid_set_feature_report_handler(pbt_addr, preport_data);
}

/****************************************************
 * FUNCTION NAME: bluetooth_hid_get_protocol
 * PURPOSE:
 *      The function is used to get  protocol
 * INPUT:
 *      pbt_addr:device address
 *
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_get_protocol(char *pbt_addr)
{
    FUNC_ENTRY;
    return linuxbt_hid_get_protocol_handler(pbt_addr);
}

/****************************************************
 * FUNCTION NAME: bluetooth_hid_set_protocol
 * PURPOSE:
 *      The function is used to set  protocol
 * INPUT:
 *      pbt_addr:device address
 *      protocol_mode: protocol mode:boot or report mode
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_hid_set_protocol(char *pbt_addr, int protocol_mode)
{
    FUNC_ENTRY;
    return linuxbt_hid_set_protocol_handler(pbt_addr, protocol_mode);
}

INT32 bluetooth_hid_virtual_unplug(char *pbt_addr)
{
    FUNC_ENTRY;
    return linuxbt_hid_virtual_unplug_handler(pbt_addr);
}

INT32 bluetooth_hid_send_data(char *pbt_addr, char *data)
{
    FUNC_ENTRY;
    return linuxbt_hid_send_data_handler(pbt_addr, data);
}

/****************************************************
 * FUNCTION NAME: bluetooth_hid_get_connection_status
 * PURPOSE:
 *      The function is used for app get hid connection info
 * INPUT:
 *      BTMW_HID_CONNECTION_INFO*: hid_connection_info
 *
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */

INT32 bluetooth_hid_get_connection_status(BTMW_HID_CONNECTION_INFO *hid_connection_info)
{
    FUNC_ENTRY;
    INT32 i4_ret = 0;
    for (int i = 0; i < MAX_HID_DEV_NUM; i++)
    {
        i4_ret = bt_mw_gap_get_bond_state(hid_connected_devices[i].addr);
        if (i4_ret == BT_SUCCESS)
        {
            hid_connected_devices[i].bonded = HID_BONDED;
        }
        else if (i4_ret == BT_ERR_STATUS_NOT_READY || i4_ret == BT_ERR_STATUS_BUSY)
        {
            BT_DBG_NORMAL(BT_DEBUG_HID, "hid get connection status fail!");
        }
        else
        {
            memset(&hid_connected_devices[i], 0, sizeof(BTMW_HID_CONNECTED_DEVICE));
        }
    }
    memcpy(hid_connection_info, hid_connected_devices, sizeof(BTMW_HID_CONNECTION_INFO));
    return BT_SUCCESS;
}


/****************************************************
 * FUNCTION NAME: bluetooth_handle_hid_connect_cb
 * PURPOSE:
 *      The function is used for callback for connected
 * INPUT:
 *      bd_addr:device address
 *
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_handle_hid_connected_cb(CHAR *pbt_addr)
{
    FUNC_ENTRY;
    UINT8 index = 0;
    BT_HIDH_CBK_STRUCT cbk_st = { 0 };

    memcpy(cbk_st.addr, pbt_addr, MAX_BDADDR_LEN);
    cbk_st.event = BT_HIDH_CONNECTED;
    if (bt_mw_hidh_find_device(pbt_addr, &index))
    {
        hid_connected_devices[index].connected = BT_HIDH_CONNECTED;
        hid_connected_devices[index].bonded = HID_BONDED;
        hid_connected_devices[index].in_used = TRUE;
    }
    else if (index != MAX_HID_DEV_NUM)
    {
        hid_connected_devices[index].connected = BT_HIDH_CONNECTED;
        hid_connected_devices[index].bonded = HID_BONDED;
        hid_connected_devices[index].in_used = TRUE;
        memcpy(hid_connected_devices[index].addr, pbt_addr, MAX_BDADDR_LEN);
    }
    else
    {
        BT_DBG_NORMAL(BT_DEBUG_HID, "hid_connected_devices num is up to max");
    }
    if(NULL != HidhCbk)
    {
        BT_DBG_NORMAL(BT_DEBUG_HID, "bluetooth_handle_hid_connected_cb : HidhCbk");
        HidhCbk(&cbk_st);
    }

    return BT_SUCCESS;
}

/****************************************************
 * FUNCTION NAME: bluetooth_handle_hid_connect_fail_cb
 * PURPOSE:
 *      The function is used for callback for connect failed
 * INPUT:
 *      bd_addr:device address
 *
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_handle_hid_connect_fail_cb(CHAR *pbt_addr)
{
    FUNC_ENTRY;
    BT_HIDH_CBK_STRUCT cbk_st = { 0 };

    memcpy(cbk_st.addr, pbt_addr, MAX_BDADDR_LEN);
    cbk_st.event = BT_HIDH_CONNECT_FAIL;
    if(NULL != HidhCbk)
        HidhCbk(&cbk_st);

    return BT_SUCCESS;

}

/****************************************************
 * FUNCTION NAME: bluetooth_handle_hid_disconnect_cb
 * PURPOSE:
 *      The function is used for callback for disconnected
 * INPUT:
 *      bd_addr:device address
 *
 *
 * OUTPUT:
 *      None
 * RETURN:
 *      INT32:error code
 * NOTES:
 *      None
 */
INT32 bluetooth_handle_hid_disconnected_cb(CHAR *pbt_addr)
{
    FUNC_ENTRY;
    UINT8 index = 0;
    BT_HIDH_CBK_STRUCT cbk_st = { 0 };

    memcpy(cbk_st.addr, pbt_addr, MAX_BDADDR_LEN);
    cbk_st.event = BT_HIDH_DISCONNECTED;

    if (bt_mw_hidh_find_device(pbt_addr, &index))
    {
        hid_connected_devices[index].connected = BT_HIDH_DISCONNECTED;
    }
    else
    {
        BT_DBG_WARNING(BT_DEBUG_HID, "bluetooth_handle_hid_disconnected_cb : not find device in db");
    }
    if(NULL != HidhCbk)
        HidhCbk(&cbk_st);

    return BT_SUCCESS;
}

static VOID bt_mw_hidh_msg_handle(tBTMW_MSG *p_msg)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "bt_mw_hidh_msg_handle: event:%d", p_msg->hdr.event);

    bt_mw_nty_send_msg(p_msg);
}

static VOID bt_mw_hidh_nty_handle(tBTMW_MSG *p_msg)
{
    tBT_MW_HIDH_MSG *hidh_msg = &p_msg->data.hidh_msg;
    BT_DBG_NORMAL(BT_DEBUG_HID, "bt_mw_hidh_nty_handle: event:%d", hidh_msg->event);

    switch(hidh_msg->event)
    {
        case BTMW_HIDH_CONNECTED:
            bluetooth_handle_hid_connected_cb(hidh_msg->addr);
            break;

        case BTMW_HIDH_DISCONNECTED:
            bluetooth_handle_hid_disconnected_cb(hidh_msg->addr);
            break;

        case BTMW_HIDH_CONNECT_FAIL:
            bluetooth_handle_hid_connect_fail_cb(hidh_msg->addr);
            break;

        default:
            break;
    }

}

static INT32 bt_mw_hidh_find_device(CHAR *bd_addr, UINT8 *index)
{
    for (UINT8 i = 0; i < MAX_HID_DEV_NUM; i++)
    {
        if (hid_connected_devices[i].in_used)
        {
            if (0 == strncasecmp(bd_addr, hid_connected_devices[i].addr, MAX_BDADDR_LEN))
            {
                *index = i;
                return TRUE;
            }
        }
    }
    for (UINT8 j = 0; j < MAX_HID_DEV_NUM; j++)
    {
        if (hid_connected_devices[j].in_used)
        {
            continue;
        }
        else
        {
            *index = j;
            return FALSE;
        }
    }
    BT_DBG_WARNING(BT_DEBUG_HID, "bt_mw_hidh_find_device db is full\n");
    return FALSE;
}

