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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>

#include "util.h"

int util_get_time(util_time *t)
{
    int res;
    struct timeval tv;
    res = gettimeofday(&tv, NULL);
    t->sec = tv.tv_sec;
    t->usec = tv.tv_usec;
    return res;
}

void *util_zalloc(int size)
{
    void *ptr = malloc(size);
    if (ptr)
        memset(ptr, 0, (size_t) size);
    return ptr;
}

int util_strlcpy(char *dest, const char *src, int size)
{
    const char *s = src;
    int left = size;

    if (left)
    {
        /* Copy string up to the maximum size of the dest buffer */
        while (--left != 0)
        {
            if ((*dest++ = *s++) == '\0')
                break;
        }
    }

    if (left == 0)
    {
        /* Not enough room for the string; force NUL-termination */
        if (size != 0)
            *dest = '\0';
        while (*s++)
            ; /* determine total src string length */
    }

    return s - src - 1;
}


int util_tokenize_cmd(char *cmd, char *argv[], int max_args)
{
    char *pos, *pos2;
    int argc = 0;

    pos = cmd;
    while (1)
    {
        // Trim the space characters.
        while (*pos == ' ')
        {
            pos++;
        }

        if (*pos == '\0')
        {
            break;
        }

        // One token may start with '"' or other characters.
        if (*pos == '"' && (pos2 = strchr(pos, '"')))
        {
            argv[argc++] = pos + 1;
            *pos2 = '\0';
            pos = pos2 + 1;

        }
        else
        {
            argv[argc++] = pos;
            while (*pos != '\0' && *pos != ' ')
            {
                pos++;
            }
            *pos++ = '\0';
        }

        // Check if the maximum of arguments is reached.
        if (argc == max_args)
        {
            break;
        }
    }

    return argc;
}

#if 0
void util_btaddr_stoh(char *btaddr_s, bt_bdaddr_t *bdaddr_h)
{
    int i;
    for (i = 0; i <6; i++)
    {
        bdaddr_h->address[i] = strtoul(btaddr_s, &btaddr_s, 16);
        btaddr_s++;
    }
}
#endif

int util_ascii_2_hex(char *p_ascii, int len, char *p_hex)
{
    int x;
    char c;
    if (NULL == p_ascii || NULL == p_hex)
    {
        return -1;
    }
    for (x = 0; (x < len) && (*p_ascii); x++)
    {
        if (isdigit (*p_ascii))
            c = (*p_ascii - '0') << 4;
        else
            c = (toupper(*p_ascii) - 'A' + 10) << 4;
        p_ascii++;
        if (*p_ascii)
        {
            if (isdigit (*p_ascii))
                c |= (*p_ascii - '0');
            else
                c |= (toupper(*p_ascii) - 'A' + 10);
            p_ascii++;
        }
        *p_hex++ = c;
    }
    return x;
}

