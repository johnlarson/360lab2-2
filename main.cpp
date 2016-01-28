#include <stdlib.h>
#include <sys/stat.h>
#include <string>
#include "utils.h"
#include "server.h"

using namespace std;

#define ERROR -1

int main(int argc, char** argv) {
        if(argc < 3) {
                errorOut("You need to type in something in this format: server port-number directory");
        }
        int port;
        port = atoi(argv[1]);
        if(port == 0) {
                errorOut("The port has to be an integer greater than zero");
        }
        string directory = string(argv[2]);
        struct stat filestat; 
        if(stat(directory, filestat) == ERROR) {
                errorOut("That's not a directory");
        }           
        if(S_ISREG(filestat.st_mode)) {
                errorOut("I need a directory, but you gave me a regular file");
        }           
        runServer(port, directory);
}

