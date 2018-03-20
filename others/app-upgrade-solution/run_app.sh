#!/bin/bash

PROGRAM_DIR=/home/root/app/

cd /home/root/upgrade

# check if there is upgrade file
ls *.tar.gz 2> /dev/null
if [ $? -eq 0 ]; then
    UPGRADE_PACK=`ls *.tar.gz | head -n 1`
    tar zxf ${UPGRADE_PACK}
    PACK_NAME=`basename ${UPGRADE_PACK} .tar.gz`
    cd ${PACK_NAME}
    ./upgrade.sh
    cd ~
    rm -r /home/root/upgrade/*
    echo "syncing..."
    sync
    reboot
    exit 0
fi

cnt=`ps -aux | grep monitor | grep -v grep | wc -l`
if [ $cnt == 0 ]
then
    cd ${PROGRAM_DIR}

    python ./monitor.py &

    echo "Program started!"
else
    echo "Program already started, skipping!"
fi
