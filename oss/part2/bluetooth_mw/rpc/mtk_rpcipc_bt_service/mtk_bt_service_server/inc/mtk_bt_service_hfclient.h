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


#ifndef _MTK_BT_SERVICE_HFCLIENT_H_
#define _MTK_BT_SERVICE_HFCLIENT_H_

#include "mtk_bt_service_hfclient_wrapper.h"

INT32 x_mtkapi_bt_hfclient_enable(VOID);
INT32 x_mtkapi_bt_hfclient_disable(VOID);
INT32 x_mtkapi_bt_hfclient_set_msbc_t1(VOID);
INT32 x_mtkapi_bt_hfclient_connect(CHAR *bt_addr);
INT32 x_mtkapi_bt_hfclient_disconnect(CHAR *bt_addr);
INT32 x_mtkapi_bt_hfclient_connect_audio(CHAR *bt_addr);
INT32 x_mtkapi_bt_hfclient_disconnect_audio(CHAR *bt_addr);
INT32 x_mtkapi_bt_hfclient_start_voice_recognition(VOID);
INT32 x_mtkapi_bt_hfclient_stop_voice_recognition(VOID);
INT32 x_mtkapi_bt_hfclient_volume_control(BT_HFCLIENT_VOLUME_TYPE_T type, INT32 volume);
INT32 x_mtkapi_bt_hfclient_dial(const CHAR *number);
INT32 x_mtkapi_bt_hfclient_dial_memory(INT32 location);
INT32 x_mtkapi_bt_hfclient_handle_call_action(BT_HFCLIENT_CALL_ACTION_T action, INT32 idx);
INT32 x_mtkapi_bt_hfclient_query_current_calls(VOID);
INT32 x_mtkapi_bt_hfclient_query_current_operator_name(VOID);
INT32 x_mtkapi_bt_hfclient_retrieve_subscriber_info(VOID);
INT32 x_mtkapi_bt_hfclient_send_dtmf(CHAR code);
INT32 x_mtkapi_bt_hfclient_request_last_voice_tag_number(VOID);
INT32 x_mtkapi_bt_hfclient_send_at_cmd(INT32 cmd, INT32 val1, INT32 val2, const CHAR *arg);
INT32 x_mtkapi_bt_hfclient_read_pb_entries(VOID);
INT32 x_mtkapi_bt_hfclient_register_callback(mtkrpcapi_BtAppHfclientCbk func, void *pv_tag);
#endif
