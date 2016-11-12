#!/bin/bash
REPEAT=y
echo -n "Please enter number of nodes: "
read NODES
echo -n "And the number of Edges: "
read EDGES
while [[ $REPEAT =~ ^[Yy]$ ]]
do
	echo -n "Creating graph..."
	./bin/graphgen $EDGES $NODES
	dot -Tpng doc/genexample_graphviz.txt > graph.png
	feh -. graph.png
	echo "...done"
	echo -n "Repeat? y/n: "
	read REPEAT
done
