#ifndef SIMULATOR_H
#define SIMULATOR_H

//HEADER FILE
#include <stdio.h>
#include <stdlib.h>
#include "StringUtils.h"
#include "configops.h"

typedef enum command_list {
	SYS, APP, DEVI, DEVOUT, CPU, EME
}command_t;

typedef enum string_args_list {
	START, END, PROCESS, ALLOCATE, ACCESS, ETHERNET, HDD, KEYBOARD, MONITOR, SERIAL, SOULD_SIGNAL,
	USB, VIDEO_SIGNAL
}sring_args_t;


typedef struct executable {
	command_t command;
	string_args_t strArgs1;
	int intArg2, intArg3;
	OpCodeType* origin;
}executable_t;

#define PROCESS_STATE_NEW 0
#define PROCESS_STATE_READY 1
#define PROCESS_STATE_RUN 2
#define PROCESS_STATE_END 3

typedef struct process {
	struct process* next;
	exetutable_t* execution_flow;
	int exe_size;
	int start_flag, end_flag;
	int state;
}process_t;

//Function prototypes
void runSim(ConfigDataType* configPtr, OpCodeType* metaDataMsterPtr);



#endif // !SIMULATOR_H
