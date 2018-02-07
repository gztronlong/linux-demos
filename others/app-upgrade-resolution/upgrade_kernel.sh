#!/bin/bash

KERNEL_DIR='./kernel'

echo "Upgrading kernel!"

cp ${KERNEL_DIR}/am57xx-evm-reva3.dtb /boot
cp ${KERNEL_DIR}/zImage /boot
tar zxf ${KERNEL_DIR}/modules/* -C /lib/modules

echo "syncing data..."
sync
echo "Upgrad kernel done!"

