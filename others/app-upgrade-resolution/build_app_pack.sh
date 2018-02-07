#!/bin/bash
set -o errexit
set -o nounset

if [ $# -lt 1 ]; then
    echo "Usage: ${0} <version name>"
    exit 1
fi

VERSION=$1
PACK_DIR=app-${VERSION}
PROGRAM_DIR=${PACK_DIR}/app

# mkdir
rm -rf $PACK_DIR
mkdir -p ${PROGRAM_DIR}

# monitor(python program)
echo ""
echo "copying monitor"
cd service/monitor
./install.sh ../../${PROGRAM_DIR}
cd -

# watchdog(C program)
echo ""
echo "Build and copy watchdog:"
cd service/watchdog
make clean
make
make install ../../${PROGRAM_DIR}
cd -

# scripts
echo ""
echo "copying scripts"
cp run_app.sh ${PROGRAM_DIR}

# upgrade script
echo ""
echo "copying upgrade scripts"
cp upgrade.sh ${PACK_DIR}
cp upgrade_app.sh ${PACK_DIR}

# generate version file
echo ${VERSION} > ${PROGRAM_DIR}/version

# tar
echo ""
echo "taring pack files"
tar zcf ${PACK_DIR}.tar.gz $PACK_DIR
rm -r ${PACK_DIR}

md5sum ${PACK_DIR}.tar.gz | awk '{print $1}' > ${PACK_DIR}.tar.gz.md5

echo ""
echo "Build app pack done!"
