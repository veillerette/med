#ifndef __HEAD_INTERFACE__
#define __HEAD_INTERFACE__

#include <stdio.h>
#include <stdlib.h>

#include "Window.h"
#include "Menu.h"
#include "Fluid.h"


#define Launch_Med() int main(int argc, char *argv[])\
{\
return Interface_Main(argc, argv);\
}\

extern int Interface_Main(int argc, char *argv[]);

#endif
