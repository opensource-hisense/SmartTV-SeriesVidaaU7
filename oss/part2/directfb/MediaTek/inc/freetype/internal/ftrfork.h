/*----------------------------------------------------------------------------
 * No Warranty :  Except  as  may  be  otherwise  agreed  to  in  writing, no *
 * warranties  of  any  kind,  whether  express or  implied, are given by MTK *
 * with  respect  to  any  Confidential  Information  or any use thereof, and *
 * the  Confidential  Information  is  provided  on  an  "AS IS"  basis.  MTK *
 * hereby  expressly  disclaims  all  such  warranties, including any implied *
 * warranties   of  merchantability ,  non-infringement  and  fitness  for  a *
 * particular purpose and any warranties arising out of course of performance *
 * course  of dealing or usage of trade.  Parties further acknowledge that in *
 * connection  with  the Purpose, Company may, either presently and/or in the *
 * future,   instruct   MTK   to   assist  it  in  the  development  and  the *
 * implementation,  in accordance with Company's designs, of certain software *
 * relating  to  Company's  product(s)  (the  "Services").   Except as may be *
 * otherwise agreed to in writing, no warranties of any kind, whether express *
 * or  implied,  are  given by MTK with respect to the Services provided, and *
 * the  Services  are  provided  on  an  "AS  IS"  basis.   Company   further *
 * acknowledges  that  the  Services  may  contain  errors,  which testing is *
 * important  and  it  is  solely  responsible for fully testing the Services *
 * and/or   derivatives   thereof   before  they  are  used,  sublicensed  or *
 * distributed.   Should  there  be any third party action brought again MTK, *
 * arising  out  of  or  relating  to  the  Services,  Company agree to fully *
 * indemnify and hold MTK harmless.                                           *
 *---------------------------------------------------------------------------*/
/***************************************************************************/
/*                                                                         */
/*  ftrfork.h                                                              */
/*                                                                         */
/*    Embedded resource forks accessor (specification).                    */
/*                                                                         */
/*  Copyright 2004 by                                                      */
/*  Masatake YAMATO and Redhat K.K.                                        */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/* Development of the code in this file is support of                      */
/* Information-technology Promotion Agency, Japan.                         */
/***************************************************************************/


#ifndef __FTRFORK_H__
#define __FTRFORK_H__


#include <ft2build.h>
#include FT_INTERNAL_OBJECTS_H


FT_BEGIN_HEADER


  /* Number of guessing rules supported in `FT_Raccess_Guess'.            */
  /* Don't forget to increment the number if you add a new guessing rule. */
#define FT_RACCESS_N_RULES  8


  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    FT_Raccess_Guess                                                   */
  /*                                                                       */
  /* <Description>                                                         */
  /*    Guess a file name and offset where the actual resource fork is     */
  /*    stored.  The macro FT_RACCESS_N_RULES holds the number of          */
  /*    guessing rules;  the guessed result for the Nth rule is            */
  /*    represented as a triplet: a new file name (new_names[N]), a file   */
  /*    offset (offsets[N]), and an error code (errors[N]).                */
  /*                                                                       */
  /* <Input>                                                               */
  /*    library ::                                                         */
  /*      A FreeType library instance.                                     */
  /*                                                                       */
  /*    stream ::                                                          */
  /*      A file stream containing the resource fork.                      */
  /*                                                                       */
  /*    base_name ::                                                       */
  /*      The (base) file name of the resource fork used for some          */
  /*      guessing rules.                                                  */
  /*                                                                       */
  /* <Output>                                                              */
  /*    new_names ::                                                       */
  /*      An array of guessed file names in which the resource forks may   */
  /*      exist.  If `new_names[N]' is NULL, the guessed file name is      */
  /*      equal to `base_name'.                                            */
  /*                                                                       */
  /*    offsets ::                                                         */
  /*      An array of guessed file offsets.  `offsets[N]' holds the file   */
  /*      offset of the possible start of the resource fork in file        */
  /*      `new_names[N]'.                                                  */
  /*                                                                       */
  /*    errors ::                                                          */
  /*      An array of FreeType error codes.  `errors[N]' is the error      */
  /*      code of Nth guessing rule function.  If `errors[N]' is not       */
  /*      FT_Err_Ok, `new_names[N]' and `offsets[N]' are meaningless.      */
  /*                                                                       */
  FT_BASE( void )
  FT_Raccess_Guess( FT_Library  library,
                    FT_Stream   stream,
                    char*       base_name,
                    char**      new_names,
                    FT_Long*    offsets,
                    FT_Error*   errors );


  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    FT_Raccess_Get_HeaderInfo                                          */
  /*                                                                       */
  /* <Description>                                                         */
  /*    Get the information from the header of resource fork.  The         */
  /*    information includes the file offset where the resource map        */
  /*    starts, and the file offset where the resource data starts.        */
  /*    `FT_Raccess_Get_DataOffsets' requires these two data.              */
  /*                                                                       */
  /* <Input>                                                               */
  /*    library ::                                                         */
  /*      A FreeType library instance.                                     */
  /*                                                                       */
  /*    stream ::                                                          */
  /*      A file stream containing the resource fork.                      */
  /*                                                                       */
  /*    rfork_offset ::                                                    */
  /*      The file offset where the resource fork starts.                  */
  /*                                                                       */
  /* <Output>                                                              */
  /*    map_offset ::                                                      */
  /*      The file offset where the resource map starts.                   */
  /*                                                                       */
  /*    rdata_pos ::                                                       */
  /*      The file offset where the resource data starts.                  */
  /*                                                                       */
  /* <Return>                                                              */
  /*    FreeType error code.  FT_Err_Ok means success.                     */
  /*                                                                       */
  FT_BASE( FT_Error )
  FT_Raccess_Get_HeaderInfo( FT_Library  library,
                             FT_Stream   stream,
                             FT_Long     rfork_offset,
                             FT_Long    *map_offset,
                             FT_Long    *rdata_pos );


  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    FT_Raccess_Get_DataOffsets                                         */
  /*                                                                       */
  /* <Description>                                                         */
  /*    Get the data offsets for a tag in a resource fork.  Offsets are    */
  /*    stored in an array because, in some cases, resources in a resource */
  /*    fork have the same tag.                                            */
  /*                                                                       */
  /* <Input>                                                               */
  /*    library ::                                                         */
  /*      A FreeType library instance.                                     */
  /*                                                                       */
  /*    stream ::                                                          */
  /*      A file stream containing the resource fork.                      */
  /*                                                                       */
  /*    map_offset ::                                                      */
  /*      The file offset where the resource map starts.                   */
  /*                                                                       */
  /*    rdata_pos ::                                                       */
  /*      The file offset where the resource data starts.                  */
  /*                                                                       */
  /*    tag ::                                                             */
  /*      The resource tag.                                                */
  /*                                                                       */
  /* <Output>                                                              */
  /*    offsets ::                                                         */
  /*      The stream offsets for the resource data specified by `tag'.     */
  /*      This array is allocated by the function, so you have to call     */
  /*      @FT_Free after use.                                              */
  /*                                                                       */
  /*    count ::                                                           */
  /*      The length of offsets array.                                     */
  /*                                                                       */
  /* <Return>                                                              */
  /*    FreeType error code.  FT_Err_Ok means success.                     */
  /*                                                                       */
  /* <Note>                                                                */
  /*    Normally you should use `FT_Raccess_Get_HeaderInfo' to get the     */
  /*    value for `map_offset' and `rdata_pos'.                            */
  /*                                                                       */
  FT_BASE( FT_Error )
  FT_Raccess_Get_DataOffsets( FT_Library  library,
                              FT_Stream   stream,
                              FT_Long     map_offset,
                              FT_Long     rdata_pos,
                              FT_Long     tag,
                              FT_Long   **offsets,
                              FT_Long    *count );


FT_END_HEADER

#endif /* __FTRFORK_H__ */


/* END */
