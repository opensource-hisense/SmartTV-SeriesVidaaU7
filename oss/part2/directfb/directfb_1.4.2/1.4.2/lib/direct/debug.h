/*
   (c) Copyright 2001-2008  The world wide DirectFB Open Source Community (directfb.org)
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

#ifndef __DIRECT__DEBUG_H__
#define __DIRECT__DEBUG_H__

#include <direct/build.h>

#include <direct/clock.h>
#include <direct/conf.h>
#include <direct/log.h>
#include <direct/messages.h>
#include <direct/system.h>
#include <direct/trace.h>
#include <direct/types.h>



#define D_DEBUG_DOMAIN( _identifier, _name, _description )                                          \
     D_LOG_DOMAIN( _identifier, _name, _description )

#ifndef DIRECT_DISABLE_DEPRECATED



D_DEBUG_DOMAIN( _direct_debug_deprecated, "-", "deprecated" );


#define D_DEBUG( ... )                                                                              \
     D_DEBUG_AT( _direct_debug_deprecated, __VA_ARGS__ )

#endif

void direct_debug_config_domain( const char *name, bool enable );

typedef enum {
    DFB_DBG_LEVEL_DISABLE=0,
    DFB_DBG_LEVEL_NORMAL,
    DFB_DFB_LEVEL_TEMP_FILE,
}DFB_Debug_Level;
#if DIRECT_BUILD_TEXT

void DIRECT_API direct_debug_at_always( DirectLogDomain *domain,
                                        const char      *format, ... )  D_FORMAT_PRINTF(2);

#define d_debug_at( domain, ... )      direct_debug_at_always( &domain, __VA_ARGS__ )


#if DIRECT_BUILD_DEBUGS

/*
 * Direct v2.0 - Debug Interface
 *
 * debug level 1-9  (0 = verbose)
 *
 */
void DIRECT_API direct_debug_log( DirectLogDomain *domain,
                                  unsigned int     debug_level,
                                  const char      *format, ... )      D_FORMAT_PRINTF(3);



/* old */
void DIRECT_API direct_debug_at( DirectLogDomain *domain,
                                 const char      *format, ... ) D_FORMAT_PRINTF(2);

void DIRECT_API direct_debug_enter( DirectLogDomain *domain,
                                    const char *func,
                                    const char *file,
                                    int         line,
                                    const char *format, ... )  D_FORMAT_PRINTF(5);

void DIRECT_API direct_debug_exit( DirectLogDomain *domain,
                                   const char *func,
                                   const char *file,
                                   int         line,
                                   const char *format, ... )  D_FORMAT_PRINTF(5);

void DIRECT_API direct_break( const char *func,
                              const char *file,
                              int         line,
                              const char *format, ... )  D_FORMAT_PRINTF(4);

void DIRECT_API direct_assertion( const char *exp,
                                  const char *func,
                                  const char *file,
                                  int         line );

void DIRECT_API direct_assumption( const char *exp,
                                   const char *func,
                                   const char *file,
                                   int         line );

void DIRECT_API direct_debug_redirect_to_tmpfile(const char  *format, ... );

#endif

#if DIRECT_BUILD_DEBUG || defined(DIRECT_ENABLE_DEBUG) || defined(DIRECT_FORCE_DEBUG)

#define D_DEBUG_ENABLED  (1)

#define D_DEBUG_LOG(_Domain,_level,...)                                              \
     do {                                                                            \
          direct_debug_log( &_Domain, _level, __VA_ARGS__ );                         \
     } while (0)


#define D_DEBUG_AT(d,...)                                                            \
     do {                                                                            \
          direct_debug_at( &d, __VA_ARGS__ );                                        \
     } while (0)

#define D_DEBUG_ENTER(d,...)                                                         \
     do {                                                                            \
          /*direct_debug_enter( &d, __FUNCTION__, __FILE__, __LINE__, x );*/             \
     } while (0)

#define D_DEBUG_EXIT(d,...)                                                          \
     do {                                                                            \
          /*direct_debug_exit( &d, __FUNCTION__, __FILE__, __LINE__, x );*/              \
     } while (0)

#define D_ASSERT(exp)                                                                \
     do {                                                                            \
          if (!(exp))                                                                \
               direct_assertion( #exp, __FUNCTION__, __FILE__, __LINE__ );           \
     } while (0)


#define D_ASSUME(exp)                                                                \
     do {                                                                            \
          if (!(exp))                                                                \
               direct_assumption( #exp, __FUNCTION__, __FILE__, __LINE__ );          \
     } while (0)


#define D_BREAK(...)                                                                 \
     do {                                                                            \
          direct_break( __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__ );                       \
     } while (0)

#define D_DEBUG_CHECK(d)                                                             \
     direct_log_domain_check( &d )

#elif defined(DIRECT_MINI_DEBUG)

/*
 * Mini debug mode, only D_DEBUG_AT, no domain filters, simple assertion
 */

#define D_DEBUG_ENABLED  (2)

#define D_DEBUG_LOG(_Domain,_level,...)                                              \
     do {                                                                            \
          if (direct_config->log_level >= DIRECT_LOG_DEBUG)                          \
               direct_debug_at_always( &d, __VA_ARGS__ );                            \
     } while (0)

#define D_DEBUG_AT(d,...)                                                            \
     do {                                                                            \
          if (direct_config->log_level >= DIRECT_LOG_DEBUG)                          \
               direct_debug_at_always( &d, __VA_ARGS__ );                            \
     } while (0)

#define D_CHECK(exp, aa)                                                             \
     do {                                                                            \
          if (!(exp)) {                                                              \
               long long   millis = direct_clock_get_millis();                       \
               const char *name   = direct_thread_self_name();                       \
                                                                                     \
               direct_log_printf( NULL,                                              \
                                  "(!) [%-15s %3lld.%03lld] (%5d) *** " #aa " [%s] failed *** [%s:%d in %s()]\n",  \
                                  name ? name : "  NO NAME  ", millis / 1000LL, millis % 1000LL,                   \
                                  direct_gettid(), #exp, __FILE__, __LINE__, __FUNCTION__ );                       \
                                                                                     \
               direct_trace_print_stack( NULL );                                     \
          }                                                                          \
     } while (0)

#define D_ASSERT(exp)    D_CHECK(exp, Assertion)
#define D_ASSUME(exp)    D_CHECK(exp, Assumption)

#define D_DEBUG_CHECK(d)                                                             \
     direct_config->debug

#endif    /* MINI_DEBUG  / DIRECT_BUILD_DEBUG || DIRECT_ENABLE_DEBUG || DIRECT_FORCE_DEBUG */


#define D_DEBUG_AT__(d,...)                                                          \
     do {                                                                            \
          direct_log_printf( NULL, __VA_ARGS__ );                                    \
     } while (0)

#endif    /* DIRECT_BUILD_TEXT */


/*
 * Fallback definitions, e.g. without DIRECT_BUILD_TEXT or DIRECT_ENABLE_DEBUG
 */

#ifndef D_DEBUG_ENABLED
#define D_DEBUG_ENABLED  (0)
#endif

#ifndef D_DEBUG_LOG
#define D_DEBUG_LOG(_Domain,_level,...)                                              \
     do {                                                                            \
     } while (0)
#endif

#ifndef D_DEBUG
#define D_DEBUG(d,...)             do {} while (0)
#endif

#ifndef D_DEBUG_AT
#define D_DEBUG_AT(d,...)          do {} while (0)
#endif

#ifndef D_DEBUG_ENTER
#define D_DEBUG_ENTER(d,...)       do {} while (0)
#endif

#ifndef D_DEBUG_EXIT
#define D_DEBUG_EXIT(d,...)        do {} while (0)
#endif

#ifndef D_ASSERT
#define D_ASSERT(exp)              do {} while (0)
#endif

#ifndef D_ASSUME
#define D_ASSUME(exp)              do {} while (0)
#endif

#ifndef D_DEBUG_AT__
#define D_DEBUG_AT__(d,...)        do {} while (0)
#endif

#ifndef D_DEBUG_CHECK
#define D_DEBUG_CHECK(d)           false
#endif

#ifndef D_BREAK
#define D_BREAK(...)               do {} while (0)
#endif

#ifndef d_debug_at
#define d_debug_at( domain, ... )  do {} while (0)
#endif

#ifndef D_LOG_DOMAIN
#define D_LOG_DOMAIN(i,n,d)
#endif



/*
 * Magic Assertions & Utilities
 */

#define D_MAGIC(spell)             ( (((spell)[sizeof(spell)*8/9] << 24) | \
                                      ((spell)[sizeof(spell)*7/9] << 16) | \
                                      ((spell)[sizeof(spell)*6/9] <<  8) | \
                                      ((spell)[sizeof(spell)*5/9]      )) ^  \
                                     (((spell)[sizeof(spell)*4/9] << 24) | \
                                      ((spell)[sizeof(spell)*3/9] << 16) | \
                                      ((spell)[sizeof(spell)*2/9] <<  8) | \
                                      ((spell)[sizeof(spell)*1/9]      )) )


#if DIRECT_BUILD_DEBUGS

#define D_MAGIC_CHECK(o,m)         ((o) != NULL && (o)->magic == D_MAGIC(#m))

#define D_MAGIC_SET(o,m)           do {                                              \
                                        D_ASSERT( (o) != NULL );                     \
                                        D_ASSUME( (o)->magic != D_MAGIC(#m) );       \
                                                                                     \
                                        (o)->magic = D_MAGIC(#m);                    \
                                   } while (0)

#define D_MAGIC_SET_ONLY(o,m)      do {                                              \
                                        D_ASSERT( (o) != NULL );                     \
                                                                                     \
                                        (o)->magic = D_MAGIC(#m);                    \
                                   } while (0)

#define D_MAGIC_ASSERT(o,m)        do {                                              \
                                        D_ASSERT( (o) != NULL );                     \
                                        D_ASSERT( (o)->magic == D_MAGIC(#m) );       \
                                   } while (0)

#define D_MAGIC_ASSUME(o,m)        do {                                              \
                                        D_ASSUME( (o) != NULL );                     \
                                        if (o)                                       \
                                             D_ASSUME( (o)->magic == D_MAGIC(#m) );  \
                                   } while (0)

#define D_MAGIC_ASSERT_IF(o,m)     do {                                              \
                                        if (o)                                       \
                                             D_ASSERT( (o)->magic == D_MAGIC(#m) );  \
                                   } while (0)

#define D_MAGIC_CLEAR(o)           do {                                              \
                                        D_ASSERT( (o) != NULL );                     \
                                        D_ASSUME( (o)->magic != 0 );                 \
                                                                                     \
                                        (o)->magic = 0;                              \
                                   } while (0)

#define D_INDEX_ASSERT(index,array)                         \
     do {                                                   \
          D_ASSERT( index >= 0 );                           \
          D_ASSERT( index < D_ARRAY_SIZE(array) );          \
     } while (0)

#else


#define D_MAGIC_CHECK(o,m)         ((o) != NULL)

#define D_MAGIC_SET(o,m)           do {                                              \
                                   } while (0)

#define D_MAGIC_SET_ONLY(o,m)      do {                                              \
                                   } while (0)

#define D_MAGIC_ASSERT(o,m)        do {                                              \
                                   } while (0)

#define D_MAGIC_ASSUME(o,m)        do {                                              \
                                   } while (0)

#define D_MAGIC_ASSERT_IF(o,m)     do {                                              \
                                   } while (0)

#define D_MAGIC_CLEAR(o)           do {                                              \
                                   } while (0)

#define D_INDEX_ASSERT(index,array)                         \
     do {                                                   \
     } while (0)

#endif



#define D_FLAGS_ASSERT(flags,f)    D_ASSERT( D_FLAGS_ARE_IN(flags,f) )


#define DBG_GLES2_MSG(x...)                                                  \
     do {                                                                    \
          if ((dfb_config->mst_debug_gles2)||(direct_config->debug))                                 \
               direct_messages_info( __FUNCTION__, __LINE__, getpid(), x);                                                  \
     } while (0)

#define DBG_LAYER_MSG(x...)                                                  \
     do {                                                                    \
          if ((dfb_config->mst_debug_layer)||(direct_config->debug))                                 \
               direct_messages_info( __FUNCTION__, __LINE__, getpid(), x);                                                  \
     } while (0)

#define DBG_SECURE_MSG(x...)                                                  \
     do {                                                                    \
          if ((dfb_config->mst_debug_secure_mode)||(direct_config->debug))                                 \
               direct_messages_info( __FUNCTION__, __LINE__, getpid(), x);                                                  \
     } while (0)

#define DBG_SUR_MSG(x...)                                                  \
    do {                                                                     \
        if ((dfb_config->mst_debug_surface)||(direct_config->debug))                                   \
            direct_messages_info( __FUNCTION__, __LINE__, getpid(), x);                                                    \
    } while (0)

#define DBG_INPUT_MSG(x...)                                                  \
    do {                                                                    \
        if ((dfb_config->mst_debug_input)||(direct_config->debug)){ \
            if(dfb_config->mst_debug_input == DFB_DFB_LEVEL_TEMP_FILE)\
               direct_debug_redirect_to_tmpfile(x);\
            else\
               direct_messages_info( __FUNCTION__, __LINE__, getpid(), x);\
        }\
    } while (0)

#define DBG_CURSOR_MSG(x...)                                                 \
     do {                                                                    \
          if ((dfb_config->mst_debug_cursor)||(direct_config->debug))        \
               printf( x );                                                  \
     } while (0)


#define D_DEBUG_ION(d,x...)                                                           \
    do {                                                                            \
        if((dfb_config->mst_debug_ion)||(direct_config->debug)) direct_debug_at_always( &d, x );                 \
    } while (0)

#define D_DEBUG_CMA(d,x...)                               \
    do {                                                                            \
        if((dfb_config->mst_debug_cma)||(direct_config->debug)) direct_debug_at_always( &d, x );                 \
    } while (0)

#define D_DEBUG_MMA(d,x...)                               \
    do {                                                                            \
        if((dfb_config->mst_debug_mma)||(direct_config->debug)) direct_debug_at_always( &d, x );                 \
    } while (0)
#endif

