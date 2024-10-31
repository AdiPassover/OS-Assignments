#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 [-l] for leader follower or [-p] for pipeline"
    exit 1
fi

make server > /dev/null

./server $1 > /dev/null &
./client.sh inputs/client1.txt > /dev/null &
./client.sh inputs/client2.txt > /dev/null &
./client.sh inputs/client3.txt > /dev/null &
sleep 1
killall -2 server
sleep 0.01

echo "Finished running"