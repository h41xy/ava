// Main for the Watcher

#include "watcher.h"


/* The main method to start a single node.
 */
int main () {
	std::string watcher_id_str = "24000";
	char* watcher_id_cstr = (char*)watcher_id_str.c_str();
	Watcher watcher(watcher_id_cstr);
	watcher.run();
}
