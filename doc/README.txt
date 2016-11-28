# To get more than 128 connections you have to modify your system.
# Typically the backlog parameter on the listen(2) systemcall is set
# by your system. Standard Linux uses a max of 128
sysctl -a | grep somaxconn
# If the nodes are started and spreading the rumor, they are bursting
# the watcher with messages depending on the nodecount.
# To increase the number of backlog connections
sysctl -w net.core.somaconn=N
# where N is your wished backlog queue.
