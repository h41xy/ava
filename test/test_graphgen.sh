#!/bin/bash
REPEAT=y
EXE=./bin/graphgen
GRAPH=doc/graph.png
echo -n "Please enter number of nodes: "
read NODES
echo -n "And the number of Edges: "
read EDGES
while [[ $REPEAT =~ ^[Yy]$ ]]
do
	echo -n "Creating graph..."
	$EXE $EDGES $NODES
	dot -Tpng doc/genexample_graphviz.txt > $GRAPH
	feh -. $GRAPH
	echo "...done"
	echo -n "Repeat? y/n: "
	read REPEAT
done
