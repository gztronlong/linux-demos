#!/bin/bash
set -o errexit
set -o nounset

if [ $# -lt 1 ]; then
    echo "Usage: ${0} <version name>"
    exit 1
fi

VERSION=$1
PACK_DIR=kernel-pack-${VERSION}

# mkdir
rm -rf $PACK_DIR
mkdir -p ${PACK_DIR}

# kernel
echo ""
echo "copying kernel"
cp -r kernel ${PACK_DIR}

# upgrade script
echo ""
echo "copying upgrade scripts"
cp upgrade.sh ${PACK_DIR}
cp upgrade_kernel.sh ${PACK_DIR}

# generate version file
echo ${VERSION} > ${PACK_DIR}/kernel/version

# tar
echo ""
echo "taring pack files"
tar zcf ${PACK_DIR}.tar.gz $PACK_DIR
rm -r ${PACK_DIR}

md5sum ${PACK_DIR}.tar.gz | awk '{print $1}' > ${PACK_DIR}.tar.gz.md5

echo ""
echo "Build kernel pack done!"
