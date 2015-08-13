#ifndef HEADER_FILE_
#define HEADER_FILE_
#include<string.h>
#include<stdlib.h>
#include<stdio.h> 
#include<string.h>
#include<sys/stat.h> 
#include<sys/types.h>
#include<fcntl.h>

#ifdef LINUX_FILEOPT
#include<dirent.h>
#include<unistd.h>
#define ACCESS access
#define MKDIR(a) mkdir((a),0755)
int L_operation_dir(char *str);
int L_display(char *str);
#elif WINDOWS_FILEOPT
#include<direct.h>
#include<io.h>
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
int W_operation_dir(char *str);
int W_display(char *str);
#endif

#endif
