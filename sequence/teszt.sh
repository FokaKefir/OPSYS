#!/bin/bash

./client 1.txt &
./client 2.txt &
./client 3.txt &

wait
echo shell teszt: vége
