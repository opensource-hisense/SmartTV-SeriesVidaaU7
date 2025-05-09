/*
   (c) Copyright 2001-2011  The world wide DirectFB Open Source Community (directfb.org)
   (c) Copyright 2000-2004  Convergence (integrated media) GmbH

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>,
              Andreas Hundt <andi@fischlustig.de>,
              Sven Neumann <neo@directfb.org>,
              Ville Syrjälä <syrjala@sci.fi> and
              Claudio Ciccani <klan@users.sf.net>.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <config.h>

#include "CoreLayerRegion.h"

extern "C" {
#include <directfb_util.h>

#include <direct/debug.h>
#include <direct/mem.h>
#include <direct/memcpy.h>
#include <direct/messages.h>

#include <core/core.h>
}

D_DEBUG_DOMAIN( DirectFB_CoreLayerRegion, "DirectFB/CoreLayerRegion", "DirectFB CoreLayerRegion" );

extern "C"
{
extern DFBResult mdfb_wait_for_sync(void);
}

/*********************************************************************************************************************/

namespace DirectFB {



DFBResult
ILayerRegion_Real::FlipUpdate(
                    const DFBRegion                           *update,
                    DFBSurfaceFlipFlags                        flags
)
{
    DFBResult ret;

    D_DEBUG_AT( DirectFB_CoreLayerRegion, "ILayerRegion_Requestor::%s() region[(%d->%d) -> (%d->%d)] flags[0x%x]\n",
        __FUNCTION__, update->x1,update->y1, update->x2, update->y2, flags );

#ifdef CC_HW_WINDOW_SUPPORT
    if(obj->context->cur_region == obj)
    {
        if(flags & DSFLIP_ONSYNC)
        {
            D_DEBUG_AT(DirectFB_CoreLayerRegion, "%s,%d: Before wait for sync\n", __func__, __LINE__);
            mdfb_wait_for_sync();
        }
    }
#endif

     ret = dfb_layer_region_flip_update( obj, update, flags );

#ifdef CC_HW_WINDOW_SUPPORT
     if(obj->context->cur_region == obj)
     {
         if(flags & DSFLIP_WAIT)
         {
             mdfb_wait_for_sync();
         }
     }
#endif

     return ret;
}


DFBResult
ILayerRegion_Real::GetSurface(
                    CoreSurface                              **ret_surface
)
{
     D_DEBUG_AT( DirectFB_CoreLayerRegion, "ILayerRegion_Requestor::%s()\n", __FUNCTION__ );

     return dfb_layer_region_get_surface( obj, ret_surface );
}


}
