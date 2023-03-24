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

#ifndef __FUSIONDALE_H__
#define __FUSIONDALE_H__

#include <direct/interface.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Version handling.
 */
extern const unsigned int fusiondale_major_version;
extern const unsigned int fusiondale_minor_version;
extern const unsigned int fusiondale_micro_version;
extern const unsigned int fusiondale_binary_age;
extern const unsigned int fusiondale_interface_age;

/*
 * Check for a certain FusionDale version.
 * In case of an error a message is returned describing the mismatch.
 */
const char * FusionDaleCheckVersion( unsigned int required_major,
                                     unsigned int required_minor,
                                     unsigned int required_micro );

/*
 * Main FusionDale interface.
 */
DECLARE_INTERFACE( IFusionDale )

/*
 * Event manager.
 */
DECLARE_INTERFACE( IFusionDaleMessenger )

/*
 * Component manager.
 */
DECLARE_INTERFACE( IComa )

/*
 * Component.
 */
DECLARE_INTERFACE( IComaComponent )

/*
 * Parses the command-line and initializes some variables. You absolutely need to
 * call this before doing anything else. Removes all options used by FusionDale from argv.
 */
DirectResult FusionDaleInit(
                              int    *argc,   /* pointer to main()'s argc */
                              char *(*argv[]) /* pointer to main()'s argv */
                            );

/*
 * Sets configuration parameters supported on command line and in config file.
 * Can only be called before FusionDaleCreate but after FusionDaleInit.
 */
DirectResult FusionDaleSetOption(
                                   const char *name,
                                   const char *value
                                 );

/*
 * Creates the super interface.
 */
DirectResult FusionDaleCreate(
                                IFusionDale **ret_interface  /* pointer to the created interface */
                              );

/*
 * Print a description of the result code along with an
 * optional message that is put in front with a colon.
 */
DirectResult FusionDaleError(
                               const char   *msg,      /* optional message */
                               DirectResult  result    /* result code to interpret */
                             );
                          
/*
 * Behaves like FusionDaleError, but shuts down the calling application.
 */
DirectResult FusionDaleErrorFatal(
                                    const char   *msg,      /* optional message */
                                    DirectResult  result    /* result code to interpret */
                                  );

/*
 * Returns a string describing 'result'.
 */
const char *FusionDaleErrorString(
                                    DirectResult result
                                  );
                                  
/*
 * Retrieves information about supported command-line flags in the
 * form of a user-readable string formatted suitable to be printed
 * as usage information.
 */
const char *FusionDaleUsageString( void );


/*
 * <i><b>IFusionDale</b></i> is the main FusionDale interface.
 */
DEFINE_INTERFACE( IFusionDale,

   /** Events **/

     /*
      * Create a new event manager.
      */
     DirectResult (*CreateMessenger) (
          IFusionDale           *thiz,
          IFusionDaleMessenger **ret_messenger
     );

     /*
      * Get an interface to an existing event manager.
      */
     DirectResult (*GetMessenger) (
          IFusionDale           *thiz,
          IFusionDaleMessenger **ret_messenger
     );


   /** Component Manager **/

     /*
      * Get an interface to a component manager.
      *
      * The <b>name</b> is a unique identifier.
      * The component manager will be created if it doesn't exist.
      */
     DirectResult (*EnterComa) (
          IFusionDale           *thiz,
          const char            *name,
          IComa                **ret_coma
     );
)



typedef unsigned long FDMessengerEventID;
typedef unsigned long FDMessengerListenerID;

#define FDM_EVENT_ID_NONE     ((unsigned long)0)
#define FDM_LISTENER_ID_NONE  ((unsigned long)0)


typedef void (*FDMessengerEventCallback)( FDMessengerEventID  event_id,
                                          int                 param,
                                          void               *data,
                                          int                 data_size,
                                          void               *context );


/*
 * <i><b>IFusionDaleMessenger</b></i> is an event manager.
 */
DEFINE_INTERFACE( IFusionDaleMessenger,

   /** Events **/

     DirectResult (*RegisterEvent) (
          IFusionDaleMessenger     *thiz,
          const char               *name,
          FDMessengerEventID       *ret_id
     );

     DirectResult (*UnregisterEvent) (
          IFusionDaleMessenger     *thiz,
          FDMessengerEventID        event_id
     );

     DirectResult (*IsEventRegistered) (
          IFusionDaleMessenger     *thiz,
          const char               *name
     );


   /** Listeners **/

     DirectResult (*RegisterListener) (
          IFusionDaleMessenger     *thiz,
          FDMessengerEventID        event_id,
          FDMessengerEventCallback  listener,
          void                     *context,
          FDMessengerListenerID    *ret_id
     );

     DirectResult (*UnregisterListener) (
          IFusionDaleMessenger     *thiz,
          FDMessengerListenerID     listener_id
     );


   /** Dispatch **/

     DirectResult (*SendSimpleEvent) (
          IFusionDaleMessenger     *thiz,
          FDMessengerEventID        event_id,
          int                       param
     );

     DirectResult (*SendEvent) (
          IFusionDaleMessenger     *thiz,
          FDMessengerEventID        event_id,
          int                       param,
          void                     *data,
          unsigned int              data_size
     );


   /** Message data **/

     DirectResult (*AllocateData) (
          IFusionDaleMessenger     *thiz,
          unsigned int              data_size,
          void                    **ret_data
     );
)


/*
 * Method ID
 */
typedef unsigned long ComaMethodID;

/*
 * Notification ID
 */
typedef unsigned long ComaNotificationID;


/*
 * 'Method Invocation' Callback
 *
 * Called at the component owner upon invocation of a method using IComaComponent::Call().
 *
 * See also IComa::CreateComponent().
 */
typedef void (*ComaMethodFunc)  (
                                  void               *ctx,
                                  ComaMethodID        method,
                                  void               *arg,
                                  unsigned int        magic
                                 );

/*
 * 'Notification Received' Callback
 *
 * Called at each listener of the notification when IComaComponent::Notify() is used.
 *
 * See also IComaComponent::Listen() and IComaComponent::InitListeners().
 */
typedef void (*ComaListenerFunc)(
                                  void               *ctx,
                                  void               *arg
                                 );

/*
 * 'Notification Dispatched' Callback
 *
 * Called at the component owner when a notification has been processed by all recipients.
 *
 * See also IComaComponent::InitNotification() and IComaComponent::InitNotifications().
 */
typedef void (*ComaNotifyFunc)  (
                                  void               *ctx,
                                  ComaNotificationID  notification,
                                  void               *arg
                                 );

/*
 * Notification flags
 *
 * See also IComaComponent::InitNotification() and IComaComponent::InitNotifications().
 */
typedef enum {
     CNF_NONE            = 0x00000000,  /* None of these */

     CNF_DEALLOC_ARG     = 0x00000001,  /* Deallocate 'arg' after notification is dispatched */

     CNF_ALL             = 0x00000001,  /* All of these */
} ComaNotificationFlags;

/*
 * Notification setup (batch)
 *
 * See also IComaComponent::InitNotifications().
 */
typedef struct {
     ComaNotificationID     id;         /* Notification ID */

     ComaNotifyFunc         func;       /* Optional 'Notification Dispatched' callback */
     void                  *ctx;        /* Optional context pointer for callback */

     ComaNotificationFlags  flags;      /* Notification flags */
} ComaNotificationInit;

/*
 * Listener setup (batch)
 *
 * See also IComaComponent::InitListeners().
 */
typedef struct {
     ComaNotificationID     id;         /* Notification ID */

     ComaListenerFunc       func;       /* 'Notification Received' callback */
     void                  *ctx;        /* Optional context pointer for callback */
} ComaListenerInit;


/*
 * <i><b>IComa</b></i> is a component manager with its own name space created/joined by IFusionDale::EnterComa().
 */
DEFINE_INTERFACE( IComa,

   /** Components **/

     /*
      * Create a new component
      *
      * The component still needs to be activated after notification setup etc. using IComaComponent::Activate().
      *
      * Corresponding calls to IComa::GetComponent() will block until the component has been activated!
      */
     DirectResult (*CreateComponent) (
          IComa                    *thiz,
          const char               *name,
          ComaMethodFunc            func,
          int                       num_notifications,
          void                     *ctx,
          IComaComponent          **ret_component
     );

     /*
      * Request a component
      *
      * This blocks until the component has been created and activated or a <b>timeout</b> occurrs.
      *
      * See also IComa::CreateComponent() and IComaComponent::Activate().
      */
     DirectResult (*GetComponent) (
          IComa                    *thiz,
          const char               *name,
          unsigned int              timeout,
          IComaComponent          **ret_component
     );


   /** Shared memory **/

     /*
      * Allocate anonymous block of shared memory
      *
      * Each allocated block must be deallocated, e.g. in a 'notification dispatched' callback,
      * when it has been used as data for a notification (asynchronous).
      *
      * See also IComa::Deallocate().
      */
     DirectResult (*Allocate) (
          IComa                    *thiz,
          unsigned int              bytes,
          void                    **ret_ptr
     );

     /*
      * Deallocate anonymous block of shared memory
      *
      * See also IComa::Allocate().
      */
     DirectResult (*Deallocate) (
          IComa                    *thiz,
          void                     *ptr
     );


   /** Thread local SHM **/

     /*
      * Get the thread local shared memory block
      *
      * The shared memory block belonging to the calling thread will be allocated or reallocated,
      * if the required amount of <b>bytes</b> is not satisfied, yet.
      *
      * The memory should not be used for asynchronous notifications (queued), but for synchronous method invocations.
      *
      * See also IComa::FreeLocal().
      */
     DirectResult (*GetLocal) (
          IComa                    *thiz,
          unsigned int              bytes,
          void                    **ret_ptr
     );

     /*
      * Free the thread local shared memory
      *
      * This should be called after huge allocations using 
      *
      * Do NOT use this after each call to IComa::GetLocal().
      * It is wise to call when the block is not going to be used at all (or its last size) in the short term.
      *
      * See also IComa::GetLocal().
      */
     DirectResult (*FreeLocal) (
          IComa                    *thiz
     );
)

/*
 * <i><b>IComaComponent</b></i> is a component created by IComa::CreateComponent() or returned by IComa::GetComponent().
 */
DEFINE_INTERFACE( IComaComponent,

   /** Initialization **/

     /*
      * Setup a notification
      *
      * See also IComaComponent::Notify().
      */
     DirectResult (*InitNotification) (
          IComaComponent           *thiz,
          ComaNotificationID        id,
          ComaNotifyFunc            func,
          void                     *ctx,
          ComaNotificationFlags     flags
     );

     /*
      * Batched notification setup
      *
      * See also IComaComponent::Notify().
      */
     DirectResult (*InitNotifications) (
          IComaComponent                *thiz,
          const ComaNotificationInit    *inits,
          int                            num_inits,
          void                          *ctx
     );


   /** Methods **/

     /*
      * Perform method invocation
      *
      * This blocks until the owner has returned from invocation or an error occurred.
      *
      * See also IComaComponent::Return().
      */
     DirectResult (*Call) (
          IComaComponent           *thiz,
          ComaMethodID              method,
          void                     *arg,
          int                      *ret_val
     );

     /*
      * Return from method invocation
      *
      * This can be called outside of the method callback and does not need to follow the call order.
      *
      * See also IComaComponent::Call().
      */
     DirectResult (*Return) (
          IComaComponent           *thiz,
          int                       val,
          unsigned int              magic
     );


   /** Notifications **/

     /*
      * Send a notification to all listeners
      *
      * This returns immediately after posting the asynchronous notification.
      *
      * See also IComaComponent::Listen() and IComaComponent::InitListeners().
      */
     DirectResult (*Notify) (
          IComaComponent           *thiz,
          ComaNotificationID        id,
          void                     *arg
     );

     /*
      * Setup a listener for one notification
      *
      * See also IComaComponent::Notify().
      */
     DirectResult (*Listen) (
          IComaComponent           *thiz,
          ComaNotificationID        id,
          ComaListenerFunc          func,
          void                     *ctx
     );

     /*
      * Batched listener setup
      *
      * See also IComaComponent::Notify().
      */
     DirectResult (*InitListeners) (
          IComaComponent           *thiz,
          const ComaListenerInit   *inits,
          int                       num_inits,
          void                     *ctx
     );

     /*
      * Stop listening
      *
      * See also IComaComponent::Listen() and IComaComponent::InitListeners().
      */
     DirectResult (*Unlisten) (
          IComaComponent           *thiz,
          ComaNotificationID        id
     );


   /** Activation **/

     /*
      * Activate the component
      *
      * This is required after creation and setup, to unblock waiting IComa::GetComponent() calls.
      */
     DirectResult (*Activate) (
          IComaComponent           *thiz
     );
)

#ifdef __cplusplus
}
#endif

#endif

