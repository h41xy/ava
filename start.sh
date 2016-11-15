#!/bin/bash

GRAPHGEN=bin/graphgen
NODE=bin/node

ADDRESSFILE=doc/addresses.txt
GRAPHFILE=doc/graph.txt
GRAPHPNG=doc/graph.png

EXEC=n

echo -n "Please enter the number of nodes: "
read NODES
echo -n "And the number of edges: "
read EDGES
echo -n "When should a rumor be believable?"
read RUMOR

$GRAPHGEN $EDGES $NODES
dot -Tpng $GRAPHFILE > $GRAPHPNG
feh -. $GRAPHPNG &

echo -n "Execute? "
read EXEC

if [[ $EXEC =~ ^[Yy]$ ]]
then
	for (( i=1; i<=$NODES; i++))
	do
		echo "Starting node with ID $i..."
		urxvt -e ./bin/node $i $RUMOR &
	done
fi
