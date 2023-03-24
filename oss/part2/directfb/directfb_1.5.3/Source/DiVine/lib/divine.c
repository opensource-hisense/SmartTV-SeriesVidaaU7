/*
   (c) Copyright 2000-2002  convergence integrated media GmbH.
   (c) Copyright 2002       convergence GmbH.

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>,
              Andreas Hundt <andi@fischlustig.de> and
              Sven Neumann <sven@convergence.de>.

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

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <direct/direct.h>

#include <misc/conf.h>

#include "divine.h"
#include "idivine.h"


/**********************************************************************************************************************/

static DFBResult CreateRemote( const char *host, int session, IDiVine **ret_interface );

/**********************************************************************************************************************/

DFBResult
DiVineInit( int *argc, char *(*argv[]) )
{
     DFBResult ret;

#ifdef DSLINUX
     IDiVine_Requestor_ctor();
#endif

     ret = dfb_config_init( argc, argv );
     if (ret)
          return ret;

     return DFB_OK;
}

/*
 * Programs have to call this to get the super interface
 * which is needed to access other functions
 */
DFBResult
DiVineCreate( IDiVine **interface )
{
     DFBResult  ret;
     IDiVine   *divine;

     if (!dfb_config) {
          /*  don't use D_ERROR() here, it uses dfb_config  */
          direct_log_printf( NULL, "(!) DiVineCreate: DiVineInit has to be called before DiVineCreate!\n" );
          return DFB_INIT;
     }

     if (!interface)
          return DFB_INVARG;

     direct_initialize();

#ifndef DIRECTFB_PURE_VOODOO
     if (dfb_config->remote.host)
          return CreateRemote( dfb_config->remote.host, dfb_config->remote.port, interface );

     DIRECT_ALLOCATE_INTERFACE( divine, IDiVine );

     ret = IDiVine_Construct( divine );
     if (ret)
          return ret;

     *interface = divine;

     return DFB_OK;
#else
     return CreateRemote( dfb_config->remote.host ?: "", dfb_config->remote.session, interface );
#endif
}

/**********************************************************************************************************************/

static DFBResult
CreateRemote( const char *host, int session, IDiVine **ret_interface )
{
     DFBResult             ret;
     DirectInterfaceFuncs *funcs;
     void                 *interface;

     D_ASSERT( host != NULL );
     D_ASSERT( ret_interface != NULL );

     ret = DirectGetInterface( &funcs, "IDiVine", "Requestor", NULL, NULL );
     if (ret)
          return ret;

     ret = funcs->Allocate( &interface );
     if (ret)
          return ret;

     ret = funcs->Construct( interface, host, session );
     if (ret)
          return ret;

     *ret_interface = interface;

     return DFB_OK;
}

/**********************************************************************************************************************/

struct _DiVine {
     int fd; /* The file descriptor of the connection (pipe) */
};

DiVine *
divine_open( const char *path )
{
     int     fd;
     DiVine *divine;

     /* Open the pipe specified by 'path' */
     fd = open( path, O_WRONLY );
     if (fd < 0) {
          perror( path );
          return NULL;
     }

     /* Allocate connection object */
     divine = calloc( 1, sizeof(DiVine) );
     if (!divine) {
          fprintf( stderr, "Out of memory!!!\n" );
          return NULL;
     }

     /* Fill out connection information */
     divine->fd = fd;

     /* Return connection object */
     return divine;
}

void
divine_send_event( DiVine *divine, const DFBInputEvent *event )
{
     /* Write event to pipe */
     write( divine->fd, event, sizeof(DFBInputEvent) );
}

void
divine_send_symbol( DiVine *divine, DFBInputDeviceKeySymbol symbol )
{
     DFBInputEvent event;

     /* Construct 'press' event */
     event.flags      = DIEF_KEYSYMBOL;
     event.type       = DIET_KEYPRESS;
     event.key_symbol = symbol;

     /* Write 'press' event to pipe */
     write( divine->fd, &event, sizeof(DFBInputEvent) );

     /* Turn into 'release' event */
     event.type = DIET_KEYRELEASE;

     /* Write 'release' event to pipe */
     write( divine->fd, &event, sizeof(DFBInputEvent) );
}

void
divine_send_identifier( DiVine *divine, DFBInputDeviceKeyIdentifier identifier )
{
     DFBInputEvent event;

     /* Construct 'press' event */
     event.flags  = DIEF_KEYID;
     event.type   = DIET_KEYPRESS;
     event.key_id = identifier;

     /* Write 'press' event to pipe */
     write( divine->fd, &event, sizeof(DFBInputEvent) );

     /* Turn into 'release' event */
     event.type = DIET_KEYRELEASE;

     /* Write 'release' event to pipe */
     write( divine->fd, &event, sizeof(DFBInputEvent) );
}

/* crude hack for minicom vt102 escape sequences */
void
divine_send_vt102( DiVine *divine, int size, const char *ansistr )
{
     int i;

     for (i = 0; i < size; i++) {
          DFBInputEvent event;
          int f = 0;

          /* Construct 'press' event */
          event.flags = DIEF_KEYSYMBOL;
          event.type  = DIET_KEYPRESS;
          f = 0;

          /* watch for escape sequences */
          switch (ansistr[i]) {
               case 27:
                    if (ansistr[i+1] == '[' || ansistr[i+1] == 'O') {
                         switch (ansistr[i+2]) {
                              case 'F':  event.key_symbol = DIKS_END;  f = 1; break;
                              case 'H':  event.key_symbol = DIKS_HOME; f = 1; break;
                              case 'P':  event.key_symbol = DIKS_F1;   f = 1; break;
                              case 'Q':  event.key_symbol = DIKS_F2;   f = 1; break;
                              case 'R':  event.key_symbol = DIKS_F3;   f = 1; break;
                              case 'S':  event.key_symbol = DIKS_F4;   f = 1; break;
                              case '1':  switch (ansistr[i+3]) {
                                        case '6':  event.key_symbol = DIKS_F5;   break;
                                        case '7':  event.key_symbol = DIKS_F6;   break;
                                        case '8':  event.key_symbol = DIKS_F7;   break;
                                        case '9':  event.key_symbol = DIKS_F8;   break;
					case '~':  event.key_symbol = DIKS_HOME; break;
                                        default:   break;
                                   }
                                   f = 3;  break;
                              case '2': switch (ansistr[i+3]) {
                                        case '0':  event.key_symbol = DIKS_F9;     break;
                                        case '1':  event.key_symbol = DIKS_F10;    break;
                                        case '3':  event.key_symbol = DIKS_F11;    break;
                                        case '4':  event.key_symbol = DIKS_F12;    break;
                                        case '~':  event.key_symbol = DIKS_INSERT; break;
                                        default: break;
                                   }
                                   f =  3; break;
                              case '3': event.key_symbol = DIKS_DELETE;       f = 3; break;
                              case '5': event.key_symbol = DIKS_PAGE_UP;      f = 3; break;
                              case '6': event.key_symbol = DIKS_PAGE_DOWN;    f = 3; break;
                              case '7': event.key_symbol = DIKS_STOP;         f = 3; break;
                              case 'A': event.key_symbol = DIKS_CURSOR_UP;    f = 2; break;
                              case 'B': event.key_symbol = DIKS_CURSOR_DOWN;  f = 2; break;
                              case 'C': event.key_symbol = DIKS_CURSOR_RIGHT; f = 2; break;
                              case 'D': event.key_symbol = DIKS_CURSOR_LEFT;  f = 2; break;
                              default:  break;
                         }
                         break;
                    }
                    else  event.key_symbol = DIKS_ESCAPE;   break;

               case 127: event.key_symbol = DIKS_BACKSPACE; break;
               case 10:  event.key_symbol = DIKS_ENTER;     break;

                 /*  emulate numbers as coming from keypad  */
               case '0':
               case '1':
               case '2':
               case '3':
               case '4':
               case '5':
               case '6':
               case '7':
               case '8':
               case '9':
                 event.key_id = DIKI_KP_0 + ansistr[i] - '0';
                 event.flags |= DIEF_KEYID;
                 /*  fallthru  */

               default:  event.key_symbol = ansistr[i];     break;
          }
          i= i + f;

          write( divine->fd, &event, sizeof( DFBInputEvent) );

          event.type = DIET_KEYRELEASE;

          write( divine->fd, &event, sizeof(DFBInputEvent) );
     }
}

void
divine_send_motion_absolute( DiVine *divine, int x, int y )
{
     DFBInputEvent event;

     /* Construct 'motion' event */
     event.flags   = DIEF_AXISABS;
     event.type    = DIET_AXISMOTION;
     event.axis    = DIAI_X;
     event.axisabs = x;

     /* Write 'motion' event to pipe */
     write( divine->fd, &event, sizeof(DFBInputEvent) );

     /* Construct 'motion' event */
     event.flags   = DIEF_AXISABS;
     event.type    = DIET_AXISMOTION;
     event.axis    = DIAI_Y;
     event.axisabs = y;

     /* Write 'motion' event to pipe */
     write( divine->fd, &event, sizeof(DFBInputEvent) );
}

void
divine_send_button_press( DiVine *divine, DFBInputDeviceButtonIdentifier button )
{
     DFBInputEvent event;

     /* Construct 'press' event */
     event.flags   = DIEF_NONE;
     event.type    = DIET_BUTTONPRESS;
     event.button  = button;

     /* Write 'press' event to pipe */
     write( divine->fd, &event, sizeof(DFBInputEvent) );
}

void
divine_send_button_release( DiVine *divine, DFBInputDeviceButtonIdentifier button )
{
     DFBInputEvent event;

     /* Construct 'release' event */
     event.flags   = DIEF_NONE;
     event.type    = DIET_BUTTONRELEASE;
     event.button  = button;

     /* Write 'release' event to pipe */
     write( divine->fd, &event, sizeof(DFBInputEvent) );
}

void
divine_close( DiVine *divine )
{
     /* Close the pipe */
     close( divine->fd );

     /* Free connection object */
     free( divine );
}
