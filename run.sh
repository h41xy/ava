#!/bin/bash

SHAREDRES="doc/shared.res"
MAIN="bin/node_exec"

LOGFILE="doc/run.log"
ADDRESSFILE="doc/addresses_all.txt"
ADDRESSFILE_THISRUN="doc/addresses.txt"

# Clean the shared file and init it
reset_runlog(){
	echo -n "" > $SHAREDRES
	echo "0" > $SHAREDRES
	echo -n "" > $LOGFILE
}

reset_runlog

if [ -z ${1+x} ]
then
	echo -n "Number of nodes: "
	read NODES
else
	NODES=$1
fi

if [ ! $(($NODES % 2)) -eq 0 ]
then
	echo "Number of nodes must be even."
	exit
fi

# Reduce the addressbook of 5000 Entries to just the number of nodes
LINES=`cat $ADDRESSFILE | wc -l`
cat $ADDRESSFILE | sed $((NODES+1)),${LINES}d > $ADDRESSFILE_THISRUN

for (( i=1; i<=$NODES; i++))
do
	echo "Starting voter node with ID $i..."
	./bin/node_exec $i &
done
