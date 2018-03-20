#!/bin/bash

FILES_DIR='./filesystem'

echo "Upgrading filesystem!"

cp ${FILES_DIR}/sqlite3 /usr/bin/
echo "Update sqlite3 done!"

echo "updating filesystem version"
SOFT_VERSION=`cat ${FILES_DIR}/version | head -n 1`
sed -i '1d' /usr/bin/fsinfo
sed -i "1i\echo \"${SOFT_VERSION}\"" /usr/bin/fsinfo

echo "syncing data..."
sync
echo "Upgrad filesystem done!"

