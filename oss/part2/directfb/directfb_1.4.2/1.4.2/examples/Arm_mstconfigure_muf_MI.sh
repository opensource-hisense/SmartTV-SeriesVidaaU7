#!/bin/sh

PHOTOSPHERE_ROOT="/home/dillan.luo/PERFORCE/DAILEO/SN_Branch/SN__-04.05/Supernova"
CHIP=napoli
PROJ_MODE=europe_dtv
if [ "$1" != "" ]; then
    CHIP=$1
fi



SHARED_LIB_PATH="$PHOTOSPHERE_ROOT/target/$PROJ_MODE.$CHIP/mslib"
DFB_NEEDED_PATH="$PHOTOSPHERE_ROOT/develop/include"
UTOPIA_LIB_PATH="/home/dillan.luo/PERFORCE/THEALE/utopia_release/UTPA2-201.0.x_Napoli/build/bsp"
UTOPIA_INCLUDE_PATH="/home/dillan.luo/PERFORCE/THEALE/utopia_release/UTPA2-201.0.x_Napoli/build/bsp/include"

DESTDIR=`pwd`/../output
DFB_PATH="$DESTDIR/vendor"
PREFIX_PATH="/vendor"
echo "======= DFB needed library ======="
echo $PHOTOSPHERE_ROOT
echo $SHARED_LIB_PATH
echo $DFB_NEEDED_PATH
echo $UTOPIA_LIB_PATH
echo "=================================="

LD_LIB_PATH="-L$SHARED_LIB_PATH -L$UTOPIA_LIB_PATH -lz -lm"
CFLAGS_SETTING=" -DMSOS_PROCESS_SAFT_MUTEX -mlittle-endian  -march=armv7-a -mfpu=vfpv3  -mfloat-abi=softfp -Wall -Wpointer-arith -Wstrict-prototypes -Winline -Wundef -fno-strict-aliasing -fno-optimize-sibling-calls -fno-exceptions -ffunction-sections -fdata-sections -O2 -pthread -I$DFB_NEEDED_PATH/png -I$DFB_NEEDED_PATH/zlib -I$DFB_NEEDED_PATH/jpeg -I$DFB_NEEDED_PATH/freetype/freetype2 -I$DFB_NEEDED_PATH/freetype -I$UTOPIA_INCLUDE_PATH -I$PHOTOSPHERE_ROOT/projects/dfbinfo/inc -DMSOS_TYPE_LINUX"
CPPFLAGS_SETTING=" -DMSOS_PROCESS_SAFT_MUTEX -mlittle-endian  -march=armv7-a -mfpu=vfpv3  -mfloat-abi=softfp -Wall -Wpointer-arith -Wstrict-prototypes -Winline -Wundef -fno-strict-aliasing -fno-optimize-sibling-calls -fno-exceptions -ffunction-sections -fdata-sections -O2 -pthread "
DIRECTFB_CFLAGS_SETTING="-D_REENTRANT -I$DFB_PATH/include -I$UTOPIA_INCLUDE_PATH -I$DFB_PATH/include/directfb -I$DFB_PATH/include/directfb-internal"
DIRECTFB_LIBS_SETTING="-L$DFB_PATH/lib -ldirect -lfusion -ldirectfb -lpthread"

# **********************************************
# Reference Libs
# **********************************************
MSTAR_LIB=" -lrt -lpthread -lutopia_glibc -ldirect"

#HW_DECODEJPEG_LIBS=" -lapiVDEC -ldrvAUDSP -lapiAUDIO -ldrvIPAUTH -lapiJPEG "
HW_DECODEJPEG_LIBS=" -lutopia_glibc "
echo "Running autoconf & automake"
autoreconf --force --install
autoconf
automake

./configure     CC="arm-none-linux-gnueabi-gcc -mlittle-endian" \
                LD="arm-none-linux-gnueabi-ld -EL"\
                CFLAGS="$CFLAGS_SETTING" \
                LDFLAGS="-Wl,-rpath,/mslib -Wl,-rpath,/lib $LD_LIB_PATH" \
                CPPFLAGS="$CPPFLAGS_SETTING" \
                FREETYPE_CFLAGS="-I$DFB_NEEDED_PATH/freetype " \
                LIBS="$MSTAR_LIB $HW_DECODEJPEG_LIBS" \
                DIRECTFB_CFLAGS="$DIRECTFB_CFLAGS_SETTING"              \
                DIRECTFB_LIBS="$DIRECTFB_LIBS_SETTING"                  \
                --prefix=$PREFIX_PATH/directfb_examples                 \
                --host=arm-none-linux-gnueabi \
                --build=i386-linux \
                --enable-debug
./fix_prefix.sh
