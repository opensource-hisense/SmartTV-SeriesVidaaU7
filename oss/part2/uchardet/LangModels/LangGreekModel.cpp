/*----------------------------------------------------------------------------*
 *MediaTek Inc. (C) 2019. All rights reserved.                                *
 *Copyright Statement:                                                        *
 *This software/firmware and related documentation ("MediaTek Software") are  *
 *protected under relevant copyright laws. The information contained herein is*
 *confidential and proprietary to MediaTek Inc. and/or its licensors. Without *
 *the prior written permission of MediaTek inc. and/or its licensors, any     *
 *reproduction, modification, use or disclosure of MediaTek Software, and     *
 *information contained herein, in whole or in part, shall be strictly        *
 *prohibited.                                                                 *
 *BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES *
 *THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")     *
 *RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER  *
 *ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL          *
 *WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED    *
 *WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR          *
 *NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH *
 *RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,            *
 *INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES*
 *TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.   *
 *RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO*
 *OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK       *
 *SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE  *
 *RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR     *
 *STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S *
 *ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE       *
 *RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE  *
 *MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE  *
 *CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.    *
 *The following software/firmware and/or related documentation ("MediaTek     *
 *Software") have been modified by MediaTek Inc. All revisions are subject to *
 *any receiver's applicable license agreements with MediaTek Inc.             *
 *---------------------------------------------------------------------------*/

/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla Communicator client code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "../nsSBCharSetProber.h"

/********* Language model for: Greek *********/

/**
 * Generated by BuildLangModel.py
 * On: 2016-05-25 15:21:50.073117
 **/

/* Character Mapping Table:
 * ILL: illegal character.
 * CTR: control character specific to the charset.
 * RET: carriage/return.
 * SYM: symbol (punctuation) that does not belong to word.
 * NUM: 0 - 9.
 *
 * Other characters are ordered by probabilities
 * (0 is the most common character in the language).
 *
 * Orders are generic to a language. So the codepoint with order X in
 * CHARSET1 maps to the same character as the codepoint with the same
 * order X in CHARSET2 for the same language.
 * As such, it is possible to get missing order. For instance the
 * ligature of 'o' and 'e' exists in ISO-8859-15 but not in ISO-8859-1
 * even though they are both used for French. Same for the euro sign.
 */
static const unsigned char Windows_1253_CharToOrderMap[] =
{
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,RET,CTR,CTR,RET,CTR,CTR, /* 0X */
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR, /* 1X */
  SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM, /* 2X */
  NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,SYM,SYM,SYM,SYM,SYM,SYM, /* 3X */
  SYM, 32, 46, 41, 40, 30, 52, 48, 42, 33, 56, 49, 39, 44, 36, 34, /* 4X */
   47, 59, 35, 38, 37, 43, 54, 50, 58, 53, 57,SYM,SYM,SYM,SYM,SYM, /* 5X */
  SYM, 32, 46, 41, 40, 30, 52, 48, 42, 33, 56, 49, 39, 44, 36, 34, /* 6X */
   47, 59, 35, 38, 37, 43, 54, 50, 58, 53, 57,SYM,SYM,SYM,SYM,CTR, /* 7X */
  SYM,ILL,SYM,SYM,SYM,SYM,SYM,SYM,ILL,SYM,ILL,SYM,ILL,ILL,ILL,ILL, /* 8X */
  ILL,SYM,SYM,SYM,SYM,SYM,SYM,SYM,ILL,SYM,ILL,SYM,ILL,ILL,ILL,ILL, /* 9X */
  SYM,SYM, 17,SYM,SYM,SYM,SYM,SYM,SYM,SYM,ILL,SYM,SYM,SYM,SYM,SYM, /* AX */
  SYM,SYM,SYM,SYM,SYM, 62,SYM,SYM, 19, 22, 15,SYM, 16,SYM, 24, 28, /* BX */
   55,  0, 25, 18, 20,  5, 29, 10, 26,  3,  8, 14, 13,  4, 31,  1, /* CX */
   11,  6,ILL,  7,  2, 12, 27, 23, 45, 21, 51, 60, 17, 19, 22, 15, /* DX */
   61,  0, 25, 18, 20,  5, 29, 10, 26,  3,  8, 14, 13,  4, 31,  1, /* EX */
   11,  6,  9,  7,  2, 12, 27, 23, 45, 21, 51, 60, 16, 24, 28,ILL, /* FX */
};
/*X0  X1  X2  X3  X4  X5  X6  X7  X8  X9  XA  XB  XC  XD  XE  XF */

static const unsigned char Iso_8859_7_CharToOrderMap[] =
{
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,RET,CTR,CTR,RET,CTR,CTR, /* 0X */
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR, /* 1X */
  SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM, /* 2X */
  NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,SYM,SYM,SYM,SYM,SYM,SYM, /* 3X */
  SYM, 32, 46, 41, 40, 30, 52, 48, 42, 33, 56, 49, 39, 44, 36, 34, /* 4X */
   47, 59, 35, 38, 37, 43, 54, 50, 58, 53, 57,SYM,SYM,SYM,SYM,SYM, /* 5X */
  SYM, 32, 46, 41, 40, 30, 52, 48, 42, 33, 56, 49, 39, 44, 36, 34, /* 6X */
   47, 59, 35, 38, 37, 43, 54, 50, 58, 53, 57,SYM,SYM,SYM,SYM,CTR, /* 7X */
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR, /* 8X */
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR, /* 9X */
  SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,ILL,SYM, /* AX */
  SYM,SYM,SYM,SYM,SYM,SYM, 17,SYM, 19, 22, 15,SYM, 16,SYM, 24, 28, /* BX */
   55,  0, 25, 18, 20,  5, 29, 10, 26,  3,  8, 14, 13,  4, 31,  1, /* CX */
   11,  6,ILL,  7,  2, 12, 27, 23, 45, 21, 51, 60, 17, 19, 22, 15, /* DX */
   61,  0, 25, 18, 20,  5, 29, 10, 26,  3,  8, 14, 13,  4, 31,  1, /* EX */
   11,  6,  9,  7,  2, 12, 27, 23, 45, 21, 51, 60, 16, 24, 28,ILL, /* FX */
};
/*X0  X1  X2  X3  X4  X5  X6  X7  X8  X9  XA  XB  XC  XD  XE  XF */


/* Model Table:
 * Total sequences: 1579
 * First 512 sequences: 0.958419074626211
 * Next 512 sequences (512-1024): 0.03968891876305471
 * Rest: 0.0018920066107342773
 * Negative sequences: TODO
 */
static const PRUint8 GreekLangModel[] =
{
  1,3,3,3,3,3,3,3,3,3,2,3,3,3,3,3,2,2,3,2,3,1,2,
   3,3,3,3,3,1,3,0,3,0,0,0,0,0,0,1,0,0,1,0,0,0,2,
  2,2,3,3,3,3,3,3,3,3,2,3,3,3,3,3,1,2,3,2,3,1,2,
   3,3,3,3,3,2,2,0,2,0,0,0,0,0,0,0,0,1,0,0,1,0,2,
  3,3,2,3,2,3,3,3,2,3,3,1,3,2,2,3,3,3,2,3,0,3,3,
   2,2,2,2,2,3,3,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,
  3,3,3,2,3,3,3,3,3,3,3,3,1,3,3,1,3,3,3,3,3,3,2,
   3,1,3,3,2,3,3,0,2,0,0,1,0,0,0,1,0,0,0,0,0,0,2,
  3,3,3,3,3,3,2,3,2,2,3,1,2,2,2,3,3,3,3,3,3,3,3,
   2,2,1,3,2,3,2,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
  2,3,3,3,3,2,3,3,3,3,2,3,3,3,3,3,2,2,3,1,3,3,1,
   3,3,3,3,3,2,2,0,3,0,0,0,0,0,0,1,0,0,0,0,0,0,2,
  3,3,3,3,3,3,3,3,3,2,3,2,3,3,3,3,3,3,3,3,3,3,3,
   3,3,2,3,2,3,2,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,1,
  3,3,3,3,2,3,2,3,3,0,3,3,3,3,2,3,3,3,2,3,2,3,3,
   3,3,2,2,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  3,3,3,3,2,3,3,2,3,2,3,2,3,2,3,3,3,3,1,3,3,3,3,
   2,3,2,2,2,3,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,
  1,1,0,1,1,1,0,1,1,0,2,1,0,1,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
  1,1,3,0,3,2,3,3,3,3,0,3,0,3,3,1,0,0,3,1,2,0,0,
   2,1,1,3,2,0,0,0,2,0,0,1,0,0,0,0,0,0,1,0,0,0,2,
  3,3,3,3,2,3,3,2,1,1,3,2,3,1,3,3,3,3,1,3,0,3,3,
   1,2,1,1,1,2,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
  3,2,3,2,3,2,3,3,3,3,2,3,0,3,3,2,2,3,3,2,3,1,2,
   3,0,3,3,2,1,3,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,2,
  3,3,1,3,2,3,1,2,1,2,3,3,2,3,1,3,3,3,1,3,1,3,3,
   1,2,3,0,3,2,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,2,
  3,3,3,3,2,3,1,2,2,2,3,2,3,3,3,3,3,3,2,3,2,3,3,
   2,3,2,2,2,3,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,
  3,3,3,1,3,3,3,3,3,3,2,3,0,3,3,0,0,0,3,0,3,3,0,
   3,0,2,3,2,0,3,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
  2,2,3,2,3,3,3,3,3,3,2,3,1,3,3,0,0,0,3,0,3,1,0,
   3,1,2,2,3,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
  2,2,3,3,3,2,3,3,3,3,2,3,1,3,3,0,0,0,3,0,3,1,0,
   3,0,3,3,3,0,3,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
  3,3,0,3,3,3,3,0,3,0,3,0,2,3,3,3,3,3,3,3,2,3,3,
   2,2,0,0,0,3,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  3,3,3,3,3,2,3,3,3,3,2,3,1,3,3,0,0,0,3,0,3,3,0,
   3,0,3,3,3,0,2,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
  3,3,1,3,2,3,3,1,0,0,3,0,3,1,0,3,3,3,0,3,0,3,3,
   0,3,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  2,1,3,2,3,1,3,3,2,3,1,3,1,3,2,2,1,2,3,1,2,0,2,
   2,0,3,3,2,1,1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,3,1,3,1,3,3,3,3,1,2,0,3,3,0,0,0,2,0,2,1,0,
   2,0,1,3,2,0,1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
  3,3,3,3,3,3,3,1,0,1,3,1,2,2,2,3,2,3,0,3,0,3,3,
   0,2,1,3,1,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  2,3,3,2,3,3,3,3,2,3,2,3,0,3,3,0,0,0,3,0,2,1,0,
   2,0,2,3,2,0,2,0,2,0,0,0,0,0,1,0,0,0,0,0,0,0,2,
  3,3,1,3,2,3,3,1,1,1,2,1,2,0,3,3,3,3,2,3,2,2,2,
   0,2,2,0,0,2,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
  3,3,0,3,3,3,3,1,1,0,3,0,3,3,3,2,2,3,1,3,0,2,3,
   0,2,0,0,1,3,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  3,3,2,3,1,3,3,3,2,0,3,1,3,1,2,3,3,3,2,3,0,3,3,
   0,2,0,2,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,1,3,2,3,0,3,3,2,3,2,3,0,3,2,0,0,0,1,0,2,1,0,
   1,0,2,2,1,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  3,3,1,3,1,3,1,1,1,0,2,0,2,2,1,2,2,2,1,2,0,3,2,
   0,2,1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,1,0,0,1,0,1,0,0,0,0,1,1,0,1,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,2,1,2,2,2,3,3,2,3,2,2,2,2,2,2,0,
  3,3,1,3,1,3,0,0,1,0,3,1,2,1,1,2,2,3,1,2,0,2,2,
   0,3,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,1,0,1,1,0,0,0,1,0,0,1,1,1,1,0,0,0,1,0,0,0,0,
   0,0,0,1,1,0,0,2,0,2,2,1,3,3,3,2,3,2,2,2,2,2,0,
  0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
   0,0,1,0,0,0,0,2,0,3,2,3,2,3,3,3,2,2,3,1,2,2,0,
  0,0,1,0,1,0,0,1,0,1,0,0,1,1,0,0,0,0,0,0,1,0,0,
   0,1,0,1,0,0,0,2,0,2,2,2,3,3,2,2,2,2,2,2,2,2,0,
  0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,1,0,0,0,3,0,3,3,3,2,2,2,2,2,2,2,1,2,2,0,
  0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,
   0,0,0,0,0,0,0,3,0,3,2,2,1,2,2,2,2,3,2,1,2,1,0,
  1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,
   0,0,0,0,0,0,1,3,0,3,3,3,2,1,2,2,2,1,1,3,2,2,0,
  0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,2,0,2,2,2,1,1,3,2,2,1,2,2,2,2,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,3,0,3,3,2,1,1,2,2,2,2,1,1,2,2,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,3,0,2,2,2,2,1,1,2,2,1,2,1,2,1,0,
  0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,2,0,2,2,2,2,1,2,1,2,2,2,3,2,1,0,
  0,0,0,0,0,0,0,0,0,0,0,1,0,2,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,3,0,2,2,2,2,2,2,1,2,1,1,1,2,2,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,2,0,2,2,1,2,2,2,2,2,2,2,1,1,2,0,
  1,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,
   0,0,0,0,0,0,0,3,0,2,2,2,1,1,1,2,2,1,1,1,2,2,0,
  2,2,0,2,0,3,0,0,0,0,3,0,2,0,0,2,1,1,0,1,0,1,2,
   0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};


const SequenceModel Windows_1253GreekModel =
{
  Windows_1253_CharToOrderMap,
  GreekLangModel,
  46,
  (float)0.958419074626211,
  PR_FALSE,
  "WINDOWS-1253"
};

const SequenceModel Iso_8859_7GreekModel =
{
  Iso_8859_7_CharToOrderMap,
  GreekLangModel,
  46,
  (float)0.958419074626211,
  PR_FALSE,
  "ISO-8859-7"
};
