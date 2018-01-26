#!/bin/sh

if [ -e /home/root/startup_counter.txt ]
then
    COUNT0=`head -n 1 /home/root/startup_counter.txt`
    COUNT=`expr ${COUNT0} + 1`
    echo ${COUNT} > /home/root/startup_counter.txt
    echo 'Number of start times' >> /home/root/startup_counter.txt
    echo 'System has started count:'
    echo ${COUNT}
else
    echo '1' > /home/root/startup_counter.txt
    echo 'Number of start times' >> /home/root/startup_counter.txt
    echo 'The first time to start up this system!'
fi

sync

