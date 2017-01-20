// Constants for the application

#ifndef CONSTANTS_H
#define CONSTANTS_H

const int EXIT_NODE = 0;
const int EXIT_ALL = 1;
const int RECV_MSG = 2;
const int SOCIALISE = 3;
const int RUMOR = 4;
const int PRINT_VTIME = 5;

const int VOTE_ME = 6;
const int CAMPAIGN = 7;

const int KEEP_ON = 8;
const int NOT_YOU = 9;

const int INIT = 10; // global init signal
const int INIT_PB = 11; // candidate nodes init their partybuddies with this signal

const int ECHO_EXPLORE = 12;
const int ECHO = 13;

const int IP_BUFFER_SIZE = 15;
const int MSG_BUFFER_SIZE = 256;

const std::string WATCHER_IP = "localhost";
const int WATCHER_PORT = 25000;

const std::string LOGGER_IP = "localhost";
const int LOGGER_PORT = 25000;


const std::string ADDRESSFILE = "doc/addresses.txt";
const std::string GRAPHFILE = "doc/graph.txt";
const std::string RESULTFILE = "doc/results.txt";
const std::string CANDIDATEFILE = "doc/candidates.txt";

#endif // CONSTANTS_H
