#!/bin/bash

#GRAPHGEN=bin/graphgen
GRAPHGEN=bin/usa_graphgen
NODE=bin/node_exec

ADDRESSFILE=doc/addresses_all.txt
ADDRESSFILE_THISRUN=doc/addresses.txt
GRAPHFILE=doc/graph.txt
GRAPHPNG=doc/graph.png
RESULTFILE=doc/results.txt
CANDIDATEFILE=doc/candidates.txt

EXEC=n

echo "Usage: start <nodecount> <candidatecount> <party buddies> <friendcount> <responsecount> [<set if you want to execute all right after generation>]"
if [ -z ${1+x} ]
then
	echo -n "Number of nodes: "
	read NODES
else
	NODES=$1
fi

if [ -z ${2+x} ]
then
	echo -n "Number of candidates: "
	read CANDIDATES
else
	CANDIDATES=$2
fi


if [ -z ${3+x} ]
then
	echo -n "Number of party buddies of each candidate: "
	read PBUDDIES
else
	PBUDDIES=$3
fi

if [ -z ${4+x} ]
then
	echo -n "Number of friends (neighbors) every voter has: "
	read FRIENDS
else
	FRIENDS=$4
fi

if [ -z ${5+x} ]
then
	echo -n "Number of responses until a candidate sends another signal: "
	read RCOUNT
else
	RCOUNT=$5
fi


echo "A graph will be generated with $NODES nodes, $CANDIDATES candidates with $PBUDDIES party buddies and every voter has $FRIENDS friends (neighbors)."

# Reduce the addressbook of 5000 Entries to just the number of nodes
LINES=`cat $ADDRESSFILE | wc -l`
cat $ADDRESSFILE | sed $((NODES+1)),${LINES}d > $ADDRESSFILE_THISRUN

#write the candidates to file
for(( i=1; i<=$CANDIDATES; i++))
do
	echo $i >> $CANDIDATEFILE
done

$GRAPHGEN $NODES $CANDIDATES $PBUDDIES $FRIENDS
#dot -Tpng $GRAPHFILE > $GRAPHPNG
#feh -. $GRAPHPNG &

if [ -z ${6+x} ]
then
	echo -n "Execute? "
	read EXEC
else
	EXEC=y
fi

if [[ $EXEC =~ ^[Yy]$ ]]
then
#	echo "###################################################################################################" >> $RESULTFILE
#	echo -n "# Nodecount:	$NODES	| Edgecount:	$EDGES	| Believingborder:	$RUMOR | " >> $RESULTFILE

	for (( i=1; i<=$CANDIDATES; i++))
	do
		echo "starting candidate node with ID $i..."
		#urxvt -e ./bin/node_exec $i $RCOUNT &
		./bin/node_exec $i $RCOUNT  >> doc/candidate.log &
	done

	for (( i=(($CANDIDATES+1)); i<=$NODES; i++))
	do
		echo "Starting voter node with ID $i..."
		#urxvt -e ./bin/node_exec $i &
		./bin/node_exec $i  >> doc/voter.log &
	done
fi
