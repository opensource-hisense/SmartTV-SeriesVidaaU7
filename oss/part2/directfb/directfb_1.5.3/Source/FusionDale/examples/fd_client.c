/*
   (c) Copyright 2006-2007  directfb.org

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>.

   This file is subject to the terms and conditions of the MIT License:

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without restriction,
   including without limitation the rights to use, copy, modify, merge,
   publish, distribute, sublicense, and/or sell copies of the Software,
   and to permit persons to whom the Software is furnished to do so,
   subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <unistd.h>

#include <direct/interface.h>
#include <direct/messages.h>

#include <voodoo/play.h>
#include <voodoo/server.h>

#include <directfb.h>
#include <fusiondale.h>


/**********************************************************************************************************************/

int
main( int argc, char *argv[] )
{
     DirectResult    ret;
     IFusionDale    *dale;
     IComa          *coma;
     IComaComponent *component;

     ret = DirectFBInit( &argc, &argv );
     if (ret)
          DirectFBErrorFatal( "DirectFBInit", ret );

     ret = FusionDaleInit( &argc, &argv );
     if (ret)
          FusionDaleErrorFatal( "FusionDaleInit", ret );

     ret = FusionDaleCreate( &dale );
     if (ret)
          FusionDaleErrorFatal( "FusionDaleCreate", ret );

     ret = dale->EnterComa( dale, "TestComa", &coma );
     if (ret)
          FusionDaleErrorFatal( "EnterComa", ret );

     ret = coma->GetComponent( coma, "TestComponent", 100000, &component );
     if (ret)
          FusionDaleErrorFatal( "GetComponent", ret );



     ret = component->Call( component, 1, NULL, NULL );
     if (ret)
          FusionDaleError( "Call", ret );


     component->Release( component );
     coma->Release( coma );
     dale->Release( dale );

     return 0;
}

