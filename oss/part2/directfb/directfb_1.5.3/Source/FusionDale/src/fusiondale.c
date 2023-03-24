/*
   (c) Copyright 2006-2007  directfb.org

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>.

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

#include <stdio.h>
#include <stdlib.h>

#include <fusiondale.h>
#include <fusiondale_version.h>

#include <direct/debug.h>
#include <direct/interface.h>
#include <direct/log.h>
#include <direct/messages.h>
#include <direct/util.h>

#include <ifusiondale.h>

#include <misc/dale_config.h>


IFusionDale *ifusiondale_singleton = NULL;

static pthread_mutex_t  fusion_dale_lock = PTHREAD_MUTEX_INITIALIZER;


/**************************************************************************************************/

static DirectResult CreateRemote( const char *host, int session, IFusionDale **ret_interface );

/**********************************************************************************************************************/

/*
 * Version checking
 */
const unsigned int fusiondale_major_version = FUSIONDALE_MAJOR_VERSION;
const unsigned int fusiondale_minor_version = FUSIONDALE_MINOR_VERSION;
const unsigned int fusiondale_micro_version = FUSIONDALE_MICRO_VERSION;
const unsigned int fusiondale_binary_age    = FUSIONDALE_BINARY_AGE;
const unsigned int fusiondale_interface_age = FUSIONDALE_INTERFACE_AGE;

const char *
FusionDaleCheckVersion( unsigned int required_major,
                         unsigned int required_minor,
                         unsigned int required_micro )
{
     if (required_major > FUSIONDALE_MAJOR_VERSION)
          return "FusionDale version too old (major mismatch)";
     if (required_major < FUSIONDALE_MAJOR_VERSION)
          return "FusionDale version too new (major mismatch)";
     if (required_minor > FUSIONDALE_MINOR_VERSION)
          return "FusionDale version too old (minor mismatch)";
     if (required_minor < FUSIONDALE_MINOR_VERSION)
          return "FusionDale version too new (minor mismatch)";
     if (required_micro < FUSIONDALE_MICRO_VERSION - FUSIONDALE_BINARY_AGE)
          return "FusionDale version too new (micro mismatch)";
     if (required_micro > FUSIONDALE_MICRO_VERSION)
          return "FusionDale version too old (micro mismatch)";

     return NULL;
}

const char *
FusionDaleUsageString( void )
{
     return fd_config_usage();
}

DirectResult
FusionDaleInit( int *argc, char **argv[] )
{
#ifdef DSLINUX
     IFusionDale_Requestor_ctor();
#endif

     return fd_config_init( argc, argv );
}

DirectResult
FusionDaleSetOption( const char *name, const char *value )
{
     if (fusiondale_config == NULL) {
          D_ERROR( "FusionDaleSetOption: FusionDaleInit has to be called first!\n" );
          return DR_INIT;
     }

     if (ifusiondale_singleton) {
          D_ERROR( "FusionDaleSetOption: FusionDaleCreate has already been called!\n" );
          return DR_INIT;
     }

     if (!name)
          return DR_INVARG;

     return fd_config_set( name, value );
}

DirectResult
FusionDaleCreate( IFusionDale **ret_interface )
{
     DirectResult ret;
     
     if (!fusiondale_config) {
          D_ERROR( "FusionDaleCreate: FusionDaleInit has to be called first!\n" );
          return DR_INIT;
     }

     if (!ret_interface)
          return DR_INVARG;
     
     pthread_mutex_lock( &fusion_dale_lock );
    
     if (ifusiondale_singleton) {
          ifusiondale_singleton->AddRef( ifusiondale_singleton );
          *ret_interface = ifusiondale_singleton;
          pthread_mutex_unlock( &fusion_dale_lock );
          return DR_OK;
     }

#ifndef DIRECTFB_PURE_VOODOO
     if (fusiondale_config->remote.host)
     {
          ret= CreateRemote( fusiondale_config->remote.host, fusiondale_config->remote.session, ret_interface );
          pthread_mutex_unlock( &fusion_dale_lock );
          return ret;
     }

     if (!(direct_config->quiet & DMT_BANNER) && fusiondale_config->banner) {
          direct_log_printf( NULL,
               "\n"
               "     *--------------) FusionDale v%d.%d.%d (--------------*\n"
               "                (c) 2006-2007  directfb.org\n"
               "       -----------------------------------------------\n"
               "\n",
               FUSIONDALE_MAJOR_VERSION, FUSIONDALE_MINOR_VERSION, FUSIONDALE_MICRO_VERSION );
     }

     DIRECT_ALLOCATE_INTERFACE( ifusiondale_singleton, IFusionDale );
     
     ret = IFusionDale_Construct( ifusiondale_singleton );
     if (ret != DR_OK)
          ifusiondale_singleton = NULL;
          
     *ret_interface = ifusiondale_singleton;
     pthread_mutex_unlock( &fusion_dale_lock );
     return DR_OK;
#else
    ret= CreateRemote( fusiondale_config->remote.host ?: "", fusiondale_config->remote.session, ret_interface );
    pthread_mutex_unlock( &fusion_dale_lock );
    return ret;
#endif
}

DirectResult
FusionDaleError( const char *msg, DirectResult error )
{
     if (msg)
          fprintf( stderr, "(#) FusionDale Error [%s]: %s\n", msg, DirectResultString( error ) );
     else
          fprintf( stderr, "(#) FusionDale Error: %s\n", DirectResultString( error ) );

     return error;
}

DirectResult
FusionDaleErrorFatal( const char *msg, DirectResult error )
{
     FusionDaleError( msg, error );
     
     exit( error );
}

const char *
FusionDaleErrorString( DirectResult error )
{
     return DirectResultString( error );
}

/**********************************************************************************************************************/

static DirectResult
CreateRemote( const char *host, int session, IFusionDale **ret_interface )
{
     DirectResult          ret;
     DirectInterfaceFuncs *funcs;
     void                 *interface;

     D_ASSERT( host != NULL );
     D_ASSERT( ret_interface != NULL );

     ret = DirectGetInterface( &funcs, "IFusionDale", "Requestor", NULL, NULL );
     if (ret)
          return ret;

     ret = funcs->Allocate( &interface );
     if (ret)
          return ret;

     ret = funcs->Construct( interface, host, session );
     if (ret)
          return ret;

     *ret_interface = interface;

     return DR_OK;
}

