#!/bin/bash

./bin/controller 25001 2 1
./bin/controller 25000 1 1

sleep 3

pkill node_exec
pkill controller
