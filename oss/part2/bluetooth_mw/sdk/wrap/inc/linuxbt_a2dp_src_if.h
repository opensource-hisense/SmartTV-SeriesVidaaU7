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

#ifndef __LINUXBT_A2DP_SRC_IF_H__
#define __LINUXBT_A2DP_SRC_IF_H__

#include "u_bt_mw_a2dp.h"

#ifdef __cplusplus
extern "C" {
#endif

INT32 linuxbt_a2dp_set_audio_hw_log_lvl(UINT8 log_level);
#if !ENABLE_A2DP_ADEV
VOID init_audio_path(VOID);
VOID uninit_audio_path(VOID);
#endif
VOID linuxbt_a2dp_src_handle_connected_event(VOID);
VOID linuxbt_a2dp_src_handle_disconnected_event(VOID);

INT32 linuxbt_a2dp_src_init(BT_A2DP_SRC_INIT_CONFIG *p_src_init_config);
VOID linuxbt_a2dp_src_deinit(VOID);
INT32 linuxbt_a2dp_src_connect(CHAR *pbt_addr);
INT32 linuxbt_a2dp_src_disconnect(CHAR *pbt_addr);
INT32 linuxbt_a2dp_src_set_channel_allocation_for_lrmode(CHAR *pbt_addr, int channel);
INT32 linuxbt_a2dp_src_set_audiomode(int AudioMode);
INT32 linuxbt_a2dp_src_active_sink(CHAR *pbt_addr);
INT32 linuxbt_a2dp_src_set_silence_device(CHAR *pbt_addr, BOOL enable);

INT32 linuxbt_a2dp_src_config_codec_info(CHAR *pbt_addr, BT_A2DP_CODEC_CONFIG *p_src_set_codec_config);
extern INT32 linuxbt_a2dp_src_codec_enable_handler(BT_A2DP_CODEC_TYPE codec_type, BOOL enable);

#ifdef __cplusplus
}
#endif

#endif /* __LINUXBT_A2DP_SRC_IF_H__ */
