#!/bin/bash
set -o errexit
set -o nounset

if [ $# -lt 1 ]; then
    echo "Usage: ${0} <version name>"
    exit 1
fi

VERSION=$1
PACK_DIR=fs-pack-${VERSION}
FILES_DIR=${PACK_DIR}/filesystem

# mkdir
rm -rf $PACK_DIR
mkdir -p ${FILES_DIR}

# sqlite3
echo ""
echo "copying sqlite3"
cp ../tools/sqlite3 ${FILES_DIR}

# upgrade script
echo ""
echo "copying upgrade scripts"
cp upgrade.sh ${PACK_DIR}
cp upgrade_filesystem.sh ${PACK_DIR}

# generate version file
echo ${VERSION} > ${FILES_DIR}/version

# tar
echo ""
echo "taring pack files"
tar zcf ${PACK_DIR}.tar.gz $PACK_DIR
rm -r ${PACK_DIR}

md5sum ${PACK_DIR}.tar.gz | awk '{print $1}' > ${PACK_DIR}.tar.gz.md5

echo ""
echo "Build filesystem pack done!"
