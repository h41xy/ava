// Constants for the application

#ifndef CONSTANTS_H
#define CONSTANTS_H

// Dummy Signal
const int STD_SIGNAL = 0;

const int EXIT_NODE = 1;
const int EXIT_ALL = 2;
const int RECV_MSG = 3;

const int REQUEST = 4;
const int ACKNOWLEDGE = 5;
const int RELEASE = 6;

const int INIT = 7;
const int INIT_ALL = 8;

const int TERMINATE = 9;

const int IP_BUFFER_SIZE = 15;
const int MSG_BUFFER_SIZE = 256;

const int TERM_BORDER = 3;

const int STD_LTIME = 0;
const std::string STD_STRING = "";

const std::string LOGGER_IP = "localhost";
const int LOGGER_PORT = 25000;

const std::string ADDRESSFILE = "doc/addresses.txt";
const std::string SHARED_FILE = "doc/shared.res";
const std::string LOGGERFILE = "doc/run.log";

#endif // CONSTANTS_H
