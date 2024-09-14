#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>

#include "raw.h"

int most_recent_pid();

void neonate(int time_arg);