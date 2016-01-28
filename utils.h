#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <string>
#include "utils.h"

using namespace std;

void errorOut(string msg);

int stat(string directory, struct stat filestat);

static inline int isWhitespace(char c);

void chomp(char *line);

string GetLine(int fds);

#endif
