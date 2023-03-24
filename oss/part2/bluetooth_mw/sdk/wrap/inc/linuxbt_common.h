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


/* FILE NAME:  linuxbt_common.h
 * AUTHOR: Hongliang Hu
 * PURPOSE:
 *      It provides bluetooth common structure to wrap.
 * NOTES:
 */

#ifndef __LINUXBT_COMMON_H__
#define __LINUXBT_COMMON_H__
#include "u_bt_mw_common.h"
#include "bluetooth.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LINUXBT_CASE_RETURN_STR(_const,str) case _const: return str

#define LINUXBT_CHECK_AND_CONVERT_ADDR(addr, rawAddr) \
do {                                         \
  if (linuxbt_check_and_convert_addr((addr), (rawAddr)) != BT_SUCCESS)                 \
  {                                                                               \
    BT_DBG_ERROR(BT_DEBUG_COMM, "invalid addr(%s)", addr==NULL?"NULL":(addr));  \
    return BT_ERR_STATUS_PARM_INVALID;                                          \
  }                                                                               \
} while(0)

INT32 linuxbt_check_and_convert_addr(CHAR *addr, RawAddress &rawAddr);

INT32 linuxbt_btaddr_stoh(CHAR *btaddr_s, RawAddress *bdaddr_h);
INT32 linuxbt_btaddr_htos(RawAddress *bdaddr_h, CHAR *btaddr_s);
INT32 linuxbt_mesh_btaddr_htos(UINT8 *bdaddr_h, CHAR *btaddr_s);
INT32 linuxbt_mesh_btaddr_stoh(CHAR *btaddr_s, UINT8 *bdaddr_h);
INT32 ascii_2_hex(CHAR *p_ascii, INT32 len, UINT8 *p_hex);
BT_ERR_STATUS_T linuxbt_return_value_convert(int ret);
const char *linuxbt_get_bt_status_str(const bt_status_t status);

#ifdef __cplusplus
}
#endif

#endif /* __LINUXBT_COMMON_H__ */
