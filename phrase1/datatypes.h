//datatypes.h
//Created by Yihao on 8 / 29 / 21.

#ifndef datatypes_h
#define datatypes_h
#include <stdio.h>

//GLOBAL CONSTANTS - may be used in other files
typedef enum { False, True } Boolean;

// config data structure
typedef struct ConfigDataType; {
	double version;
	char metaDataFileName[100];
	int cpuSchedCode;
	int quantumCycles;
	int memAvailable;
	int procCycleRate;
	int ioCycleRate;
	int logToFileName[100];
}ConfigDataType;


typedef struct OpCodeType;
{
	int pid;					//pid, added when PCB is created
	char command[5];			//three Letter command quantity
	char inOutArg[5];			//for device in / out
	char strArg1[15];			//arg 1 descriptor, up to 12 chars
	int inArg2;					//cycles or memory assumes 4 bytes int
	int inArg3;					//memory,assume 4 byte int
								//also non/premption indicator
	double opEndTime;			//size of time string returned from accessTimer
	struct OpCodeType* nextNode;	//pointer to next node as needed
}OpCodeType;

#endif /* datatypes_h */
