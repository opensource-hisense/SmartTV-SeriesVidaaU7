
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

/********* Language model for: Romanian *********/

/**
 * Generated by BuildLangModel.py
 * On: 2016-09-28 18:58:13.757152
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
static const unsigned char Iso_8859_16_CharToOrderMap[] =
{
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,RET,CTR,CTR,RET,CTR,CTR, /* 0X */
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR, /* 1X */
  SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM, /* 2X */
  NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,SYM,SYM,SYM,SYM,SYM,SYM, /* 3X */
  SYM,  2, 17,  9, 11,  0, 16, 15, 23,  1, 26, 27,  6, 12,  4,  8, /* 4X */
   13, 32,  3, 10,  5,  7, 21, 29, 25, 28, 22,SYM,SYM,SYM,SYM,SYM, /* 5X */
  SYM,  2, 17,  9, 11,  0, 16, 15, 23,  1, 26, 27,  6, 12,  4,  8, /* 6X */
   13, 32,  3, 10,  5,  7, 21, 29, 25, 28, 22,SYM,SYM,SYM,SYM,CTR, /* 7X */
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR, /* 8X */
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR, /* 9X */
  SYM, 60, 61, 46,SYM,SYM, 38,SYM, 38,SYM, 19,SYM, 62,SYM, 63, 64, /* AX */
  SYM,SYM, 41, 46, 40,SYM,SYM,SYM, 40, 41, 19,SYM, 65, 66, 67, 68, /* BX */
   69, 30, 24, 14, 33, 35, 53, 42, 45, 31, 58, 49, 70, 37, 20, 48, /* CX */
   43, 52, 59, 34, 71, 44, 36, 56, 50, 72, 47, 73, 39, 74, 18, 57, /* DX */
   75, 30, 24, 14, 33, 35, 53, 42, 45, 31, 58, 49, 76, 37, 20, 48, /* EX */
   43, 52, 59, 34, 77, 44, 36, 56, 50, 78, 47, 79, 39, 80, 18, 81, /* FX */
};
/*X0  X1  X2  X3  X4  X5  X6  X7  X8  X9  XA  XB  XC  XD  XE  XF */

static const unsigned char Iso_8859_2_CharToOrderMap[] =
{
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,RET,CTR,CTR,RET,CTR,CTR, /* 0X */
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR, /* 1X */
  SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM, /* 2X */
  NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,SYM,SYM,SYM,SYM,SYM,SYM, /* 3X */
  SYM,  2, 17,  9, 11,  0, 16, 15, 23,  1, 26, 27,  6, 12,  4,  8, /* 4X */
   13, 32,  3, 10,  5,  7, 21, 29, 25, 28, 22,SYM,SYM,SYM,SYM,SYM, /* 5X */
  SYM,  2, 17,  9, 11,  0, 16, 15, 23,  1, 26, 27,  6, 12,  4,  8, /* 6X */
   13, 32,  3, 10,  5,  7, 21, 29, 25, 28, 22,SYM,SYM,SYM,SYM,CTR, /* 7X */
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR, /* 8X */
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR, /* 9X */
  SYM, 82,SYM, 46,SYM, 83, 56,SYM,SYM, 38, 84, 85, 86,SYM, 40, 87, /* AX */
  SYM, 88,SYM, 46,SYM, 89, 56,SYM,SYM, 38, 90, 91, 92,SYM, 40, 93, /* BX */
   94, 30, 24, 14, 33, 95, 35, 42, 41, 31, 96, 49, 51, 37, 20, 97, /* CX */
   43, 52, 98, 34, 99, 44, 36,SYM, 55,100, 47, 50, 39, 54,101, 57, /* DX */
  102, 30, 24, 14, 33,103, 35, 42, 41, 31,104, 49, 51, 37, 20,105, /* EX */
   43, 52,106, 34,107, 44, 36,SYM, 55,108, 47, 50, 39, 54,109,SYM, /* FX */
};
/*X0  X1  X2  X3  X4  X5  X6  X7  X8  X9  XA  XB  XC  XD  XE  XF */

static const unsigned char Windows_1250_CharToOrderMap[] =
{
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,RET,CTR,CTR,RET,CTR,CTR, /* 0X */
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR, /* 1X */
  SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM, /* 2X */
  NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,SYM,SYM,SYM,SYM,SYM,SYM, /* 3X */
  SYM,  2, 17,  9, 11,  0, 16, 15, 23,  1, 26, 27,  6, 12,  4,  8, /* 4X */
   13, 32,  3, 10,  5,  7, 21, 29, 25, 28, 22,SYM,SYM,SYM,SYM,SYM, /* 5X */
  SYM,  2, 17,  9, 11,  0, 16, 15, 23,  1, 26, 27,  6, 12,  4,  8, /* 6X */
   13, 32,  3, 10,  5,  7, 21, 29, 25, 28, 22,SYM,SYM,SYM,SYM,CTR, /* 7X */
  SYM,ILL,SYM,ILL,SYM,SYM,SYM,SYM,ILL,SYM, 38,SYM, 56,110, 40,111, /* 8X */
  ILL,SYM,SYM,SYM,SYM,SYM,SYM,SYM,ILL,SYM, 38,SYM, 56,112, 40,113, /* 9X */
  SYM,SYM,SYM, 46,SYM,114,SYM,SYM,SYM,SYM,115,SYM,SYM,SYM,SYM,116, /* AX */
  SYM,SYM,SYM, 46,SYM,SYM,SYM,SYM,SYM,117,118,SYM,119,SYM,120,121, /* BX */
  122, 30, 24, 14, 33,123, 35, 42, 41, 31,124, 49, 51, 37, 20,125, /* CX */
   43, 52,126, 34,127, 44, 36,SYM, 55,128, 47, 50, 39, 54,129, 57, /* DX */
  130, 30, 24, 14, 33,131, 35, 42, 41, 31,132, 49, 51, 37, 20,133, /* EX */
   43, 52,134, 34,135, 44, 36,SYM, 55,136, 47, 50, 39, 54,137,SYM, /* FX */
};
/*X0  X1  X2  X3  X4  X5  X6  X7  X8  X9  XA  XB  XC  XD  XE  XF */

static const unsigned char Ibm852_CharToOrderMap[] =
{
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,RET,CTR,CTR,RET,CTR,CTR, /* 0X */
  CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR,CTR, /* 1X */
  SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM, /* 2X */
  NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,NUM,SYM,SYM,SYM,SYM,SYM,SYM, /* 3X */
  SYM,  2, 17,  9, 11,  0, 16, 15, 23,  1, 26, 27,  6, 12,  4,  8, /* 4X */
   13, 32,  3, 10,  5,  7, 21, 29, 25, 28, 22,SYM,SYM,SYM,SYM,SYM, /* 5X */
  SYM,  2, 17,  9, 11,  0, 16, 15, 23,  1, 26, 27,  6, 12,  4,  8, /* 6X */
   13, 32,  3, 10,  5,  7, 21, 29, 25, 28, 22,SYM,SYM,SYM,SYM,CTR, /* 7X */
   42, 39, 31, 24, 33,138, 35, 42, 46, 49, 44, 44, 20,139, 33, 35, /* 8X */
   31,140,141,142, 36,143,144, 56, 56, 36, 39,145,146, 46,SYM, 41, /* 9X */
   30, 37, 34, 47,147,148, 40, 40,149,150,SYM,151, 41,152,SYM,SYM, /* AX */
  SYM,SYM,SYM,SYM,SYM, 30, 24, 51,153,SYM,SYM,SYM,SYM,154,155,SYM, /* BX */
  SYM,SYM,SYM,SYM,SYM,SYM, 14, 14,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM, /* CX */
   43, 43,156, 49,157,158, 37, 20, 51,SYM,SYM,SYM,SYM,159,160,SYM, /* DX */
   34, 57,161, 52, 52,162, 38, 38,163, 47,164, 50, 54, 54,165,SYM, /* EX */
  SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM,SYM, 50, 55, 55,SYM,SYM, /* FX */
};
/*X0  X1  X2  X3  X4  X5  X6  X7  X8  X9  XA  XB  XC  XD  XE  XF */


/* Model Table:
 * Total sequences: 981
 * First 512 sequences: 0.997762564143313
 * Next 512 sequences (512-1024): 0.002237435856687006
 * Rest: 3.0357660829594124e-18
 * Negative sequences: TODO
 */
static const PRUint8 RomanianLangModel[] =
{
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,3,3,3,3,3,3,3,3,3,0,3,3,3,3,3,2,0,2,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,3,3,3,3,3,0,3,3,3,2,3,3,3,2,2,0,0,2,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,3,3,3,3,3,0,3,3,3,0,3,3,3,3,3,0,2,2,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,3,3,3,3,2,2,3,3,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,3,3,3,2,3,3,3,3,3,3,0,3,3,3,3,2,3,3,3,3,2,2,2,
  3,3,3,3,3,3,3,3,3,3,3,2,3,3,3,2,3,3,0,2,2,3,3,3,3,0,2,2,3,3,2,3,0,
  3,3,3,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,3,2,2,3,0,3,3,3,2,2,2,0,
  3,3,3,3,3,3,3,2,3,3,3,3,3,3,3,3,3,3,3,3,0,3,3,3,3,3,3,3,2,2,0,2,0,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,3,3,3,3,3,0,3,3,3,0,3,2,3,3,3,2,0,2,
  3,3,3,3,3,3,3,3,3,3,3,3,2,2,3,3,2,2,3,2,0,3,2,3,3,0,3,3,2,2,0,2,2,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,3,3,3,0,2,2,3,3,3,0,2,3,3,3,2,2,2,
  3,3,3,3,3,2,3,3,3,2,3,3,3,2,3,3,2,3,0,0,0,3,2,3,3,0,2,2,3,3,3,2,0,
  3,3,3,2,3,3,3,3,3,3,3,2,3,3,3,2,2,3,3,2,2,2,2,3,3,2,0,0,3,2,2,2,0,
  3,3,3,3,3,3,3,3,3,3,3,2,2,3,3,0,0,2,3,0,2,0,2,3,3,0,2,2,3,0,2,2,0,
  2,3,0,3,3,3,3,3,0,3,3,3,3,3,0,3,0,3,3,3,0,3,3,0,0,0,2,2,0,0,0,0,0,
  3,3,3,3,3,2,3,3,3,0,2,3,3,2,3,3,2,3,0,0,2,3,2,3,3,0,2,0,3,2,2,2,0,
  3,3,3,3,0,3,3,3,3,2,2,2,3,2,3,2,3,0,0,0,0,0,0,2,3,0,0,0,2,0,2,2,0,
  3,3,3,3,3,3,3,3,3,3,3,3,3,2,3,2,2,3,3,2,0,2,2,2,3,0,2,2,3,2,2,2,0,
  3,3,3,0,0,0,0,3,2,2,2,0,0,0,3,0,0,0,0,0,2,2,0,0,2,0,0,2,0,0,0,0,0,
  3,3,3,0,3,3,3,3,3,3,0,2,2,0,3,0,0,0,0,0,0,2,0,0,2,0,0,2,0,0,0,0,0,
  0,3,0,2,3,0,3,0,0,0,0,0,3,0,0,0,0,0,2,3,0,0,2,2,0,0,0,2,0,0,0,0,0,
  3,3,3,3,3,2,3,3,3,2,2,3,2,0,3,2,2,2,0,0,0,0,0,0,3,0,2,2,2,0,2,0,0,
  3,3,3,2,2,2,2,3,3,0,2,3,2,2,3,2,0,3,0,0,0,3,3,2,3,0,0,2,2,0,2,2,0,
  3,3,3,3,3,3,3,3,3,2,3,2,2,2,3,0,2,3,0,0,0,2,2,0,2,0,2,2,3,2,2,2,0,
  0,3,0,3,3,3,3,3,0,2,2,2,3,0,0,0,0,0,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,
  3,3,3,2,0,3,0,3,3,3,2,0,0,3,3,0,3,0,0,0,0,3,0,2,2,3,0,0,3,0,0,0,0,
  3,3,3,2,2,2,3,3,3,0,2,2,2,0,2,0,0,2,0,0,0,2,0,0,2,0,0,2,0,0,2,0,0,
  3,3,3,3,2,3,3,3,3,2,3,2,3,2,2,2,2,2,2,2,2,2,0,3,0,0,0,2,3,2,2,2,0,
  3,2,3,3,3,2,3,2,3,3,3,3,3,2,0,2,0,2,0,0,0,2,2,2,0,0,2,2,0,2,2,0,0,
  3,3,3,2,3,2,2,2,3,2,3,2,2,2,0,0,2,2,0,0,0,0,0,3,0,0,0,0,2,3,0,0,0,
  2,3,0,3,3,2,2,0,0,2,2,2,2,0,0,2,0,0,0,0,0,0,2,0,0,0,0,2,0,0,0,0,0,
  0,3,2,2,2,2,2,0,0,2,2,2,2,2,0,2,0,2,0,0,0,2,2,0,0,0,2,2,0,0,0,0,0,
  0,0,2,0,0,2,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
};


const SequenceModel Iso_8859_16RomanianModel =
{
  Iso_8859_16_CharToOrderMap,
  RomanianLangModel,
  33,
  (float)0.997762564143313,
  PR_TRUE,
  "ISO-8859-16"
};

const SequenceModel Iso_8859_2RomanianModel =
{
  Iso_8859_2_CharToOrderMap,
  RomanianLangModel,
  33,
  (float)0.997762564143313,
  PR_TRUE,
  "ISO-8859-2"
};

const SequenceModel Windows_1250RomanianModel =
{
  Windows_1250_CharToOrderMap,
  RomanianLangModel,
  33,
  (float)0.997762564143313,
  PR_TRUE,
  "WINDOWS-1250"
};

const SequenceModel Ibm852RomanianModel =
{
  Ibm852_CharToOrderMap,
  RomanianLangModel,
  33,
  (float)0.997762564143313,
  PR_TRUE,
  "IBM852"
};