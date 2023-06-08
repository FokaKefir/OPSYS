#!/bin/bash
#segít tesztelés közben törölni a sort
ipcs -q| tail -n +4| egrep '0x23456789' | cut -f2 -d " " | while read queue
do
    if [ -n "$queue" ];then
        echo $queue törölve
        ipcrm -q $queue
    fi
done

