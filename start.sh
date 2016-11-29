#!/bin/bash

GRAPHGEN=bin/graphgen
NODE=bin/node

ADDRESSFILE=doc/addresses_all.txt
ADDRESSFILE_THISRUN=doc/addresses.txt
GRAPHFILE=doc/graph.txt
GRAPHPNG=doc/graph.png
RESULTFILE=doc/results.txt

EXEC=n

echo "You can also give the node count as first argument, the edge count as second and the believeborder as a third argument. Also if the fourth one is set, the execute question will be skipped."
if [ -z ${1+x} ]
then
	echo -n "Please enter the number of nodes: "
	read NODES
else
	NODES=$1
fi

if [ -z ${2+x} ]
then
echo -n "And the number of edges: "
read EDGES
else
	EDGES=$2
fi


if [ -z ${3+x} ]
then
	echo -n "When should a rumor be believable? "
	read RUMOR
else
	RUMOR=$3
fi

echo "I will now generate a graph with $NODES nodes, $EDGES edges. Thereupon, nodes will be started which believe a rumor if heard $RUMOR times."

# Reduce the addressbook of 5000 Entries to just the number of nodes
LINES=`cat $ADDRESSFILE | wc -l`
cat $ADDRESSFILE | sed $((NODES+1)),${LINES}d > $ADDRESSFILE_THISRUN

$GRAPHGEN $EDGES $NODES
#dot -Tpng $GRAPHFILE > $GRAPHPNG
#feh -. $GRAPHPNG &

if [ -z ${4+x} ]
then
	echo -n "Execute? "
	read EXEC
else
	EXEC=y
fi

if [[ $EXEC =~ ^[Yy]$ ]]
then
	echo "###################################################################################################" >> $RESULTFILE
	echo -n "# Nodecount:	$NODES	| Edgecount:	$EDGES	| Believingborder:	$RUMOR | " >> $RESULTFILE

	for (( i=1; i<=$NODES; i++))
	do
		echo "Starting node with ID $i..."
		#urxvt -e ./bin/node $i $RUMOR &
		./bin/node $i $RUMOR &
	done
fi
