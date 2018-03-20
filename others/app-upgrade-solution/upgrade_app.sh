#!/bin/bash

PROGRAM_DIR=app

# check system version
FS_VER1=v1.2.0_170920_alpha
FS_VER2=v1.2.1_170929_alpha
CUR_FS_VER=`fsinfo | head -n 1`
if [ "x${CUR_FS_VER}" != "x${FS_VER1}" -a "x${CUR_FS_VER}" != "x${FS_VER2}" ]; then
    echo "Filesystem version not match! Aborting upgrade!"
    exit 2
fi

# clear old version
rm -rf /home/root/${PROGRAM_DIR}

mv ${PROGRAM_DIR} /home/root/${PROGRAM_DIR}

echo "Updating startup script..."
cp run_app.sh /etc/init.d/
cd /etc/init.d/
update-rc.d -f run_app.sh remove
update-rc.d -f run_app.sh start 99 5 .
cd -

echo "syncing data..."
sync
echo "upgrade application done!"
