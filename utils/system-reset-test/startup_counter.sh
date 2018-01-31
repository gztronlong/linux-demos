#!/bin/sh

if [ -e /home/root/startup_counter.txt ]
then
    date '+%F %T' >> /home/root/startup_time.txt
    COUNT0=`head -n 1 /home/root/startup_counter.txt`
    COUNT=`expr ${COUNT0} + 1`
    echo ${COUNT} > /home/root/startup_counter.txt
    echo 'Number of start times' >> /home/root/startup_counter.txt
    echo 'System has started count:'
    echo ${COUNT}
else
    date '+%F %T' >> /home/root/startup_time.txt
    echo '1' > /home/root/startup_counter.txt
    echo 'Number of start times' >> /home/root/startup_counter.txt
    echo 'The first time to start up this system!'
fi

sync

