#include "configops.h"
#include "metadataops.h"
#include "simtimer.h"
#include "simulator.h"
#include "StringUtils.h"

//use posix thread
#include <pthread.h>

char* memory_alloca = NULL;
process_t* root_process_list, * process_list;
int running_process_identifier = 0;

/*
Function Name: output
Algorithm: Output the message with given config
Precondition : given pointer to config data
Postcondition: Output to the monitor, file or both.Exceptions: none
Notes: none
*/
void output(char* message, ConfigDataType* configPtr) {
	char* value = (char*)malloc(sizeof(char) * 100);
	accessTimer(LAP_TIMER, value);
	//if log to file,then write file
	if (configPtr->logToCode == LOGTO_FILE_CODE) {
		FILE* fp = fopen(configPtr->logToFileName, "a+");
		fprintf(fp, "[ %s] %s\n", value, message);
		fclose(fp);
	}
	//if both,then write to file and to screen
	if (configPtr->logToCode == LOGTO_BOTH_CODE) {
		FILE* fp = fopen(configPtr->logToFileName, "a+");
		fprintf(fp, "[ %s] %s\n", value, message);
		fclose(fp);
		printf("[ %s] %s\n", value, message);
	}
	//if screen only, then write to screen
	if (configPtr->logToCode == LOGTO_MONITOR_CODE) {
		printf("[ %s] %s\n", value, message);
	}
	free(value);
}

/*
Function Name: msleep
Algorithm: sleep some time in milliseconds.
	only used by POSIx thread
Precondition: The pointer to the time interval
Postcondition: Block the thread for some time.
Exceptions: none
Notes: none
*/
void* msleep(void* args) {
	double millisecond = *((double*)args);
	char dummy[100];
	double start_time = accessTimer(LAP_TIMER, dummy);
	while(1000 * (accessTimer(LAP_TIMER, dummy) - start_time) <= millisecond);
	return NULL;
}


/*
Function Name: timer
Algorithm: sleep some time in milliseconds .
Precondition: Given the time for sleeping
Postcondition : Block the thread for some time.Exceptions: none
Notes: none
*/
void timer(double millisecond) {
	if (millisecond <= 1e-3)return;
	pthread_t pid;
	double* ptr = (double*)malloc(sizeof(double));
	*ptr = millisecond;
	//create POSIX thread for simulating the IO operation
	pthread_create(&pid, NULL, msleep, (void*)ptr);
	//join the thread and block it
	pthread_join(pid, NULL);
	free(ptr);
}

/*
Function Name: mem_alloca_func
Algorithm: simulate the memory allocation operation
Precondition: Given the address range
Postcondition: currently , none
Exceptions: none
Notes: none
*/
Boolean mem_alloca_func(int dummy1, int dummy2) {
	return True;
}

/*
Function Name: mem_access_func
Algorithm: simulate the memory access operation
Precondition: Given the address
Postcondition: currently, noneExceptions: none
Notes : none
*/
Boolean mem_access_func(int dummy) {
	return True;
}

/*
Function Name: runProcess
Algorithm: master the process and execute the process one by one
Precondition: given the head pointer of the process configuration and the config data type
Postcondition: Provide whether should go further running
Exceptions: none
Notes: none
*/
Boolean runProcess(process_t* current_process, ConfigDataType* ConfigDataType) {
	//if it is system end,then stop running
	if (current_process->end_flag == 1) {
		output("OS:System stop", ConfigDataType);
		return False;
	}
	//for system start, just go on running
	if (current_process->start_flag == 1) {
		return True;
	}
	int i;
	//set state
	current_process->state = PROCESS_STATE_READY;
	for (i = 0; i < current_process->exe_size; i++) {
		//for each process, running from the first execution flow to the end
		switch (current_process->execution_flow[i].command) {
		case APP:
		{
			//app start
			if (current_process->execution_flow[i].strArgs1 == START) {
				timer(current_process->execution_flow[i].intArg2 *
					ConfigDataType->quantumCycles);
				//create the output message char array
				char* value = (char*)malloc(sizeof(char) * 1000);
				sprintf(value, "OS: Process %d set to RUNNING state from READY state",
					running_process_identifier);
				output(value, ConfigDataType);
				//after output,free it
				free(value);
				//set process state
				current_process->state = PROCESS_STATE_RUN;
			}
			else if (current_process->execution_flow[i].strArgs1 == END) {
				//app end
				timer(current_process->execution_flow[i].intArg2 *
					ConfigDataType->quantumCycles);
				//craete the ouput message char array and free it after output
				char* value = (char*)malloc(sizeof(char) * 1000);
				sprintf(value, "OS: Process %d set to EXITED state from RUNNING state.",
					running_process_identifier);
				output(value, ConfigDataType);
				free(value);
				running_process_identifier++;
				current_process->state = PROCESS_STATE_END;
			}
			break;
		}
		case DEVIN: {
			//create the output message char array and free it after output
			char* value = (char*)malloc(sizeof(char) * 1000);
			sprintf(value, "Process: %d device input %s start, time: %d ms.",
				running_process_identifier, current_process->execution_flow[i].origin->strArg1,
				current_process->execution_flow[i].intArg2 * ConfigDataType->quantumCycles *
				ConfigDataType->ioCycleRate);
			output(value, ConfigDataType);

			timer(current_process->execution_flow[i].intArg2 * ConfigDataType->quantumCycles *
				ConfigDataType->ioCycleRate);
			sprintf(value, "Process: %d device input %s end.", running_process_identifier,
				current_process->execution_flow[i].origin->strArg1);
			output(value, ConfigDataType);
			free(value);
			break;
		}
		case DEVOUT: {
			//create the output message char array and free it after output
			char* value = (char*)malloc(sizeof(char) * 1000);
			sprintf(value, "Process: %d device output %s start, time: %d ms.",
				running_process_identifier, current_process->execution_flow[i].origin->strArg1,
				current_process->execution_flow[i].intArg2 * ConfigDataType->quantumCycles *
				ConfigDataType->ioCycleRate);
			output(value, ConfigDataType);

			timer(current_process->execution_flow[i].intArg2 * ConfigDataType->quantumCycles *
				ConfigDataType->ioCycleRate);
			sprintf(value, "Process: %d device output %s end.", running_process_identifier,
				current_process->execution_flow[i].origin->strArg1);
			output(value, ConfigDataType);
			free(value);
			break;
		}
		case CPU: {
			// create the output message char arrayand free it after output
            char* value = (char*)malloc(sizeof(char) * 1000);
			sprintf(value, "Process: %d CPU Process,time: %d ms.", running_process_identifier,
				current_process->execution_flow[i].intArg2 * ConfigDataType->quantumCycles);
			output(value, ConfigDataType);
			timer(current_process->execution_flow[i].intArg2 * ConfigDataType->quantumCycles);
			sprintf(value, "Process: %d CPU Process end.", running_process_identifier);
			output(value, ConfigDataType);
			free(value);
			break;
		}
		case MEM: {
			if (current_process->execution_flow[i].strArgs1 == ALLOCATE) {
				// create the output message char arrayand free it after output
				char* value = (char*)malloc(sizeof(char) * 1000);
				sprintf(value, "Process: %d Memory allocation from %d to %d.",
					running_process_identifier,current_process->execution_flow[i].intArg2,
					current_process->execution_flow[i].intArg3);
				output(value, ConfigDataType);
				// check whether allocation area can be allocated
				Boolean result = mem_alloca_func(current_process->execution_flow[i].intArg2,
					current_process->execution_flow[i].intArg3);
				if (result) sprintf(value, "Process: %d Memory allocation from %d to %d success.",
					running_process_identifier,
					current_process->execution_flow[i].intArg2,
					current_process->execution_flow[i].intArg3);
				else sprintf(value, "Process: %d Memory allocation from %d to %d failed.",
					running_process_identifier, current_process->execution_flow[i].intArg2,
					current_process->execution_flow[i].intArg3);
				output(value, ConfigDataType);
				free(value);
				break;
			}
			else if (current_process->execution_flow[i].strArgs1 == ACCESS) {
				//create the output message char arrayand free it after output
				char* value = (char*)malloc(sizeof(char) * 1000);
				sprintf(value, "Process: %d Memory access to %d. ", running_process_identifier,
					current_process->execution_flow[i].intArg2);
				output(value, ConfigDataType);
				//check whether the psition can be accessed
				Boolean result = mem_access_func(current_process->execution_flow[i].intArg2);
				if (result) sprintf(value,"Process: %d Memory access to %d success.",
					running_process_identifier, current_process->execution_flow[i].intArg2);
				else sprintf(value,"Process: %d Memory access to %d failed.",
					running_process_identifier, current_process->execution_flow[i].intArg2);
				output(value, ConfigDataType);
				free(value);
				break;
			}
		}
            case SYS:break;
		}
	}
	return True;
}

/*
Function Name: initial_process
Algorithm: Initial the process status and output the messageplace holder
Precondition: The process count and the config
Postcondition: Output the message
Exceptions: none
Notes: none
*/
void initial_process(int n, ConfigDataType* configPtr) {
	char value[1007];
	int i;
	for (i = 0; i < n; ++i) {
		sprintf(value, "OS: Process %d set to READY state from NEW state.", i);
		output(value, configPtr);
	}
}

/*
Function Name : memset_usr
Algorithm: set the memory area with the same value
Precondition: The memory pointer and the count with value
Postcondition: The filled memory area.
Exceptions: none
Notes: none
*/
void memset_usr(char* ptr, int count, char value) {
	int i;
	for (i = 0; i < count; ++i) ptr[i] = value;
}

/*
Function Name: runSims
Algorithm: master driver for simulator operations ;
conducts os simulation with varyin sceduling strategiesand vary numbers of processes
Precondition: given head pointer to config data and metadata
Postcondition: simulator is provided,file output is provided as configuredExceptions: none
Notes: none
*/
void runSim(ConfigDataType* configPtr, OpCodeType* metaDataMsterPtr) {
	//Make the timer enabled
	runTimer(0);
	//make the dummy char array for timer
	char* value = (char*)malloc(sizeof(char) * 100);
	//reset the timer here
	accessTimer(ZERO_TIMER, value);
	//output the simulation start message
	output("OS: Simulation Start", configPtr);
	accessTimer(LAP_TIMER, value);
	//set the running flag to be True
	Boolean simRun = True;
	//malloc the initial process list object and initialize the value to zero
	process_list = (process_t*)malloc(sizeof(process_t));
	memset_usr((char*)process_list, sizeof(process_t) / sizeof(char), 0);
	//set up the head pointer of the list
	root_process_list = process_list;

	OpCodeType* ptr = metaDataMsterPtr;
	process_t* prevProcess = NULL;
	OpCodeType* app_start, * app_end;
	int range = 0;
	int process_cnt = 0;
	//visit all the config pointer and make it into process information
	while (ptr != NULL) {
		//sys start and end situation
		if (compareString(ptr->command, "sys") == 0) {
			//start
			if (compareString(ptr->strArg1, "start") == 0) process_list->start_flag = 1;
			else process_list->end_flag = 1;
			// if this is not the first process, set the link to the previous process
			if (prevProcess != NULL) prevProcess->next = process_list;
			// update the previous process
			prevProcess = process_list;
			// malloc the initial process list objectand initialize the value to zero
			process_list = (process_t*)malloc(sizeof(process_t));
			memset_usr((char*)process_list, sizeof(process_t) / sizeof(char),0);
			process_cnt++;
		}
		else if (compareString(ptr->command, "app") == 0) {
			//app start and end
			if (compareString(ptr->strArg1, "start") == 0) {
				// set the range sizeand the pointer
				range = 1;
				app_start = ptr;
			}
			else if (compareString(ptr->strArg1, "end") == 0) {
				// set the rangeand the end pointer
				range++;
				app_end = ptr;
				// malloc the initial execution list objectand initialize the value to zero
				process_list->execution_flow = (executable_t*)malloc(sizeof(executable_t) *
					(range + 7));
				memset_usr((char*)process_list->execution_flow, (sizeof(executable_t) * (range +
					7)) / sizeof(char), 0);
					// set up the size
					process_list->exe_size = range;
					int command_idx = 0;
					struct OpCodeType* app_ptr;
					// make the original string type into defined type
					// visit from the first pointer to the end(application range)
					for (app_ptr = app_start; app_ptr != app_end; app_ptr = app_ptr->nextNode) {
						if (compareString(app_ptr->strArg1, "app") == 0) {
							//app start
							if (compareString(app_ptr->strArg1, "start") == 0) {
								process_list->execution_flow[command_idx].command = APP;
								process_list->execution_flow[command_idx].strArgs1 = START;
							}
							else if (compareString(app_ptr->strArg1, "end") == 0) {
								// app end
								process_list->execution_flow[command_idx].command = APP;
								process_list->execution_flow[command_idx].strArgs1 = END;
							}
						}
						else if (compareString(app_ptr->command, "cpu") == 0) {
							// cpu command
							if (compareString(app_ptr->strArg1, "process") == 0) {
								process_list->execution_flow[command_idx].command = CPU;
								process_list->execution_flow[command_idx].strArgs1 = PROCESS;
							}
						}
						else if (compareString(app_ptr->command, "dev") == 0) {
							if (compareString(app_ptr->inOutArg, "in") == 0) {
								// device in
								process_list->execution_flow[command_idx].command = DEVIN;
							}
							else if (compareString(app_ptr->inOutArg, "out") == 0) {
								// device out
								process_list->execution_flow[command_idx].command = DEVOUT;
							}
							// device type
							if (compareString(app_ptr->strArg1, "monitor") == 0)
								process_list->execution_flow[command_idx].strArgs1 = MONITOR;
							if (compareString(app_ptr->strArg1, "sound signal") == 0)
								process_list->execution_flow[command_idx].strArgs1 = SOUND_SIGNAL;
							if (compareString(app_ptr->strArg1, "ethernet") == 0)
								process_list->execution_flow[command_idx].strArgs1 = ETHERNET;
							if (compareString(app_ptr->strArg1, "hard drive") == 0)
								process_list->execution_flow[command_idx].strArgs1 = HDD;
							if (compareString(app_ptr->strArg1, "keyboard") == 0)
								process_list->execution_flow[command_idx].strArgs1 = KEYBOARD;
							if (compareString(app_ptr->strArg1, "serial") == 0)
								process_list->execution_flow[command_idx].strArgs1 = SERIAL;
							if (compareString(app_ptr->strArg1, "video signal") == 0)
								process_list->execution_flow[command_idx].strArgs1 = VIDEO_SIGNAL;
							if (compareString(app_ptr->strArg1, "usb") == 0)
								process_list->execution_flow[command_idx].strArgs1 = USB;

						}
						else if (compareString(app_ptr->command, "mem") == 0) {
							process_list->execution_flow[command_idx].command = MEM;
							if (compareString(app_ptr->strArg1, "access") == 0)
								// memory access
								process_list->execution_flow[command_idx].strArgs1 = ACCESS;
							else if (compareString(app_ptr->strArg1, "allocate") == 0)
								//memory allocation
								process_list->execution_flow[command_idx].strArgs1 = ALLOCATE;
						}
						//int args
						process_list->execution_flow[command_idx].intArg2 = app_ptr->intArg2;
						process_list->execution_flow[command_idx].intArg3 = app_ptr->intArg3; process_list->execution_flow[command_idx].origin = app_ptr;
						process_list->state = PROCESS_STATE_NEW;
						command_idx += 1;
					}
					if (prevProcess != NULL) prevProcess->next = process_list;
					// update the previous process
					// malloc the initial process list object and initialize the value to zero
					prevProcess = process_list;
					process_list = (process_t*)malloc(sizeof(process_t));
					memset_usr((char*)process_list, sizeof(process_t) / sizeof(char), 0);
					process_cnt++;
			}
		}
		else {
			range++;
		}
		//set connection with next node
		ptr = ptr->nextNode;
	}
	output("Os: system start", configPtr);
	initial_process(process_cnt, configPtr);
	free(process_list);
	process_list = root_process_list;

	while (simRun) {
		//run from the first process
		simRun = runProcess(process_list, configPtr);
		if (!simRun)break;
		process_list = process_list->next;
	}
	output("OS: Simulation End", configPtr);
	//after simulation, free the memory
	while (root_process_list != NULL)
	{
		process_t* p = root_process_list->next;
		free(root_process_list->execution_flow);
		free(root_process_list);
		root_process_list = p;
	}
	free(value);
}
