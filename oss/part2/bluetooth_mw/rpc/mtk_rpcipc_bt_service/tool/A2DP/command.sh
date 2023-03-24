#!/bin/bash
#
echo "Step 1 : copy headers file to the folder"
cp -f ../../../../rpc/rpc_ipc/inc/u_rpc.h .
cp -f ../../../../inc_rpc/u_rpcipc_types.h .
cp -f ../../../../inc/u_bt_mw_types.h .
cp -f ../../../../inc/u_bt_mw_a2dp.h .
cp -f ../../../../inc/u_bt_mw_common.h .

echo "Step 2 : rpcd Make mtk_bt_service_a2dp_ipcrpc_struct files"
../rpcd/./rpcd -i src_files.h -o mtk_bt_service_a2dp_ipcrpc_struct -s src_header_file.h -p preamble_file.h

echo "Step 3 : make mtk_bt_service_a2dp_ipcrpc_struct files (__rpc_get_desc__ to __rpc_get_a2dp_desc__)"
sed "/^const/c\EXPORT_SYMBOL const RPC_DESC_T* __rpc_get_a2dp_desc__ (UINT32  ui4_idx)" mtk_bt_service_a2dp_ipcrpc_struct.c > a.c
sed "s/__rpc_get_desc__/__rpc_get_a2dp_desc__/g" mtk_bt_service_a2dp_ipcrpc_struct.h > a.h

rm mtk_bt_service_a2dp_ipcrpc_struct.c
rm mtk_bt_service_a2dp_ipcrpc_struct.h
mv a.c mtk_bt_service_a2dp_ipcrpc_struct.c
mv a.h mtk_bt_service_a2dp_ipcrpc_struct.h

echo "Step 4 : copy mtk_bt_service_a2dp_ipcrpc_struct.c to ../../mtk_bt_service_client/src/a2dp"
cp -f mtk_bt_service_a2dp_ipcrpc_struct.c ../../mtk_bt_service_client/src/a2dp

echo "Step 5 : copy mtk_bt_service_a2dp_ipcrpc_struct.h to ../../mtk_bt_service_client/inc"
cp -f mtk_bt_service_a2dp_ipcrpc_struct.h ../../mtk_bt_service_client/inc

rm -f u_rpc.h
rm -f u_rpcipc_types.h
rm -f u_bt_mw_types.h
rm -f u_bt_mw_a2dp.h
rm -r u_bt_mw_common.h
rm -f u_bt_mw_gap.h
rm -f c_mw_config.h
rm -f mtk_bt_service_a2dp_ipcrpc_struct.c
rm -f mtk_bt_service_a2dp_ipcrpc_struct.h
