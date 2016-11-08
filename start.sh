#!/bin/bash

for (( i=1; i<=$(cat doc/example.txt | wc -l); i++))
do
	echo "Starting node with ID $i..."
	urxvt -e ./bin/node $i &
done
