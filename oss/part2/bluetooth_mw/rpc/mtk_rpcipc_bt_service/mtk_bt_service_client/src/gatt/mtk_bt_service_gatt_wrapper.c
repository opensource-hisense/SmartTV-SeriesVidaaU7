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


/*-----------------------------------------------------------------------------
                        include files
-----------------------------------------------------------------------------*/
#include <stdio.h>

#include "mtk_bt_service_gatt_wrapper.h"
#include "mtk_bt_service_gatt_ipcrpc_struct.h"
#include "client_common.h"
#include "ri_common.h"

#define BT_RW_LOG(_stmt...) \
        do{ \
            if(1){    \
                printf("[Client]Func:%s Line:%d--->: ", __FUNCTION__, __LINE__);   \
                printf(_stmt); \
                printf("\n"); \
            }        \
        }   \
        while(0)

EXPORT_SYMBOL INT32 a_mtkapi_bt_gatt_profile_init(VOID)
{
    BT_RW_LOG("a_mtkapi_bt_gatt_profile_init");
    INT32 i4_InvalidValue = 0;
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gatt_profile_init");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

EXPORT_SYMBOL INT32 a_mtkapi_bt_gatt_profile_deinit(VOID)
{
    BT_RW_LOG("a_mtkapi_bt_gatt_profile_deinit");
    INT32 i4_InvalidValue = 0;
    RPC_CLIENT_DECL(1, INT32);
    RPC_CLIENT_ARG_INP(ARG_TYPE_INT32, i4_InvalidValue);
    RPC_BT_SERVICE_CLIENT_DO_OP("x_mtkapi_bt_gatt_profile_deinit");
    RPC_CLIENT_RETURN(ARG_TYPE_INT32, -1);
}

INT32 c_rpc_reg_mtk_bt_service_gatt_cb_hndlrs(VOID)
{
    //int i4_ret = 0;
    /* if need callabck ,need reg here
       RPC_REG_CB_HNDLR(bt_app_gatt_xx_yy);
       */
    return RPCR_OK;
}

