#!/bin/bash

SLEEPTIME=$5

echo "Starting $1 testrun(s) with $2 Nodes, $3 Edges and $4 Believing border."
for ((i=1;i<=$1;i++))
do
	sleep $SLEEPTIME
	./start.sh $2 $3 $4 y
	sleep $SLEEPTIME
	echo "Starting the watcher and spreading the rumor."
	./bin/controller 25001 4 0 &
	sleep $SLEEPTIME
	echo "Ending the testrun."
	./bin/controller 25001 1 1
	./bin/controller 25000 1 1
	echo "Testrun ended"
done
