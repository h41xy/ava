#!/bin/bash

CANDIDATEA=$(grep 'S_ID: 16' doc/results.txt | awk -F ']' '{print $7}' | awk '{print $2}' | awk -F ':' '{sum += $2} END {print sum}')
CANDIDATEB=$(grep 'S_ID: 16' doc/results.txt | awk -F ']' '{print $7}' | awk '{print $3}' | awk -F ':' '{sum += $2} END {print sum}')

echo "Candidate 1 has $CANDIDATEA votes and Candidate 2 has $CANDIDATEB votes."
