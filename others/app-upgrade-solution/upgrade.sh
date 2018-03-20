#!/bin/bash

if [ -e ./upgrade_app.sh ]; then
    ./upgrade_app.sh
fi

if [ -e ./upgrade_kernel.sh ]; then
    ./upgrade_kernel.sh
fi

if [ -e ./upgrade_filesystem.sh ]; then
    ./upgrade_filesystem.sh
fi

echo "upgrade done!"
