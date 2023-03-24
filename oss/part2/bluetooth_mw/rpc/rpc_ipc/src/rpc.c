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

/*------------------------------------------------------------------------
                    include files
 -----------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "_ipc.h"
#include "_rpc_ipc_util.h"
#include "rpc.h"

#define MAX_PATH 255

/*------------------------------------------------------------------------
                    global variable
 -----------------------------------------------------------------------*/
extern BOOL   g_b_log;
static UINT8 ui_rpc_init_cnt = 0;

/*------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 -----------------------------------------------------------------------*/

#define IPCR_TO_RPCR(i4_ret) (i4_ret)

#define MAX_HEIGHT 32

/*
 * The memory address region tree.
 * It is a binary tree with node of memory region.
 */
typedef struct RPC_RGN_NODE_T_
{
    const VOID *             pv_addr;
    /* This is not constant !!*/
    RPC_DESC_T *             pt_desc;


    struct RPC_RGN_NODE_T_ * pt_left;
    struct RPC_RGN_NODE_T_ * pt_right;

    /*Sub tree height , for balancing. */
    UINT32                   i4_h;

}RPC_RGN_NODE_T;

typedef struct RPC_REF_VAL_T_
{
    const VOID *            pv_addr;
    struct RPC_REF_VAL_T_ * pt_next;
}RPC_REF_VAL_T;

#if 0
typedef struct RPC_REF_OUT_T_
{
    const VOID *            pv_addr;
    struct RPC_REF_OUT_T_ * pt_next;
}RPC_REF_OUT_T;
#endif

#define MAX_ARG_IDX  63


typedef struct RPC_DB_T_
{
    /* Region tree root */
    RPC_RGN_NODE_T * pt_root;

    /* Argument types by user respecified */
    ARG_TYPE_T      a_arg_type[MAX_ARG_IDX + 1];

    /*
     * References to be value
     */
    RPC_REF_VAL_T * pt_ref_val_head;

#if 0
    /*
     * References to be output
     */
    RPC_REF_OUT_T * pt_ref_out_head;
#endif
}RPC_DB_T;

#define MAX_DESC_SIZE(z_size) \
    (sizeof(RPC_DESC_T) + (z_size) * sizeof(RPC_DESC_ENTRY_T))

#define NML_DESC_SIZE(ui_num_entries) \
    (sizeof(RPC_DESC_T) + sizeof(RPC_DESC_ENTRY_T) * (ui4_num_entries) * 2)


/*------------------------------------------------------------------------
                    functions declarations
 -----------------------------------------------------------------------*/

static RPC_DESC_T * _create_desc(
    ARG_TYPE_T  e_type,
    SIZE_T      z_size,
    UINT32      ui4_num_entries,
    BOOL        b_max_size);

static RPC_DESC_T * _clone_desc(RPC_DESC_T *  pt_desc_in, BOOL b_max_size);


static VOID _free_desc(RPC_DESC_T * pt_desc);

static VOID _entry_set(
    RPC_DESC_ENTRY_T *  pt_entry,
    ARG_TYPE_T          e_type,
    const RPC_DESC_T *  pt_to_desc,
    UINT32              ui4_num_entries,
    SIZE_T              z_offs);

static INT32 _desc_add_entry(
    RPC_DESC_T *       pt_desc,
    ARG_TYPE_T         e_type,
    const RPC_DESC_T * pt_to_desc,
    UINT32             ui4_num_entries,
    SIZE_T             z_offs);


static INT32 _desc_remove_entry(
    RPC_DESC_T * pt_desc,
    RPC_DESC_ENTRY_T * pt_entry);

static BOOL _resolve_desc(
    RPC_DESC_T *        pt_desc,
    SIZE_T              z_offs,
    const RPC_DESC_T *  pt_sub_desc);

static RPC_RGN_NODE_T * _find_node(
    RPC_DB_T *        pt_db,
    const VOID *      pv_addr,
    RPC_RGN_NODE_T *  ppt_path[],
    UINT32  *         pui4_num);

static BOOL _get_merged_addr_region(
    const VOID *        pv_addr,
    SIZE_T              z_size,
    RPC_RGN_NODE_T *    pt_node,
    VOID **             ppv_addr_min,
    SIZE_T *            z_merged_size);

static RPC_RGN_NODE_T * _find_overlapped(
    RPC_DB_T *          pt_db,
    const VOID *        pv_addr,
    SIZE_T              z_size,
    RPC_RGN_NODE_T *    ppt_path[],
    UINT32  *           pui4_num);

static RPC_RGN_NODE_T * _create_node(
    const VOID *    pv_addr,
    RPC_DESC_T *    pt_desc_in);

static VOID _free_node(RPC_RGN_NODE_T * pt_node);


static RPC_RGN_NODE_T ** _get_my_space(
    RPC_DB_T * pt_db,
    RPC_RGN_NODE_T * ppt_path[],
    UINT32   ui4_idx);

static VOID _update_height(RPC_RGN_NODE_T * pt_node);

static VOID _ajust_tree(
    RPC_DB_T *       pt_db,
    RPC_RGN_NODE_T * ppt_path[],
    UINT32           ui4_num);

static VOID _remove_node(RPC_DB_T *         pt_db,
                         RPC_RGN_NODE_T *   pt_node,
                         RPC_RGN_NODE_T *   ppt_path[],
                         UINT32             ui4_num);

static RPC_RGN_NODE_T * _add_node(RPC_DB_T * pt_db, RPC_RGN_NODE_T * pt_node);

static VOID _delete_tree_r(RPC_RGN_NODE_T * pt_node);

static VOID _delete_tree(RPC_DB_T * pt_db);

static INT32 _add_sub_addr(RPC_DB_T * pt_db, RPC_DESC_T * pt_desc, const VOID * pv_addr);

static INT32 _add_addr(
    RPC_DB_T *          pt_db,
    const VOID *        pv_addr,
    const RPC_DESC_T *  pt_desc_in);

#if 0
static VOID _remove_addr(RPC_DB_T * pt_db, const VOID * pv_addr);
#endif

static VOID _clear_arg(RPC_DB_T  * pt_db);

static INT32 _update_arg_types(RPC_ID_T t_rpc_id, UINT32 ui4_num_args, ARG_DESC_T*  pt_args);

static INT32 _update_ref_vals(RPC_DB_T * pt_db);

static VOID _delete_ref_vals(RPC_DB_T *   pt_db);

static RPC_DB_T * _create_db();

static RPC_DB_T * _get_db(RPC_ID_T t_rpc_id);

static VOID _delete_db(RPC_DB_T  * pt_db);

static RPC_DESC_T * _get_desc_cb_impl (VOID * pv_addr,VOID * pv_tag, VOID ** ppv_start_addr);

static INT32 _proc_rpc_cb (
    IPC_ID_T        t_ipc_id,
    const CHAR*     ps_cb_type,
    VOID*           pv_cb_addr,
    UINT32          ui4_num_args,
    ARG_DESC_T*     pt_args,
    ARG_DESC_T*     pt_return);

static INT32 _proc_rpc_op(
    IPC_ID_T        t_ipc_id,
    const CHAR*     ps_op,
    UINT32          ui4_num_args,
    ARG_DESC_T*     pt_args,
    ARG_DESC_T*     pt_return);

#if 0
static VOID _close_func(VOID * pv_tag);
#endif

/*------------------------------------------------------------------------
                    data declarations
 -----------------------------------------------------------------------*/

static VOID * pv_db_th_key = NULL;

/*Mutex for protecting ui4_count*/
static VOID * pv_count_mtx = NULL;

VOID _dump_desc(const RPC_DESC_T * pt_desc, INT32 i4_depth, INT32 i4_max_depth)
{
#if 0
    RPC_DESC_ENTRY_T * pt_entry;
#endif
    INT32              i4_i;
    CHAR               ac_head[32];

    if (i4_max_depth < i4_depth || i4_depth > 8)
    {
        return;
    }

    for (i4_i = 0; i4_i < 3 * i4_depth; i4_i += 3)
    {
        ac_head[i4_i] = '|';
        ac_head[i4_i + 1] = ' ';
        ac_head[i4_i + 2] = ' ';
    }
    ac_head[i4_i] = '\0';
    //RPC_LOG("%s|  \n", ac_head);

    if (pt_desc != NULL)
    {

        //RPC_LOG("%s+--[DESC: 0x%x], type: %d, size: 0x%x, num: %d\n", ac_head, (UINT32)pt_desc, pt_desc->e_type, pt_desc->z_size, pt_desc->ui4_num_entries);

#if 0
        FOREACH_RPC_ENTRY(pt_desc, pt_entry)
        {
            RPC_LOG("%s  <Entry:type : %x, ui4_num_of_entries, %d\n",
                        ac_head,
                        pt_entry->e_type,
                        pt_entry->ui4_num_entries);
            _dump_desc(pt_entry->pt_desc, i4_depth + 1, i4_max_depth);
        }
#endif
    }
    else
    {
        RPC_LOG("%s+--[NULL]\n", ac_head);
    }
}
/*------------------------------------------------------------------------
 * Name:  _create_desc
 *
 * Description: Create a dynamic RPC_DESC_T.
 *
 * Inputs:  e_type          Desc arg type
 *          z_size          Data size
 *          ui4_num_entries Entry count
 *          b_max_size      To be max sized (This is for creating desc for
                                dynamiclly adding entries)
 * Outputs: -
 *
 * Returns: A RPC_DESC_T * refrence.
 -----------------------------------------------------------------------*/
static RPC_DESC_T * _create_desc(
    ARG_TYPE_T  e_type,
    SIZE_T      z_size,
    UINT32      ui4_num_entries,
    BOOL        b_max_size)
{
    RPC_DESC_T *  pt_desc;
    SIZE_T        z_desc_size;

    /* Entry count should not be more this */
    /*IPC_ASSERT(ui4_num_entries <= z_size/sizeof(VOID *));*/

    if (b_max_size == TRUE)
    {
        z_desc_size = MAX_DESC_SIZE(z_size);
    }
    else
    {
        z_desc_size = NML_DESC_SIZE(ui4_num_entries);
    }
    pt_desc = RPC_MALLOC(z_desc_size);

    if(pt_desc != NULL)
    {
        pt_desc->e_type = e_type;
        pt_desc->ui4_num_entries = ui4_num_entries;
        pt_desc->z_size = z_size;
    }
    return pt_desc;
}
/*------------------------------------------------------------------------
 * Name:  _clone_desc
 *
 * Description: Clone a desc
 *
 * Inputs:  pt_desc_in     Source desc
 *          b_max_size     To be max sized (This is for creating desc for
                                dynamiclly adding entries)
 * Outputs: -
 *
 * Returns: A RPC_DESC_T * refrence.
 -----------------------------------------------------------------------*/
static RPC_DESC_T * _clone_desc(RPC_DESC_T *  pt_desc_in, BOOL b_max_size)
{
    RPC_DESC_T *  pt_desc;
    SIZE_T        z_desc_size;

    if (b_max_size == TRUE)
    {
        z_desc_size = MAX_DESC_SIZE(pt_desc_in->z_size);
    }
    else
    {
        z_desc_size = RPC_DESC_SIZE(pt_desc_in);
    }

    pt_desc = RPC_MALLOC(z_desc_size);
    if(pt_desc != NULL)
    {
        memcpy(pt_desc, pt_desc_in, z_desc_size);
    }
    return pt_desc;
}

/*------------------------------------------------------------------------
 * Name:  _free_desc
 *
 * Description: Free a desc
 *
 * Inputs:  pt_desc     Description to be released
 * Outputs: -*
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _free_desc(RPC_DESC_T * pt_desc)
{
    if(NULL != pt_desc)
    {
    RPC_FREE(pt_desc);
        pt_desc = NULL;
    }else
    {
        printf("<DBG> Have free. @Line %d\n", __LINE__);
    }
}
/*------------------------------------------------------------------------
 * Name:  _entry_set
 *
 * Description: Set entry information
 *
 * Inputs:  pt_entry        Entry to be updated
 *          e_type          Type of entry
 *          pt_to_desc      Entry pointed description
 *          ui4_num_entries Count for array
 *          z_offs          Offset
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _entry_set(
    RPC_DESC_ENTRY_T *  pt_entry,
    ARG_TYPE_T          e_type,
    const RPC_DESC_T *  pt_to_desc,
    UINT32              ui4_num_entries,
    SIZE_T              z_offs)
{
    pt_entry->e_type            = e_type;
    pt_entry->pt_desc           = pt_to_desc;
    pt_entry->ui4_num_entries   = ui4_num_entries;
    pt_entry->u.z_offs          = z_offs;
}

/*------------------------------------------------------------------------
 * Name:  _desc_add_entry
 *
 * Description: Add entry to desc
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _desc_add_entry(
    RPC_DESC_T *       pt_desc,
    ARG_TYPE_T         e_type,
    const RPC_DESC_T * pt_to_desc,
    UINT32             ui4_num_entries,
    SIZE_T             z_offs)
{
    RPC_DESC_ENTRY_T * pt_entry_end;

    pt_entry_end = RPC_DESC_GET_ENTRY_END(pt_desc);

    _entry_set(pt_entry_end, e_type, pt_to_desc, ui4_num_entries, z_offs);

    pt_desc->ui4_num_entries ++;

    return RPCR_OK;
}


/*------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _desc_remove_entry(RPC_DESC_T * pt_desc, RPC_DESC_ENTRY_T * pt_entry)
{
    RPC_DESC_ENTRY_T * pt_entry_last;

    if (pt_desc->ui4_num_entries > 1)
    {
        pt_entry_last = RPC_DESC_GET_ENTRY_END(pt_desc) - 1;
        if (pt_entry_last != pt_entry)
        {
            memcpy(pt_entry, pt_entry_last, sizeof(RPC_DESC_ENTRY_T));
        }
    }
    pt_desc->ui4_num_entries --;

    return RPCR_OK;
}
#if 0
static SIZE_T _get_size_of(ARG_TYPE_T e_type, VOID * pv_ref)
{
    switch(e_type & ARG_MASK_TYPE)
    {
        case ARG_TYPE_REF_CHAR:
            return sizeof(CHAR);
        case ARG_TYPE_REF_INT8:
            return sizeof(INT8);
        case ARG_TYPE_REF_INT16:
            return sizeof(INT16);
        case ARG_TYPE_REF_INT32:
            return sizeof(INT32);
        case ARG_TYPE_REF_UINT8:
            return sizeof(UINT8);
        case ARG_TYPE_REF_UINT16:
            return sizeof(UINT16);
        case ARG_TYPE_REF_UINT32:
            return sizeof(UINT32);
        case ARG_TYPE_REF_SIZE_T:
            return sizeof(SIZE_T);
        case ARG_TYPE_REF_INT64:
            return sizeof(INT64);
        case ARG_TYPE_REF_UINT64:
            return sizeof(UINT64);
        case ARG_TYPE_REF_BOOL:
            return sizeof(BOOL);
        case ARG_TYPE_REF_STR:
            IPC_ASSERT(pv_ref != NULL)
            return strlen(pv_ref) + 1;
        default:
            IPC_ASSERT(0);
    }
}

static BOOL _resolve_desc_const(
    RPC_DESC_T *        pt_desc,
    SIZE_T              z_offs,
    const RPC_DESC_T *  pt_sub_desc,
    bool                b_parent_const)
{
    RPC_DESC_ENTRY_T *  pt_entry;
    RPC_DESC_ENTRY_T *  pt_sub_entry;

    BOOL                b_ret = FALSE;
    UINT32              ui4_i;

    SIZE_T              z_sub_offs;
    BOOL                b_const;

    if (pt_sub_desc != NULL)
    {
        if((pt_sub_desc->e_type & ARG_MASK_TYPE)== ARG_TYPE_STRUCT)
        {
            FOREACH_RPC_ENTRY(pt_sub_desc, pt_sub_entry)
            {
                z_sub_offs = z_offs + pt_sub_entry->u.z_offs;
                if ((pt_sub_entry->e_type  & ARG_MASK_TYPE) == ARG_TYPE_DESC)
                {
                    b_const = FALSE;
                    if(b_parent_const == TRUE)
                    {
                        b_const = TRUE;
                    }
                    else
                    {
                        /* If the dir is ONLY input, this will be const */
                        if((pt_sub_entry->e_type & ARG_MASK_DIR) == ARG_TYPE_INP)
                        {
                            b_const = TRUE;
                        }
                    }
                    for (ui4_i = 0; ui4_i < pt_sub_entry->ui4_num_entries; ui4_i ++)
                    {
                        if(_resolve_desc(pt_desc, z_sub_offs, pt_sub_entry->pt_desc, b_const) == TRUE)
                        {
                            b_ret = TRUE;
                        }
                        z_sub_offs += pt_sub_entry->pt_desc->z_size;
                    }
                }
                else if (   (pt_sub_entry->e_type & ARG_MASK_TYPE) == ARG_TYPE_REF_DESC
                         || (pt_sub_entry->e_type & ARG_MASK_TYPE) == ARG_TYPE_REF_STR)
                {
                    for (ui4_i = 0; ui4_i < pt_sub_entry->ui4_num_entries; ui4_i ++)
                    {
                        FOREACH_RPC_ENTRY(pt_desc, pt_entry)
                        {
                            if (pt_entry->u.z_offs == z_sub_offs + ui4_i * sizeof(VOID *))
                            {
                                break;
                            }
                        }
                        /* For reference, we should not apply the parent const qualifier,
                         * Because currently we don't have ARG_DIR_... to specify the form:
                         * X_T * const pt_xt;
                         */
                        if (pt_entry == RPC_DESC_GET_ENTRY_END(pt_desc))
                        {
                            _desc_add_entry(
                                pt_desc,
                                pt_sub_entry->e_type,
                                pt_sub_entry->pt_desc,
                                1,
                                z_sub_offs + ui4_i * sizeof(VOID *));

                            b_ret = TRUE;
                        }
                    }
                }
                else
                {
                    if ((pt_sub_entry->e_type  & ARG_MASK_DIR) == ARG_DIR_INP)
                    {
                        ARG_TYPE_T e_type;

                        for (ui4_i = 0; ui4_i < pt_sub_entry->ui4_num_entries; ui4_i ++)
                        {
                            /* Assume no duplicate description */
                            FOREACH_RPC_ENTRY(pt_desc, pt_entry)
                            {
                                if (pt_entry->u.z_offs == z_sub_offs
                                                        + ui4_i * _get_size_of(pt_sub_entry->e_type & ARG_MASK_TYPE, NULL))
                                {
                                    break;
                                }
                            }

                            if (pt_entry == RPC_DESC_GET_ENTRY_END(pt_desc))
                            {
                                e_type = pt_sub_entry->e_type;
                                if(b_parent_const)
                                {
                                    e_type = (e_type & ARG_MASK_TYPE) & ARG_DIR_INP;
                                }
                                _desc_add_entry(
                                    pt_desc,
                                    e_type,
                                    pt_sub_entry->pt_desc,
                                    1,
                                    z_sub_offs + ui4_i * _get_size_of(pt_sub_entry->e_type & ARG_MASK_TYPE, NULL));

                                b_ret = TRUE;
                            }
                        }
                    }
                    else
                    {
                        RPC_ERR("Unexpected 0x%x\n", pt_sub_entry->e_type);
                        IPC_ASSERT(0);
                    }
                }
            }
        }
        else
        {

        }
    }
    return b_ret;
}
#endif

static BOOL _resolve_desc_inp(
    RPC_DESC_T *        pt_desc,
    SIZE_T              z_offs,
    const RPC_DESC_T *  pt_sub_desc)
{
    RPC_DESC_ENTRY_T *  pt_entry;
    RPC_DESC_ENTRY_T *  pt_sub_entry;

    BOOL                b_ret = FALSE;
    UINT32              ui4_i;

    SIZE_T              z_sub_offs;

    if (pt_sub_desc != NULL)
    {
        if((pt_sub_desc->e_type & ARG_MASK_TYPE)== ARG_TYPE_STRUCT)
        {
            FOREACH_RPC_ENTRY(pt_sub_desc, pt_sub_entry)
            {
                z_sub_offs = z_offs + pt_sub_entry->u.z_offs;
                if ((pt_sub_entry->e_type  & ARG_MASK_TYPE) == ARG_TYPE_DESC)
                {
                    for (ui4_i = 0; ui4_i < pt_sub_entry->ui4_num_entries; ui4_i ++)
                    {
                        if(_resolve_desc(pt_desc, z_sub_offs, pt_sub_entry->pt_desc) == TRUE)
                        {
                            b_ret = TRUE;
                        }
                        z_sub_offs += pt_sub_entry->pt_desc->z_size;
                    }
                    /*TODO: Here , we should add a entry for const structure */
                }
                else if (   (pt_sub_entry->e_type & ARG_MASK_TYPE) == ARG_TYPE_REF_DESC
                         || (pt_sub_entry->e_type & ARG_MASK_TYPE) == ARG_TYPE_REF_STR)
                {
                    for (ui4_i = 0; ui4_i < pt_sub_entry->ui4_num_entries; ui4_i ++)
                    {
                        FOREACH_RPC_ENTRY(pt_desc, pt_entry)
                        {
                            if (pt_entry->u.z_offs == z_sub_offs + ui4_i * sizeof(VOID *)
                                && (   (pt_entry->e_type & ARG_MASK_TYPE) == ARG_TYPE_REF_DESC
                                    || (pt_entry->e_type & ARG_MASK_TYPE) == ARG_TYPE_REF_STR)
                                )
                            {
                                break;
                            }
                        }
                        if (pt_entry == RPC_DESC_GET_ENTRY_END(pt_desc))
                        {
                            _desc_add_entry(
                                pt_desc,
                                pt_sub_entry->e_type,
                                pt_sub_entry->pt_desc,
                                1,
                                z_sub_offs + ui4_i * sizeof(VOID *));

                            b_ret = TRUE;
                        }
                    }
                }
                else if (pt_sub_entry->e_type == (ARG_TYPE_UINT8 | ARG_DIR_INP))
                {
                    /*ARG_TYPE_T e_type;*/

                    /* Assume no duplicate description */
                    FOREACH_RPC_ENTRY(pt_desc, pt_entry)
                    {
                        if (    pt_entry->u.z_offs == z_sub_offs
                            &&  pt_entry->e_type == (ARG_TYPE_UINT8 | ARG_DIR_INP)
                            &&  pt_entry->ui4_num_entries >= pt_sub_entry->ui4_num_entries)
                        {
                            /* TODO: Merge the region for const */
                            break;
                        }
                    }

                    if (pt_entry == RPC_DESC_GET_ENTRY_END(pt_desc))
                    {
                        _desc_add_entry(
                            pt_desc,
                            pt_sub_entry->e_type,
                            pt_sub_entry->pt_desc,
                            /* This is size of the const region, */
                            pt_sub_entry->ui4_num_entries,
                            z_sub_offs);

                        b_ret = TRUE;
                    }

                }
                else
                {
                    RPC_ERR("Unexpected %d\n", (int)(pt_sub_entry->e_type));
                    IPC_ASSERT(0);
                }
            }
        }
        else
        {

        }
    }
    return b_ret;
}


/*------------------------------------------------------------------------
 * Name:
 *
 * Description: This is for update a sub entry desc into a existing desc.
 * Add each link field from sub desc with offset re-caculated.
 * The ARG_TYPE_DESC with type ARG_TYPE_STRUCT will be resolved.
 *
 * Undetermined entries of ARG_TYPE_DESC with type ARG_TYPE_UNION
 * currently are ignored. FIXME: We need to save it as a verification
 * for resolving. So currently ps_field_name are not used.
 *
 *
 * If no new link added, this returns FALSE.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/

static BOOL _resolve_desc(
    RPC_DESC_T *        pt_desc,
    SIZE_T              z_offs,
    const RPC_DESC_T *  pt_sub_desc)
{
    return _resolve_desc_inp(pt_desc, z_offs, pt_sub_desc);
#if 0
    RPC_DESC_ENTRY_T *  pt_entry;
    RPC_DESC_ENTRY_T *  pt_sub_entry;

    BOOL                b_ret = FALSE;
    UINT32              ui4_i;

    SIZE_T              z_sub_offs;

    if (pt_sub_desc != NULL)
    {
        if((pt_sub_desc->e_type & ARG_MASK_TYPE)== ARG_TYPE_STRUCT)
        {
            FOREACH_RPC_ENTRY(pt_sub_desc, pt_sub_entry)
            {
                z_sub_offs = z_offs + pt_sub_entry->u.z_offs;
                if ((pt_sub_entry->e_type  & ARG_MASK_TYPE) == ARG_TYPE_DESC)
                {
                    for (ui4_i = 0; ui4_i < pt_sub_entry->ui4_num_entries; ui4_i ++)
                    {
                        if(_resolve_desc(pt_desc, z_sub_offs, pt_sub_entry->pt_desc) == TRUE)
                        {
                            b_ret = TRUE;
                        }
                        z_sub_offs += pt_sub_entry->pt_desc->z_size;
                    }
                }
                else if (   (pt_sub_entry->e_type & ARG_MASK_TYPE) == ARG_TYPE_REF_DESC
                         || (pt_sub_entry->e_type & ARG_MASK_TYPE) == ARG_TYPE_REF_STR)
                {
                    for (ui4_i = 0; ui4_i < pt_sub_entry->ui4_num_entries; ui4_i ++)
                    {
                        FOREACH_RPC_ENTRY(pt_desc, pt_entry)
                        {
                            if (pt_entry->u.z_offs == z_sub_offs + ui4_i * sizeof(VOID *))
                            {
                                break;
                            }
                        }
                        if (pt_entry == RPC_DESC_GET_ENTRY_END(pt_desc))
                        {
                            _desc_add_entry(
                                pt_desc,
                                pt_sub_entry->e_type,
                                pt_sub_entry->pt_desc,
                                1,
                                z_sub_offs + ui4_i * sizeof(VOID *));

                            b_ret = TRUE;
                        }
                    }
                }
                else
                {
                    RPC_ERR("Unexpected 0x%x\n", pt_sub_entry->e_type);
                    IPC_ASSERT(0);
                }
            }
        }
        else
        {

        }
    }
    return b_ret;
#endif
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Look for node which address equaling pv_addr
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static RPC_RGN_NODE_T * _find_node(
    RPC_DB_T *        pt_db,
    const VOID *      pv_addr,
    RPC_RGN_NODE_T *  ppt_path[],
    UINT32  *         pui4_num)
{
    RPC_RGN_NODE_T * pt_csr;

    if (pt_db->pt_root == NULL)
    {
        if(pui4_num != NULL)
        {
            *pui4_num = 0;
        }
        return NULL;
    }
    if(ppt_path != NULL && pui4_num != NULL)
    {
        *pui4_num = 0;

        for (pt_csr = pt_db->pt_root; pt_csr != NULL; )
        {
            ppt_path[(*pui4_num)++] = pt_csr;

            if (pv_addr == pt_csr->pv_addr)
            {
                return pt_csr;
            }
            else if ((UINT32)pv_addr < (UINT32)pt_csr->pv_addr)
            {
                pt_csr = pt_csr->pt_left;
            }
            else
            {
                pt_csr = pt_csr->pt_right;
            }

        }
    }
    else
    {
        for (pt_csr = pt_db->pt_root; pt_csr != NULL; )
        {
            if (pv_addr == pt_csr->pv_addr)
            {
                return pt_csr;
            }
            else if ((UINT32)pv_addr < (UINT32)pt_csr->pv_addr)
            {
                pt_csr = pt_csr->pt_left;
            }
            else
            {
                pt_csr = pt_csr->pt_right;
            }

        }
    }
    return NULL;
}
/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Get merged address region for a node and a input region
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static BOOL _get_merged_addr_region(
    const VOID *        pv_addr,
    SIZE_T              z_size,
    RPC_RGN_NODE_T *    pt_node,
    VOID **             ppv_addr_min,
    SIZE_T *            z_merged_size)
{
    VOID *  pv_addr_max;
    BOOL    b_increased = FALSE;

    if ((CHAR *)pv_addr < (CHAR *)pt_node->pv_addr)
    {
        b_increased = TRUE;
        *ppv_addr_min = (VOID *)pv_addr;
    }
    else
    {
        *ppv_addr_min = (CHAR *)pt_node->pv_addr;
    }

    if ((CHAR *)pv_addr + z_size <= (CHAR *)pt_node->pv_addr + pt_node->pt_desc->z_size)
    {
        pv_addr_max = (CHAR *)pt_node->pv_addr + pt_node->pt_desc->z_size;
    }
    else
    {
        b_increased = TRUE;
        pv_addr_max = (CHAR *)pv_addr + z_size;
    }

    *z_merged_size = (CHAR *)pv_addr_max - (CHAR *)*ppv_addr_min;

    return b_increased;
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Find node if region overlapped with some node
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
/*
 * Find the addr overlapped region node
 */
static RPC_RGN_NODE_T * _find_overlapped(
    RPC_DB_T *          pt_db,
    const VOID *        pv_addr,
    SIZE_T              z_size,
    RPC_RGN_NODE_T *    ppt_path[],
    UINT32  *           pui4_num)
{
    RPC_RGN_NODE_T * pt_csr;

    if (pt_db->pt_root == NULL)
    {
        if(pui4_num != NULL)
        {
            *pui4_num = 0;
        }
        return NULL;
    }

    if(ppt_path != NULL && pui4_num != NULL)
    {
        *pui4_num = 0;

        for (pt_csr = pt_db->pt_root; pt_csr != NULL; )
        {
            ppt_path[(*pui4_num)++] = pt_csr;

            if ((UINT32)pv_addr + z_size <= (UINT32)pt_csr->pv_addr)
            {
                pt_csr = pt_csr->pt_left;
            }
            else if ((UINT32)pv_addr >= (UINT32)pt_csr->pv_addr + pt_csr->pt_desc->z_size)
            {
                pt_csr = pt_csr->pt_right;
            }
            else
            {
                return pt_csr;
            }
        }
    }
    else
    {
        for (pt_csr = pt_db->pt_root; pt_csr != NULL; )
        {
            if ((UINT32)pv_addr + z_size <= (UINT32)pt_csr->pv_addr)
            {
                pt_csr = pt_csr->pt_left;
            }
            else if ((UINT32)pv_addr >= (UINT32)pt_csr->pv_addr + pt_csr->pt_desc->z_size)
            {
                pt_csr = pt_csr->pt_right;
            }
            else
            {
                return pt_csr;
            }
        }
    }

    return NULL;
}


/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Create a new node
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static RPC_RGN_NODE_T * _create_node(
    const VOID *    pv_addr,
    RPC_DESC_T *    pt_desc_in)
{
    RPC_RGN_NODE_T *    pt_node;

    pt_node = RPC_MALLOC(sizeof(RPC_RGN_NODE_T));

    if(pt_node != NULL)
    {
        pt_node->pv_addr    = pv_addr;

        if(pt_desc_in != NULL)
        {
            pt_node->pt_desc = _clone_desc(pt_desc_in, FALSE);
        }

        pt_node->pt_left    = NULL;
        pt_node->pt_right   = NULL;


        pt_node->i4_h   = 1;
    }

    return pt_node;
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Free node
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _free_node(RPC_RGN_NODE_T * pt_node)
{
    if ((NULL != pt_node) && (NULL != pt_node->pt_desc))
    {
    _free_desc(pt_node->pt_desc);
    }

    if (NULL != pt_node)
    {
    RPC_FREE(pt_node);
	    pt_node = NULL;
    }
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Get address storing the pointer to self in parent
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static RPC_RGN_NODE_T ** _get_my_space(
    RPC_DB_T *       pt_db,
    RPC_RGN_NODE_T * ppt_path[],
    UINT32           ui4_idx)
{
    RPC_RGN_NODE_T * pt_parent;
    if(ui4_idx == 0)
    {
        return &pt_db->pt_root;
    }
    else
    {
        pt_parent = ppt_path[ui4_idx - 1];
        if(pt_parent->pt_left == ppt_path[ui4_idx])
        {
            return &pt_parent->pt_left;
        }
        else
        {
            return &pt_parent->pt_right;
        }
    }
}



#define HEIGHT(pt_node) ((INT32)((pt_node) == NULL?0:(pt_node)->i4_h))
/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Update node height info
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _update_height(RPC_RGN_NODE_T * pt_node)
{
    INT32 i4_hl = HEIGHT(pt_node->pt_left);
    INT32 i4_hr = HEIGHT(pt_node->pt_right);

    pt_node->i4_h = ((i4_hl > i4_hr)?i4_hl:i4_hr) + 1;
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Ajust tree to balancing
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
/*
 * Common balancing for the tree. (AVL)
 * TODO: For adding we just need one time of rotation .
 *
 */
static VOID _ajust_tree(
    RPC_DB_T *       pt_db,
    RPC_RGN_NODE_T * ppt_path[],
    UINT32           ui4_num)
{
    RPC_RGN_NODE_T * pt_left;
    RPC_RGN_NODE_T * pt_right;

    RPC_RGN_NODE_T * pt_left_left;
    RPC_RGN_NODE_T * pt_left_right;
    RPC_RGN_NODE_T * pt_right_left;
    RPC_RGN_NODE_T * pt_right_right;

    RPC_RGN_NODE_T * pt_left_right_left;
    RPC_RGN_NODE_T * pt_left_right_right;

    RPC_RGN_NODE_T * pt_right_left_left;
    RPC_RGN_NODE_T * pt_right_left_right;

    RPC_RGN_NODE_T ** ppt_chld;


    RPC_RGN_NODE_T * pt_node;

    INT32 i4_left_h;
    INT32 i4_right_h;

    INT32 i4_left_left_h;
    INT32 i4_left_right_h;
    INT32 i4_right_left_h;
    INT32 i4_right_right_h;

    while(ui4_num > 0)
    {
        pt_node     = ppt_path[ui4_num - 1];
        pt_left     = pt_node->pt_left;
        pt_right    = pt_node->pt_right;
        i4_left_h   = HEIGHT(pt_left);
        i4_right_h  = HEIGHT(pt_right);

        if(i4_left_h > i4_right_h + 1)
        {
            if (NULL == pt_left)
            {
                printf("pt_left is null, L%d\n", __LINE__);
                return ;
            }
            pt_left_left     = pt_left->pt_left;
            i4_left_left_h   = HEIGHT(pt_left_left);

            pt_left_right    = pt_left->pt_right;
            i4_left_right_h  = HEIGHT(pt_left_right);

            /* LL*/
            if(i4_left_left_h > i4_left_right_h)
            {
                RPC_LOG("Rot LL\n");
                ppt_chld = _get_my_space(pt_db, ppt_path, ui4_num - 1);

                pt_node->pt_left    = pt_left_right;
                pt_left->pt_right   = pt_node;
                *ppt_chld = pt_left;

                _update_height(pt_node);
                _update_height(pt_left);
            }
            /* LR */
            else
            {
                RPC_LOG("Rot LR\n");
                ppt_chld = _get_my_space(pt_db, ppt_path, ui4_num - 1);
                if (NULL == pt_left_right)
                {
                    printf("pt_left_right is null, L%d\n", __LINE__);
                    return ;
                }
                pt_left_right_left      = pt_left_right->pt_left;
                pt_left_right_right     = pt_left_right->pt_right;
                pt_left_right->pt_left  = pt_left;
                pt_left_right->pt_right = pt_node;
                pt_left->pt_right       = pt_left_right_left;
                pt_node->pt_left        = pt_left_right_right;

                if(ppt_chld != NULL)*ppt_chld = pt_left_right;

                _update_height(pt_left);
                _update_height(pt_node);
                _update_height(pt_left_right);
            }
            /* TODO: For adding, here we could return*/
        }
        else if(i4_right_h > i4_left_h + 1)
        {
            if (NULL == pt_right)
            {
                printf("pt_right is null, L%d\n", __LINE__);
                return ;
            }
            pt_right_right     = pt_right->pt_right;
            i4_right_right_h   = HEIGHT(pt_right_right);

            pt_right_left    = pt_right->pt_left;
            i4_right_left_h  = HEIGHT(pt_right_left);

            /* RR*/
            if(i4_right_right_h > i4_right_left_h)
            {
                RPC_LOG("Rot RR\n");
                ppt_chld = _get_my_space(pt_db, ppt_path, ui4_num - 1);

                pt_node->pt_right   = pt_right_left;
                pt_right->pt_left   = pt_node;
                *ppt_chld = pt_right;

                _update_height(pt_node);
                _update_height(pt_right);

            }
            /* RL */
            else
            {
                RPC_LOG("Rot RL\n");
                ppt_chld = _get_my_space(pt_db, ppt_path, ui4_num - 1);
                if (NULL == pt_right_left)
                {
                    printf("pt_right_left is null, L%d\n", __LINE__);
                    return ;
                }
                pt_right_left_right     = pt_right_left->pt_right;
                pt_right_left_left      = pt_right_left->pt_left;
                pt_right_left->pt_right = pt_right;
                pt_right_left->pt_left  = pt_node;
                pt_right->pt_left       = pt_right_left_right;
                pt_node->pt_right       = pt_right_left_left;

                if(ppt_chld != NULL)*ppt_chld = pt_right_left;

                _update_height(pt_right);
                _update_height(pt_node);
                _update_height(pt_right_left);

            }
            /* TODO: For adding, here we could return*/
        }
        else
        {
            _update_height(pt_node);
        }
        ui4_num--;
    }

}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Remove node
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _remove_node(
    RPC_DB_T *         pt_db,
    RPC_RGN_NODE_T *   pt_node,
    RPC_RGN_NODE_T *   ppt_path[],
    UINT32             ui4_num)
{
    RPC_RGN_NODE_T *  pt_del;
    RPC_RGN_NODE_T *  pt_chld;
    RPC_RGN_NODE_T ** ppt_chld;

    if (pt_node->pt_left == NULL || pt_node->pt_right == NULL)
    {
        pt_del = pt_node;
    }
    else
    {
        pt_del = pt_node->pt_right;
        do
        {
            ppt_path[ui4_num ++] = pt_del;
            pt_del    = pt_del->pt_left;
            /* Find the minimum */
        }while(pt_del != NULL);

        pt_del = ppt_path[ui4_num - 1];
    }

    ppt_chld = _get_my_space(pt_db, ppt_path, ui4_num - 1);

    if (pt_del->pt_left != NULL)
    {
        pt_chld = pt_del->pt_left;
    }
    else
    {
        pt_chld = pt_del->pt_right;
    }

    *ppt_chld = pt_chld;

    if (pt_del != pt_node)
    {
        RPC_DESC_T *    pt_desc_tmp;
        /* Copy data to node */
        pt_node->pv_addr = pt_del->pv_addr;

        pt_desc_tmp      = pt_node->pt_desc;
        pt_node->pt_desc = pt_del->pt_desc;
        pt_del->pt_desc  = pt_desc_tmp;

    }

    _free_node(pt_del);

    ui4_num --;

    _ajust_tree(pt_db, ppt_path, ui4_num);
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Add new node, if node addr exists old node returned
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static RPC_RGN_NODE_T * _add_node(RPC_DB_T * pt_db, RPC_RGN_NODE_T * pt_node)
{
    RPC_RGN_NODE_T * ppt_path[MAX_HEIGHT];
    UINT32           ui4_num;
    RPC_RGN_NODE_T * pt_csr;

    pt_csr = _find_node(pt_db, pt_node->pv_addr, ppt_path, &ui4_num);
    if(pt_csr != NULL)
    {
        return pt_csr;
    }
    else
    {
        if(ui4_num == 0)
        {
            pt_db->pt_root = pt_node;
        }
        else
        {
            if ((UINT32)pt_node->pv_addr < (UINT32)ppt_path[ui4_num - 1]->pv_addr)
            {
                ppt_path[ui4_num - 1]->pt_left  = pt_node;
            }
            else
            {
                ppt_path[ui4_num - 1]->pt_right = pt_node;
            }

            _ajust_tree(pt_db, ppt_path, ui4_num);
        }
    }

    return pt_node;
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Delete tree recursively
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _delete_tree_r(RPC_RGN_NODE_T * pt_node)
{
    if(pt_node->pt_left != NULL)
    {
        _delete_tree_r(pt_node->pt_left);
		pt_node->pt_left = NULL;
    }
    if(pt_node->pt_right != NULL)
    {
        _delete_tree_r(pt_node->pt_right);
		pt_node->pt_right = NULL;
    }
    _free_node(pt_node);
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Delete tree
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _delete_tree(RPC_DB_T * pt_db)
{
    if(pt_db->pt_root != NULL)
    {
        _delete_tree_r(pt_db->pt_root);
    }
    pt_db->pt_root = NULL;
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
#if 0
void _dump_tree(RPC_RGN_NODE_T * pt_node, INT32 i4_l, INT32 i4_lr)
{
    CHAR               ac_head[128];
    int i;

    for (i = 0; i < i4_l; i ++)
    {
        ac_head[i] = ' ';
    }
    ac_head[i] = '\0';
    if(pt_node == NULL)
    {
        return;
    }
    if(i4_lr == 0)
    {
        printf("%sL (%d), %d\n", ac_head, pt_node->pv_addr, pt_node->i4_h);
    }
    else
    {
        printf("%sR (%d), %d\n", ac_head, pt_node->pv_addr, pt_node->i4_h);
    }
    _dump_tree(pt_node->pt_left, i4_l + 1, 0);
    _dump_tree(pt_node->pt_right, i4_l + 1, 1);

}
#endif

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Add sub address descriptions into database by going through
 *              each entry of its description.
 *              This is recursive with _add_addr.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 _add_sub_addr(
    RPC_DB_T *          pt_db,
    RPC_DESC_T *        pt_desc,
    const VOID *        pv_addr)
{
    RPC_DESC_ENTRY_T *  pt_entry;
    INT32               i4_ret;

    FOREACH_RPC_ENTRY(pt_desc, pt_entry)
    {
        if (pt_entry->pt_desc != NULL && pt_entry->e_type != (ARG_TYPE_UINT8 | ARG_DIR_INP))
        {
            IPC_ASSERT(pt_entry->e_type != ARG_TYPE_DESC);

            i4_ret = _add_addr(pt_db,
                               *(VOID **)((CHAR *)pv_addr + pt_entry->u.z_offs),
                               pt_entry->pt_desc);

            if(i4_ret != IPCR_OK)
            {
                return i4_ret;
            }
            /*
             * Unset this desc link.
             * New desc link will be rebuilt after adding.
             */
            pt_entry->pt_desc = NULL;
        }
    }
    return IPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Add address with input description into database, if the
 *              description contribute new information (new links, increasing
 *              size etc.). Also the sub address in the region will also added.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _add_addr(
    RPC_DB_T *          pt_db,
    const VOID *        pv_addr,
    const RPC_DESC_T *  pt_desc_in)
{

    RPC_RGN_NODE_T * pt_node;
    VOID *           pv_addr_min;

    RPC_DESC_T *     pt_desc;

    RPC_RGN_NODE_T * ppt_path[MAX_HEIGHT];
    UINT32           ui4_path_num;

    IPC_ASSERT(pt_desc_in != NULL);

    //RPC_LOG("[IPC DB] _add_addr 0x%x,  z_size:0x%x\n", (unsigned)pv_addr, (unsigned)pt_desc_in->z_size);


    if(pv_addr == NULL)
    {
        /* NULL pointer not take error, just ignore */
        return IPCR_OK;
    }

    if((pt_desc = _create_desc(ARG_TYPE_STRUCT, pt_desc_in->z_size, 0, TRUE)) == NULL)
    {
        return RPCR_OUTOFMEMORY;
    }

    _resolve_desc(pt_desc, 0, pt_desc_in);

    while ((pt_node = _find_overlapped(pt_db,
                                       pv_addr,
                                       pt_desc->z_size,
                                       ppt_path,
                                       &ui4_path_num)) != NULL)
    {

        /* We find it, do some update */
        SIZE_T          z_size;
        RPC_DESC_T *    pt_ex_desc;
        BOOL            b_increased;

        b_increased = _get_merged_addr_region(pv_addr, pt_desc->z_size, pt_node, &pv_addr_min, &z_size);

        /* A desc with maxnium */
        if((pt_ex_desc = _create_desc(ARG_TYPE_STRUCT, z_size, 0, TRUE)) == NULL)
        {
            _free_desc(pt_desc);
            return RPCR_OUTOFMEMORY;
        }
        /* Merge these two. */
        _resolve_desc(pt_ex_desc,
                      (CHAR *)pt_node->pv_addr - (CHAR *)pv_addr_min,
                      pt_node->pt_desc);
        /*
         * If this new pt_desc does not contribute new reference
         * link information, AND it does not change any region size.
         * Then it is considered a duplicated and ignored.
         *
         * This is one of the end condition of recursive !
         */
        if (_resolve_desc(pt_ex_desc,
                          (CHAR *)pv_addr - (CHAR *)pv_addr_min,
                          pt_desc) == FALSE)
        {
            if(b_increased == FALSE)
            {
                RPC_LOG("pv_addr %p does not contribute\n", pv_addr);
                _free_desc(pt_desc);
                _free_desc(pt_ex_desc);
                return IPCR_OK;
            }
        }

        /* Remove old one */
        _remove_node(pt_db, pt_node, ppt_path, ui4_path_num);

        _free_desc(pt_desc);
        pt_desc = pt_ex_desc;

        pv_addr = pv_addr_min;

    }

    /* Here the pt_desc will not be overlapped with database */

    if((pt_node = _create_node(pv_addr, pt_desc)) == NULL)
    {
        _free_desc(pt_desc);
        return RPCR_OUTOFMEMORY;
    }

    _free_desc(pt_desc);

    _add_node(pt_db, pt_node);
    _add_sub_addr(pt_db, pt_node->pt_desc, pv_addr);

    return RPCR_OK;

}

#if 0
/*------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
/* Delete the addr stored node */
static VOID _remove_addr(RPC_DB_T * pt_db, const VOID * pv_addr)
{
    RPC_RGN_NODE_T * pt_node;
    RPC_RGN_NODE_T * ppt_path[MAX_HEIGHT];
    UINT32  ui4_num;

    pt_node = _find_node(pt_db, pv_addr, ppt_path, &ui4_num);
    if (pt_node == NULL)
    {
        return;
    }

    _remove_node(pt_db, pt_node, ppt_path, ui4_num);

}
#endif

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Clear arg settings.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _clear_arg(RPC_DB_T  * pt_db)
{
    UINT32      ui4_i;

    for (ui4_i = 0; ui4_i < MAX_ARG_IDX; ui4_i ++)
    {
        pt_db->a_arg_type[ui4_i] = ARG_TYPE_VOID;
    }

}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Update user specified arg type for ARG_TYPE_VARIABLE
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _update_arg_types(RPC_ID_T t_rpc_id, UINT32 ui4_num_args, ARG_DESC_T*  pt_args)
{
    UINT32      ui4_idx;
    ARG_TYPE_T  e_type;

    for (ui4_idx = 0; (ui4_idx < ui4_num_args) && (ui4_idx <MAX_ARG_IDX); ui4_idx ++)
    {
        if ((pt_args [ui4_idx].e_type & ARG_MASK_TYPE) == ARG_TYPE_VARIABLE)
        {
            e_type = bt_rpc_arg_type (t_rpc_id, ui4_idx);

            if ((e_type & ARG_MASK_TYPE) != ARG_TYPE_VOID)
            {
                pt_args[ui4_idx].e_type = e_type;
            }
            else
            {
                RPC_INFO("RPCR_INV_ARGS, L%d\n", __LINE__);
                return RPCR_INV_ARGS;
            }
        }
    }

    return RPCR_OK;

}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Update database to apply that some link is a 32bit value
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _update_ref_vals(RPC_DB_T * pt_db)
{
    RPC_RGN_NODE_T *    pt_node;
    RPC_DESC_ENTRY_T *  pt_entry;
    RPC_REF_VAL_T *     pt_vr;
    VOID *              pv_chld;

    for (pt_vr = pt_db->pt_ref_val_head; pt_vr != NULL; pt_vr = pt_vr->pt_next)
    {
        pt_node = _find_overlapped(pt_db, pt_vr->pv_addr, 1, NULL, NULL);
        if(pt_node != NULL)
        {
            FOREACH_RPC_ENTRY(pt_node->pt_desc, pt_entry)
            {
                pv_chld = (CHAR *)pt_node->pv_addr + pt_entry->u.z_offs;
                if (pv_chld == pt_vr->pv_addr)
                {
                    _desc_remove_entry(pt_node->pt_desc, pt_entry);
                    break;
                }
            }
        }
    }

    return RPCR_OK;
}
#if 0
/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Update database to apply that some link is a 32bit value
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _update_ref_out(RPC_DB_T * pt_db)
{
    RPC_RGN_NODE_T *    pt_node;
    RPC_DESC_ENTRY_T *  pt_entry;
    RPC_REF_OUT_T *     pt_out;
    VOID *              pv_chld;

    for (pt_out = pt_db->pt_ref_out_head; pt_out != NULL; pt_out = pt_out->pt_next)
    {
        pt_node = _find_overlapped(pt_db, pt_out->pv_addr, 1, NULL, NULL);
        if(pt_node != NULL)
        {
            FOREACH_RPC_ENTRY(pt_node->pt_desc, pt_entry)
            {
                pv_chld = (CHAR *)pt_node->pv_addr + pt_entry->u.z_offs;
                if (pv_chld == pt_out->pv_addr)
                {
                    pt_entry->e_type |= ARG_DIR_OUT;
                    break;
                }
            }
        }
    }

    return RPCR_OK;
}
#endif
/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Delete respecified value information.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _delete_ref_vals(RPC_DB_T *   pt_db)
{
    RPC_REF_VAL_T *     pt_vr;
    while (pt_db->pt_ref_val_head)
    {
        pt_vr = pt_db->pt_ref_val_head->pt_next;
        RPC_FREE(pt_db->pt_ref_val_head);
        pt_db->pt_ref_val_head = pt_vr;
    }
}

#if 0
/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Delete respecified out information.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _delete_ref_out(RPC_DB_T *   pt_db)
{
    RPC_REF_OUT_T *     pt_out;
    while (pt_db->pt_ref_out_head)
    {
        pt_out = pt_db->pt_ref_out_head->pt_next;
        RPC_FREE(pt_db->pt_ref_out_head);
        pt_db->pt_ref_out_head = pt_out;
    }
}
#endif
/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Create and init a rpc database.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static RPC_DB_T * _create_db()
{
    RPC_DB_T  * pt_db;

    pt_db = RPC_MALLOC(sizeof(RPC_DB_T));
    if(pt_db != NULL)
    {
        pt_db->pt_root = NULL;
        pt_db->pt_ref_val_head = NULL;
#if 0
        pt_db->pt_ref_out_head = NULL;
#endif
        _clear_arg(pt_db);
    }
    return pt_db;
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Get database for current thread
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static RPC_DB_T * _get_db(RPC_ID_T t_rpc_id)
{
    if((t_rpc_id < 0)||(pv_db_th_key == NULL))
	return NULL;

    RPC_DB_T * pt_db;

    pt_db = rpcu_os_tls_get(pv_db_th_key);

    /*TODO: Check the context */
    if (pt_db == NULL)
    {
        pt_db = _create_db();
        rpcu_os_tls_set(pv_db_th_key, pt_db);
    }

    return pt_db;
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Delete database.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _delete_db(RPC_DB_T  * pt_db)
{
    _delete_tree(pt_db);

    _delete_ref_vals(pt_db);

    rpcu_os_tls_set(pv_db_th_key, NULL);

    RPC_FREE(pt_db);

}
/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Specify that a reference contains a 32bit value
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _add_ref_val (
    RPC_DB_T *   pt_db,
    const VOID*  pv_ref)
{
    RPC_REF_VAL_T * pt_rv;

    pt_rv = RPC_MALLOC(sizeof(RPC_REF_VAL_T));
    if(pt_rv == NULL)
    {
        return RPCR_OUTOFMEMORY;
    }

    pt_rv->pv_addr          = pv_ref;

    pt_rv->pt_next          = pt_db->pt_ref_val_head;
    pt_db->pt_ref_val_head  = pt_rv;

    return RPCR_OK;
}

#if 0
/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Specify that a reference contains a 32bit value
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _add_ref_out (
    RPC_DB_T *   pt_db,
    const VOID*  pv_ref)
{
    RPC_REF_OUT_T * pt_out;

    pt_out = RPC_MALLOC(sizeof(RPC_REF_OUT_T));
    if(pt_out == NULL)
    {
        return RPCR_OUTOFMEMORY;
    }

    pt_out->pv_addr         = pv_ref;

    pt_out->pt_next         = pt_db->pt_ref_val_head;
    pt_db->pt_ref_out_head  = pt_out;

    return RPCR_OK;
}
#endif

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Process RPC cb request from IPC callback
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _proc_rpc_cb (
    IPC_ID_T        t_ipc_id,
    const CHAR*     ps_cb_type,
    VOID*           pv_cb_addr,
    UINT32          ui4_num_args,
    ARG_DESC_T*     pt_args,
    ARG_DESC_T*     pt_return)
{
    rpc_cb_hndlr_fct    pf_hndlr;
    INT32               i4_ret;

    if((i4_ret = bt_rpc_get_cb_hndlr(ps_cb_type, &pf_hndlr)) != IPCR_OK)
    {
        RPC_ERR("RPC handler not found %s\n", ps_cb_type);
        return i4_ret;
    }

    IPC_ASSERT(t_ipc_id > 0);

    i4_ret = pf_hndlr((RPC_ID_T)t_ipc_id,
                      ps_cb_type,
                      pv_cb_addr,
                      ui4_num_args,
                      pt_args,
                      pt_return);
    /*
     * FIXME: Here tmp ignore this ret value
     */
    return IPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Process RPC OP request from IPC callback
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _proc_rpc_op(
    IPC_ID_T        t_ipc_id,
    const CHAR*     ps_op,
    UINT32          ui4_num_args,
    ARG_DESC_T*     pt_args,
    ARG_DESC_T*     pt_return)
{
    rpc_op_hndlr_fct    pf_hndlr;
    INT32               i4_ret;

    if((i4_ret = bt_rpc_get_op_hndlr(ps_op, &pf_hndlr)) != IPCR_OK)
    {
        RPC_ERR("RPC handler not found %s\n", ps_op);
        return i4_ret;
    }
    IPC_ASSERT(t_ipc_id > 0);

    i4_ret = pf_hndlr((RPC_ID_T)t_ipc_id,
                      ps_op,
                      ui4_num_args,
                      pt_args,
                      pt_return);
    /*
     * FIXME: Here tmp ignore this ret value
     */
    return IPCR_OK;
}
/*------------------------------------------------------------------------
 * Name:
 *
 * Description: This API informs the RPC argument database that the reference
 * specified in argument 'pv_buff' shall be interpreted as a buffer of Bytes.
 * Argument 'z_size' contains the size in number of Bytes of the buffer.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
EXPORT_SYMBOL INT32 bt_rpc_add_ref_buff (
    RPC_ID_T        t_rpc_id,
    const VOID*     pv_buff,
    SIZE_T          z_size)
{
    RPC_DB_T *      pt_db;
    RPC_DESC_T *    pt_desc;
    INT32           i4_ret = IPCR_INV_ARGS;

    ipc_push_logger(t_rpc_id);
    RPC_LOG("Add ref buff :0x%p, size: %zu\n", pv_buff, z_size);

    if(pv_buff == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return RPCR_OK;
    }

    if(z_size == 0)
    {
        ipc_pop_logger(t_rpc_id);
        return IPCR_INV_ARGS;
    }

    if((pt_db = _get_db(t_rpc_id)) == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return RPCR_OUTOFMEMORY;
    }

    if((pt_desc = _create_desc(ARG_TYPE_STRUCT, z_size, 0, FALSE)) == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return RPCR_OUTOFMEMORY;
    }

    i4_ret = _add_addr(pt_db, pv_buff, (const RPC_DESC_T *)pt_desc);

    _free_desc(pt_desc);

    ipc_pop_logger(t_rpc_id);

    return i4_ret;
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: This API informs the RPC argument database that the reference
 * specified in argument 'pv_obj' shall be interpreted as an object as
 * described by argument 'pt_desc'.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
EXPORT_SYMBOL INT32 bt_rpc_add_ref_desc (
    RPC_ID_T            t_rpc_id,
    const VOID *        pv_obj,
    const RPC_DESC_T*   pt_desc,
    const CHAR*         ps_field_name)
{

    RPC_DB_T *          pt_db;
    RPC_DESC_ENTRY_T *  pt_entry;
    RPC_DESC_T *        pt_desc_st;
    RPC_DESC_ENTRY_T *  pt_entry_st;
    INT32               i4_ret;

    ipc_push_logger(t_rpc_id);

    //RPC_LOG("Add ref desc pv_obj:0x%x\n", pv_obj);

    if(pt_desc == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return RPCR_INV_ARGS;
    }

    /* Ignore this */
    if(pv_obj == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return RPCR_OK;
    }

    if((pt_db = _get_db(t_rpc_id)) == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return RPCR_NOT_OPENED;
    }

    i4_ret = IPCR_INV_ARGS;

    _dump_desc(pt_desc, 0, 3);

    if((pt_desc->e_type & ARG_MASK_TYPE) == ARG_TYPE_UNION && ps_field_name != NULL)
    {
        if((pt_desc_st  = _create_desc(ARG_TYPE_STRUCT, pt_desc->z_size, 1, FALSE)) == NULL)
        {
            ipc_pop_logger(t_rpc_id);
            return RPCR_OUTOFMEMORY;
        }
        pt_entry_st = RPC_DESC_GET_ENTRY_START(pt_desc_st);

        FOREACH_RPC_ENTRY(pt_desc, pt_entry)
        {
            if(strcmp(pt_entry->u.ps_field_name, ps_field_name) == 0)
            {
                _entry_set( pt_entry_st,
                            pt_entry->e_type,
                            pt_entry->pt_desc,
                            pt_entry->ui4_num_entries,
                            0);

                i4_ret = _add_addr(pt_db, pv_obj, pt_desc_st);
                break;
            }
        }
        _free_desc(pt_desc_st);

    }
    else
    {
        i4_ret = _add_addr(pt_db, pv_obj, pt_desc);
    }

    ipc_pop_logger(t_rpc_id);
    return i4_ret;

}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: This API informs the RPC argument database that the reference
 * specified in argument 'pv_obj' shall be interpreted as an array of objects
 * as described by argument 'ppt_desc'.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
EXPORT_SYMBOL INT32 bt_rpc_add_ref_desc_arr (
    RPC_ID_T            t_rpc_id,
    UINT32              ui4_num_entries,
    const VOID*         pv_obj,
    const RPC_DESC_T*   pt_desc,
    const CHAR*         ps_field_name)
{

    RPC_DB_T *  pt_db;
    RPC_DESC_T* pt_desc_all;
    INT32       i4_ret = IPCR_INV_ARGS;

    ipc_push_logger(t_rpc_id);
    RPC_LOG("Add ref_desc_arr num:%u pv_obj:%p\n", ui4_num_entries, pv_obj);

    if(   pt_desc == NULL
       || ui4_num_entries == 0
       || pv_obj == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return RPCR_INV_ARGS;
    }

    if((pt_db = _get_db(t_rpc_id)) == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return RPCR_OUTOFMEMORY;
    }
    if(pv_obj == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return IPCR_OK;
    }
    if((pt_desc_all = _create_desc(ARG_TYPE_STRUCT,
                                   pt_desc->z_size * ui4_num_entries,
                                   1,
                                   FALSE)) == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return RPCR_OUTOFMEMORY;
    }

    _entry_set(RPC_DESC_GET_ENTRY_START(pt_desc_all),
               ARG_TYPE_DESC,
               pt_desc,
               ui4_num_entries,
               0);

    i4_ret = _add_addr(pt_db, pv_obj, (const RPC_DESC_T *)pt_desc_all);

    _free_desc(pt_desc_all);

    ipc_pop_logger(t_rpc_id);
    return i4_ret;
}


/*------------------------------------------------------------------------
 * Name:
 *
 * Description: This API informs the RPC argument database that the reference
 * specified in argument 'ps_str' shall be interpreted as a zero terminated string.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 bt_rpc_add_ref_str (
    RPC_ID_T    t_rpc_id,
    const CHAR* ps_str)
{
    RPC_DB_T *   pt_db;
    RPC_DESC_T * pt_desc;
    INT32 i4_ret = IPCR_INV_ARGS;

    ipc_push_logger(t_rpc_id);
    RPC_LOG("Add ref str %p\n", ps_str);

    if(ps_str == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return IPCR_OK;
    }

    if((pt_db = _get_db(t_rpc_id)) == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return RPCR_OUTOFMEMORY;
    }

    if((pt_desc = _create_desc(ARG_TYPE_STRUCT, strlen(ps_str) + 1, 0, FALSE)) == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return RPCR_OUTOFMEMORY;
    }

    i4_ret = _add_addr(pt_db, (VOID *)ps_str, pt_desc);

    _free_desc(pt_desc);

    ipc_pop_logger(t_rpc_id);
    return i4_ret;

}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: This API informs the RPC argument database that the value
 * referenced by argument 'pv_ref' shall be interpreted as a 32 bit value.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 bt_rpc_add_ref_val (
    RPC_ID_T     t_rpc_id,
    const VOID*  pv_ref)
{
    RPC_DB_T *      pt_db;
    INT32           i4_ret;
    ipc_push_logger(t_rpc_id);
    RPC_LOG("Add ref val %p\n", pv_ref);
    if(pv_ref == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return IPCR_INV_ARGS;
    }

    if((pt_db = _get_db(t_rpc_id)) == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return RPCR_OUTOFMEMORY;
    }

   i4_ret = _add_ref_val(pt_db, pv_ref);
   ipc_pop_logger(t_rpc_id);

   return i4_ret;
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: This API informs the RPC argument database that an argument
 *              is of a specific type. This API is used in situations where
 *              a function argument type may differ and cannot be described
 *              by 'ARG_DESC_T'. Note that the corresponding argument
 *              'ARG_DESC_T' entry must be of type 'ARG_TYPE_VARIABLE'.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 bt_rpc_add_arg_type (
    RPC_ID_T    t_rpc_id,
    UINT32      ui4_arg_idx,
    ARG_TYPE_T  e_type)
{
    RPC_DB_T * pt_db;
    ipc_push_logger(t_rpc_id);
    RPC_LOG("Add arg type IDX:%u T:%u\n", ui4_arg_idx, e_type);

    if(   ui4_arg_idx > MAX_ARG_IDX
       || (e_type & ARG_MASK_TYPE) == ARG_TYPE_VARIABLE
       || (e_type & ARG_MASK_TYPE) == ARG_TYPE_UNION
       || (e_type & ARG_MASK_TYPE) == ARG_TYPE_STRUCT
       || (e_type & ARG_MASK_TYPE) == ARG_TYPE_VOID)
    {
        ipc_pop_logger(t_rpc_id);
        return IPCR_INV_ARGS;
    }

    if((pt_db = _get_db(t_rpc_id)) == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return RPCR_OUTOFMEMORY;
    }

    pt_db->a_arg_type[ui4_arg_idx] = e_type;

    ipc_pop_logger(t_rpc_id);
    return RPCR_OK;
}

#if 0
/*------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpc_add_ref_out (
    RPC_ID_T     t_rpc_id,
    const VOID*  pv_ref)
{
    RPC_DB_T *      pt_db;

    RPC_LOG("Add ref out 0x%x\n", pv_ref);

    if(pv_ref == NULL)
    {
        return IPCR_INV_ARGS;
    }

    if((pt_db = _get_db(t_rpc_id)) == NULL)
    {
        return RPCR_OUTOFMEMORY;
    }

    return _add_ref_out(pt_db, pv_ref);
}
#endif

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: This API returns argument types specified using API 'bt_rpc_add_arg_type'.
 * In case no argument type was specified or an error occurred this function
 * will return 'ARG_TYPE_VOID'.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
ARG_TYPE_T bt_rpc_arg_type (
    RPC_ID_T  t_rpc_id,
    UINT32    ui4_arg_idx)
{
    RPC_DB_T * pt_db;

    IPC_ASSERT(ui4_arg_idx < MAX_ARG_IDX);

    if((pt_db = _get_db(t_rpc_id)) == NULL)
    {
        /* FIXME. No err return value ?? */
        return ARG_TYPE_VOID;
    }

    return pt_db->a_arg_type[ui4_arg_idx];


}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: This API is called by a client and closes a communication
 * link to the server.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
EXPORT_SYMBOL INT32 bt_rpc_close_client (RPC_ID_T  t_rpc_id)
{
    INT32 i4_ret;
    printf("bt_rpc_close_client\n");
    ipc_push_logger(t_rpc_id);
    i4_ret = IPCR_TO_RPCR(ipc_close_client(t_rpc_id, FALSE));
    ipc_pop_logger(t_rpc_id);
    return i4_ret;
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: This API is called by a server and closes a communication
 * link to all connected clients.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
EXPORT_SYMBOL INT32 bt_rpc_close_server (RPC_ID_T  t_rpc_id)
{
    INT32 i4_ret;
    ipc_push_logger(t_rpc_id);
    i4_ret = IPCR_TO_RPCR(ipc_close_server(t_rpc_id));
    ipc_pop_logger(t_rpc_id);
    return i4_ret;
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: This API is called by a client and deletes the RPC argument database.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
EXPORT_SYMBOL INT32 bt_rpc_del (RPC_ID_T  t_rpc_id)
{
    RPC_DB_T  * pt_db;
    ipc_push_logger(t_rpc_id);
    if((pt_db = _get_db(t_rpc_id)) == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return RPCR_NOT_OPENED;
    }
    _delete_db(pt_db);
    ipc_pop_logger(t_rpc_id);
    return RPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: IPC callback implementation for getting description from DB.
 * We could use a lazy rebulding for tree node's pt_desc linking. When someone
 * searchs the database, a link visted will be rebuilt and updated. Use a rebuilt
 * mark to  node.
 *
 * The desc linkage by entries can be NULL. That is OK.
 * Rebuilding all the links will be at worst O(links*log(objs)).
 * But IPC could get neccessary links but maybe not all. So we keep this
 * link to NULL.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static RPC_DESC_T * _get_desc_cb_impl (
    VOID*    pv_addr,
    VOID*    pv_tag,
    VOID **  ppv_start_addr)
{
    RPC_ID_T            t_rpc_id;
    RPC_DB_T *          pt_db;
    RPC_RGN_NODE_T *    pt_node;

    t_rpc_id = (RPC_ID_T)pv_tag;
    pt_db = _get_db(t_rpc_id);

    if (pt_db == NULL || pv_addr == NULL)
    {
        return NULL;
    }

    pt_node = _find_overlapped(pt_db, pv_addr, 1, NULL, NULL);

    if (pt_node == NULL)
    {
        RPC_ERR("[IPC DB]Query addr 0x%x failed\n", (unsigned)pv_addr);
        *ppv_start_addr = NULL;
        return NULL;
    }
    else
    {
        RPC_DESC_T * pt_desc = (VOID *)pt_node->pt_desc;
        *ppv_start_addr = (VOID *)pt_node->pv_addr;

        RPC_LOG("[IPC DB]Query addr 0x%x, desc:{%u,%u,...}\n",
            (unsigned)pv_addr,
            (unsigned)pt_desc->z_size,
            (unsigned)pt_desc->ui4_num_entries);

        return pt_desc;

    }
}


/*------------------------------------------------------------------------
 * Name:
 *
 * Description: Do callback to client. This function is a synchronous
 *              operation and will only return once the client has completed
 *              the requested asynchronous callback operation.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
EXPORT_SYMBOL INT32 bt_rpc_do_cb (
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID*        pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return,
    UINT32       ui4_timeout)
{
    INT32       i4_ret;
    RPC_DB_T *  pt_db;

    ipc_push_logger(t_rpc_id);
    RPC_LOG(" RPC DO CB %s\n", ps_cb_type);

    if(   ps_cb_type    == NULL
       || pt_return     == NULL
       || (pt_args      == NULL && ui4_num_args != 0)
       || (pt_args      != NULL && ui4_num_args == 0)
       || t_rpc_id      </*=*/ 0)
       //|| pv_cb_addr    == NULL)
    {
        ipc_pop_logger(t_rpc_id);
        return IPCR_INV_ARGS;
    }

    if (pt_args  == NULL) //for klocwork issue
    {
        RPC_ERR(" RPC DO CB %s, args is null\n", ps_cb_type);
        ipc_pop_logger(t_rpc_id);
        return IPCR_INV_ARGS;
    }

    if((pt_db = _get_db(t_rpc_id)) != NULL)
    {
        if(   (i4_ret = _update_ref_vals(pt_db)) == RPCR_OK
#if 0
           && (i4_ret = _update_ref_out(pt_db))  == RPCR_OK
#endif
           && (i4_ret = _update_arg_types(t_rpc_id, ui4_num_args, pt_args))  == RPCR_OK)
        {
            i4_ret = ipc_do_cb((IPC_ID_T)t_rpc_id,
                               ps_cb_type,
                               pv_cb_addr,
                               ui4_num_args,
                               pt_args,
                               pt_return,
                               ui4_timeout,
                               _get_desc_cb_impl,
                               (VOID *)t_rpc_id);
            i4_ret = IPCR_TO_RPCR(i4_ret);
        }
        else
        {
            RPC_ERR("ipc_do_cb %s failed ret:%d, L%d\n", ps_cb_type, i4_ret, __LINE__);
        }
    }
    else
    {
        i4_ret = RPCR_NOT_FOUND;
        RPC_ERR("ipc_do_cb %s failed ret:%d, L%d\n", ps_cb_type, i4_ret, __LINE__);
    }
    if (i4_ret != RPCR_OK)
    {
    	if ( i4_ret != IPCR_TIMEOUT)
        {
        RPC_ERR("ipc_do_cb %s failed ret:%d, L%d\n", ps_cb_type, i4_ret, __LINE__);
    }
    }
    else
    {
    RPC_LOG(" RPC DO CB %s DONE\n", ps_cb_type);
    }
    ipc_pop_logger(t_rpc_id);
    return i4_ret;
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: This API is called by a client and requests the server to
 *              perform an operation as specified in argument 'ps_op'.
 *              This function is a synchronous operation and will only return
 *              once the server has completed the requested operation.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
EXPORT_SYMBOL INT32 bt_rpc_do_op (
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return,
    UINT32       ui4_timeout)
{
    INT32       i4_ret;
    RPC_DB_T *  pt_db;

    ipc_push_logger(t_rpc_id);

    RPC_LOG("YZ RPC DO OP %s\n", ps_op);
    if(   ps_op     == NULL
       || pt_return == NULL
       || (pt_args  == NULL && ui4_num_args != 0)
       || (pt_args  != NULL && ui4_num_args == 0))
    {
        ipc_pop_logger(t_rpc_id);
        return RPCR_INV_ARGS;
    }

    if (pt_args  == NULL) //for klocwork issue
    {
        RPC_ERR(" RPC DO OP %s, args is null\n", ps_op);
        ipc_pop_logger(t_rpc_id);
        return RPCR_INV_ARGS;
    }

    if((pt_db = _get_db(t_rpc_id)) != NULL)
    {
        if(   (i4_ret = _update_ref_vals(pt_db)) == RPCR_OK
#if 0
           && (i4_ret = _update_ref_out(pt_db))  == RPCR_OK
#endif
           && (i4_ret = _update_arg_types(t_rpc_id, ui4_num_args, pt_args))  == RPCR_OK)
        {
            i4_ret = ipc_do_op((IPC_ID_T)t_rpc_id,
                               ps_op,
                               ui4_num_args,
                               pt_args,
                               pt_return,
                               ui4_timeout,
                               _get_desc_cb_impl,
                               (VOID *)t_rpc_id);
            i4_ret = IPCR_TO_RPCR(i4_ret);
        }
    }
    else
    {
        i4_ret = RPCR_NOT_FOUND;
    }
    RPC_LOG("YZ RPC DO OP %s DONE\n", ps_op);

    ipc_pop_logger(t_rpc_id);

    return i4_ret;;
}

#define DECL_TKN(tkn) const char * tkn
#define INIT_TKN(fmt, tkn) do{tkn = fmt;}while(0)
#define READ_TKN(tkn, s_str) \
do{\
    char * __p;\
    while(*(tkn) == ' ')(tkn)++;\
    for(__p = (s_str); *(tkn) != '\0' && *(tkn) != ' '; __p++, (tkn) ++)\
    {\
        *__p = *(tkn);\
    }\
    *__p = '\0';\
    while(*(tkn) == ' ')(tkn)++;\
}while(0)

#define HAS_TKN(tkn) (*(tkn))

#define OPT_PROC(t_vlst, opt_str, opt_type, s_str, var) \
do{ \
    if(strcmp(s_str, opt_str) == 0) \
    { \
        var = (opt_type)va_arg(t_vlst, opt_type); \
    } \
}while(0)

#define MAX_OPT 255

#if 0
/*------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID _close_func(VOID * pv_tag)
{
    /*_delete_db((RPC_ID_T)pv_tag);    */
}
#endif

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: This API is called by a client and opens a communication link
 *              to the server. One client may only open one communication link
 *              with one server. However, one client may open multiple
 *              communication links with different servers.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
EXPORT_SYMBOL RPC_ID_T bt_rpc_open_client (const CHAR*  ps_server, ...)
{
    IPC_ID_T t_id;
    CHAR                s_str[MAX_OPT];
    CHAR                s_server_name[MAX_PATH + 1];
    rpc_log_fct         pf_log_fct = NULL;
    INT32               i4_log_level = RPC_LOG_NONE;
    va_list             t_vlst;
	INT32 ret;

    if(ps_server == NULL)
    {
        return IPCR_INV_ARGS;
    }
    DECL_TKN(tkn);

    memset(&t_vlst, 0, sizeof(va_list));
    va_start(t_vlst, ps_server);

    INIT_TKN(ps_server, tkn);

    READ_TKN(tkn, s_server_name);

    while(HAS_TKN(tkn))
    {
        READ_TKN(tkn, s_str);
        OPT_PROC(t_vlst, RPC_KW_LOG_FUNCTION,  rpc_log_fct,  s_str, pf_log_fct);
        OPT_PROC(t_vlst, RPC_KW_LOG_LEVEL,     INT32,  s_str, i4_log_level);
    }

    va_end(t_vlst);

    if (pf_log_fct != NULL)
    {
        printf("pf_log_fct=%p, i4_log_level=%d\n", pf_log_fct, (int)i4_log_level);
    }
    ret = rpcu_push_logger(pf_log_fct, i4_log_level);
    if (ret < 0) return IPCR_INV_ARGS;

    printf("YZ Open Client:%s\n", ps_server);

    t_id = ipc_open_client(s_server_name, NULL, NULL, NULL, _proc_rpc_cb);

    rpcu_pop_logger( );
    return t_id;
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: This API is called by a server and opens the server's communication port.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
EXPORT_SYMBOL RPC_ID_T bt_rpc_open_server (const CHAR*  ps_server, ...)
{
    IPC_ID_T t_id;
    CHAR                s_str[MAX_OPT];
    CHAR                s_server_name[MAX_PATH + 1];
    rpc_log_fct         pf_log_fct = NULL;
    INT32               i4_log_level = RPC_LOG_NONE;
    va_list             t_vlst;
	INT32 ret;

	if(ps_server == NULL)
    {
        return IPCR_INV_ARGS;
}
    DECL_TKN(tkn);

    memset(&t_vlst, 0, sizeof(va_list));
    va_start(t_vlst, ps_server);

    INIT_TKN(ps_server, tkn);

    READ_TKN(tkn, s_server_name);

    while(HAS_TKN(tkn))
{
        READ_TKN(tkn, s_str);
        OPT_PROC(t_vlst, RPC_KW_LOG_FUNCTION,  rpc_log_fct,  s_str, pf_log_fct);
        OPT_PROC(t_vlst, RPC_KW_LOG_LEVEL,     INT32,  s_str, i4_log_level);
    }

    va_end(t_vlst);

    ret = rpcu_push_logger(pf_log_fct, i4_log_level);
    if (ret < 0) return IPCR_INV_ARGS;

    printf("YZ Open Server:%s\n", ps_server);

    t_id = ipc_open_server(s_server_name, NULL, NULL, NULL, _proc_rpc_op);

    rpcu_pop_logger( );

    return t_id;
}

static VOID _rpc_db_auto_clean(VOID * pv_key)
{
    if(pv_key != NULL)
    {
        _delete_db(pv_key);
    }
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: This API is called by a client or server context and initializes
 *             the RPC component. This function will in turn execute the
 *             function 'ipc_init'.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
EXPORT_SYMBOL INT32 bt_rpc_init(
/* Do we need max ID ? */
    const OS_FNCT_T*  pt_os_fnct)
{
    INT32 i4_ret;

    printf("YZ rpc_init is called\n");

    if(pv_count_mtx == NULL) rpcu_os_mutex_create(&pv_count_mtx);

    rpcu_os_mutex_lock(pv_count_mtx);

    rpcu_init_tls_logger();

    if (ui_rpc_init_cnt == 0)
    {
        ui_rpc_init_cnt++;

    if((i4_ret = bt_rpc_handler_init()) != RPCR_OK)
    {
            rpcu_os_mutex_unlock(pv_count_mtx);
        return IPCR_TO_RPCR(i4_ret);
    }
    }
    else
    {
      printf("YZ rpc_init is already done\n");
#ifdef RPC_ENABLE_MULTI_CLIENT
      rpcu_os_mutex_unlock(pv_count_mtx);
      return RPCR_OK;
#else
//      return RPCR_OK;
#endif
    }
    rpcu_os_mutex_unlock(pv_count_mtx);

    if (NULL == pv_db_th_key)
    {
    if((pv_db_th_key = rpcu_os_new_tls_key(_rpc_db_auto_clean)) == NULL)
    {
        printf("create pv_db_th_key fail!\n");
        return RPCR_OSERR;
    }
    }

    if((i4_ret = ipc_init(36, pt_os_fnct)) != IPCR_OK) //mtk40156 20110408, 24-36
    {
        return IPCR_TO_RPCR(i4_ret);
    }

    printf("YZ RPC Inited\n");
    return RPCR_OK;
}


INT32 bt_rpc_set_opt(RPC_ID_T t_id, const CHAR * ps_opt, ...)
{
    CHAR                s_str[MAX_OPT];
    rpc_log_fct         pf_log_fct = NULL;
    INT32               i4_log_level = RPC_LOG_NONE;
    va_list             t_vlst;

    DECL_TKN(tkn);

    ipc_push_logger(t_id);

    memset(&t_vlst, 0, sizeof(va_list));
    va_start(t_vlst, ps_opt);

    INIT_TKN(ps_opt, tkn);

    while(HAS_TKN(tkn))
    {
        READ_TKN(tkn, s_str);
        OPT_PROC(t_vlst, RPC_KW_LOG_FUNCTION,  rpc_log_fct,  s_str, pf_log_fct);
        OPT_PROC(t_vlst, RPC_KW_LOG_LEVEL,     INT32,  s_str, i4_log_level);
    }

    va_end(t_vlst);

    rpcu_push_logger(pf_log_fct, i4_log_level);
    ipc_update_logger(t_id);

    return RPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:
 *
 *This API is called by a client or server context and uninitializes the RPC component.
 *This function will in turn execute the function 'ipc_uninit'. Note that this function must
 *be called the same number of times 'rpc_init' has been called and will only perform
 *the actual uninitialization when this function has been called the same number of times
 *as 'rpc_init'.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
EXPORT_SYMBOL VOID bt_rpc_uninit()
{
    if(ui_rpc_init_cnt > 0)
    {
        printf("YZ bt_rpc_uninit\n");
        if (pv_count_mtx == NULL)
        {
            printf("pv_count_mtx is NULL!\n");
            IPC_ASSERT(0);
            return;
        }
        rpcu_os_mutex_lock(pv_count_mtx);
        ui_rpc_init_cnt--;
        rpcu_os_mutex_unlock(pv_count_mtx);
        if(ui_rpc_init_cnt == 0)
        {
            bt_rpc_handler_uninit();
            if(pv_db_th_key != NULL)
            {
                rpcu_os_delete_tls_key(pv_db_th_key);
                pv_db_th_key = NULL;
            }
            ipc_uninit();
            if(pv_count_mtx != NULL)
            {
                rpcu_os_mutex_delete(pv_count_mtx);
                pv_count_mtx = NULL;
            }
        }
    }
}

VOID bt_rpc_simple_uninit()
{
    if(ui_rpc_init_cnt > 0)
    {
        printf("bt_rpc_simple_uninit\n");
        if (pv_count_mtx == NULL)
        {
            printf("pv_count_mtx is NULL!\n");
            IPC_ASSERT(0);
            return;
        }
        rpcu_os_mutex_lock(pv_count_mtx);
        ui_rpc_init_cnt--;
        rpcu_os_mutex_unlock(pv_count_mtx);
        if(ui_rpc_init_cnt == 0)
        {
            bt_rpc_handler_uninit();
        }
  }
}

VOID bt_rpcipc_set_log_status(BOOL b_on)
{
    g_b_log = b_on;

    printf("<RPCIPC>log status is: %d\n", g_b_log);

    return;
}


#if 0
/* ------------------------------------------------------------
 * Unit Tests
 *
 * ------------------------------------------------------------
 */

/* Unit test for AVL tree */
void _test_assert_height(RPC_RGN_NODE_T * pt_node)
{
    INT32 i4_hl = HEIGHT(pt_node->pt_left);
    INT32 i4_hr = HEIGHT(pt_node->pt_right);

    IPC_ASSERT(i4_hl - i4_hr == 1 || i4_hl - i4_hr == 0 || i4_hl - i4_hr == -1);
}

void _test_assert_val(RPC_RGN_NODE_T * pt_node)
{
    if(pt_node->pt_left != NULL)
    {
        IPC_ASSERT((UINT32)pt_node->pt_left->pv_addr < (UINT32)pt_node->pv_addr);
    }
    if(pt_node->pt_right != NULL)
    {
        IPC_ASSERT((UINT32)pt_node->pt_right->pv_addr > (UINT32)pt_node->pv_addr);
    }
}
void _test_verify_tree(RPC_RGN_NODE_T * pt_node)
{
    if(pt_node == NULL)
    {
        return;
    }
    _test_assert_height(pt_node);
    _test_assert_val(pt_node);
    _test_verify_tree(pt_node->pt_left);
    _test_verify_tree(pt_node->pt_right);
}

void test_avl()
{
    RPC_DB_T * pt_db = _create_db();
    RPC_RGN_NODE_T * pt_node;
    RPC_RGN_NODE_T * ppt_path[MAX_HEIGHT];
    UINT32  ui4_num;
    UINT32 i;
    UINT32 j;

    UINT32 aui4_test_val[] = {
        12,10,8,6,4,9,20,21,22,24,23,50,56,38,100,
        101, 110, 102, 130, 1000, 1003, 1004, 1015, 1006, 1009};

    for(i = 0 ;i < sizeof(aui4_test_val)/sizeof(VOID*); i ++)
    {
        RPC_LOG("Add %u\n", (unsigned)aui4_test_val[i]);
        pt_node = _create_node((VOID *)aui4_test_val[i], NULL);

        _add_node(pt_db, pt_node);

        _test_verify_tree(pt_db->pt_root);
        pt_node = _find_node(pt_db, (VOID *)aui4_test_val[i], NULL, NULL);

        IPC_ASSERT(pt_node != NULL);
        IPC_ASSERT(pt_node->pv_addr == (VOID *)aui4_test_val[i]);


    }

    for(i = 0 ;i < sizeof(aui4_test_val)/sizeof(VOID*); i ++)
    {
        pt_node = _find_node(pt_db, (VOID *)aui4_test_val[i], ppt_path, &ui4_num);

        _remove_node(pt_db, pt_node, ppt_path, ui4_num);

        _test_verify_tree(pt_db->pt_root);

        pt_node = _find_node(pt_db, (VOID *)aui4_test_val[i], NULL, NULL);

        IPC_ASSERT(pt_node == NULL);

        for(j = i + 1; j < sizeof(aui4_test_val)/sizeof(VOID*); j ++)
        {
            pt_node = _find_node(pt_db, (VOID *)aui4_test_val[j], NULL, NULL);
            IPC_ASSERT(pt_node != NULL);
        }
        RPC_LOG(".");
    }

    RPC_LOG("Test successful\n");
}

/*------------------------------------------------------------------------
 * Name:
 *
 * Description: This API is called by a client and set the client close func and tag.
 *
 * Inputs:
 * Outputs: -
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpc_set_close_fct(RPC_ID_T t_ipc_id, void * pv_tag, ipc_close_fct pf_close)
{
    ipc_set_close_fct(t_ipc_id, pv_tag, pf_close);

    return RPCR_OK;
}
#endif
