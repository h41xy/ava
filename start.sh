#!/bin/bash

GRAPHGEN=bin/graphgen
NODE=bin/node

ADDRESSFILE=doc/example.txt
GRAPHFILE=doc/example_graph.txt
GRAPHPNG=doc/example_graph.png

EXEC=n

echo -n "Please enter the number of nodes: "
read NODES
echo -n "And the number of edges: "
read EDGES

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
		urxvt -e ./bin/node $i &
	done
fi
