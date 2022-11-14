//header file.
#include "configops.h"
/*
Function Name: clearConfigData
Algorithm: frees allocated memory for config data
Precondition: configData holds allocated data set
Postcondition: memory is freed,pointer is set to NULL
Exceptions: none
Notes : none
*/
ConfigDataType* clearConfigData(ConfigDataType* configData)
{
	//check that config data pointer is not null
	if (configData  != NULL)
	{
		// free data structure memory 
		// function: free
		free(configData);
		// set config data to null
		configData = NULL;
	}
	// set config data pointer to null(returned as parameter)
	return NULL; // temporary stub return
}

/*
Function Nare : conrigCodeToString
Algorithn : utility function to support display
			of CPU schrduling or Log To code strings
Precondition : code vadiable holds constant value from ConfigDatacodes
			for item (e.g., CPU_SCHED_SRTF_P, LOGTO_FILE_CODE,etc.)
Postcondition : string parameter holds correct string
			associated with the given cnnstant
Exceptians : none
Notes : none
*/
void configCodeToString(int code, char* outString)
{
	//Define array with eight items, and short(10)lengths
	char displayStrings[8][10] = { "SJF-N","SRTF-P","FCFS-P",
	"RR-P","FCFS-N","Monitor","File","Both" };
	//copy string to return parameter
	//function : copyString
		copyString(outString, displayStrings[code]);
}

/*
Function Name: displayConfigData
Algorithm: diagnostic function to show config data output
Precondition: configData holds allocated data set
Postcondition: data is printed to screen
Exceptions: none
Notes: none
*/
void displayConfigData(ConfigDataType* configData)
{
	//initilizae function / varibales
	char displayString[STD_STR_LEN];

	//print lines of display
	//function : printf, configCodeToString(translates coded items)
	printf("Config File Display\n");
	printf("-------------------\n"); 
	printf("version               : %3.2f\n",configData-> version);
	printf("Program file name     : %s\n",configData->metaDataFileName);
	configCodeToString(configData->cpuSchedCode,displayString);
	printf("CPU schedule selection: %s\n",displayString);
	printf("Quantum time          : %d\n",configData->quantumCycles);
	printf("Memory Available      : %d\n",configData->memAvailable);
	printf("Process cycle rate    : %d\n",configData->procCycleRate);
	printf("I/O cycle rate        : %d\n",configData->ioCycleRate);
	configCodeToString(configData->logToCode, displayString);
	printf("Log to selection      : %s\n",displayString);
	printf("Log file name         : %s\n\n",configData->logToFileName);
}

/*
Function Name: getConfigData
Algorithm: opens file,acquires configuration data,
			returns pointer to data structure
precondition: For correct operation, file is avaible,is formatted correctly,
			and has all configuration lines and data although
			the configuration lines are not required to bein specific order
Postcondition: in correct operation,
			returns pointer to correct configuration data structure
Exceptions: correctly and appropriately (without program failure)
			responds to and reports file access faiure,
			incorrectly formatted lead or end descriptors,
			incorrectly formatted prompt,data out of range,
			and incomplete file conditions
Notes: none
*/
Boolean getConfigData(char* fileName, ConfigDataType** configData, char* endStateMsg)
{
	// initialize function / variables
	// set constant number of data lines
	const int NUM_DATA_LINES = 9;

	//set read only constant
	const char READ_ONLY_FLAG[] = "r";

	//create pointer for data input
	ConfigDataType* tempData;

	//declare other variables
	FILE* fileAccessPtr;
	char dataBuffer[MAX_STR_LEN];
	int intData, dataLineCode, lineCtr = 0;
	double doubleData;
	Boolean dontStopAtNonPrintable = False;

	// set endStateMsg to success
	// function : copyString
	copyString(endStateMsg, "Configuration file upload successful");

	// initialize config data pointer in case of return error
	*configData = NULL;

	// open file
	// function : fopen
	fileAccessPtr = fopen(fileName, READ_ONLY_FLAG);

	// check for file open failure
	if (fileAccessPtr == NULL)
	{
		// set end state message to config file access error
		// function: copyString
		copyString(endStateMsg, "Configuration file access error");
		// return file access error
		return False;
	}
	// get first line, check for failurell 
	// function : getLineTo
	if (getLineTo(fileAccessPtr, MAX_STR_LEN, COLON,
		dataBuffer,IGNORE_LEADING_WS,dontStopAtNonPrintable) != NO_ERR
		|| compareString(dataBuffer,"Start Simulator Configuration File") != STR_EQ)
	{
		// close file access
		// function : fclose
		fclose(fileAccessPtr);
		// set end state message to corrupt leader line error
		//function : copyString
		copyString(endStateMsg, "Corrupt configuration leader liner error");
		// return corrupt file data
		return False;
	}

	// create temporary pointer to configuratio data structure
	tempData = (ConfigDataType*)malloc(sizeof(ConfigDataType));

	// Loop to end of config data items
	while (lineCtr < NUM_DATA_LINES)
	{
		// get line leader, check for failure
		//function : getLineTo
		if (getLineTo(fileAccessPtr,MAX_STR_LEN, COLON,
			dataBuffer, IGNORE_LEADING_WS, dontStopAtNonPrintable) != NO_ERR)
		{
			// free temp struct memory
		//function : free
			free(tempData);
			//close file accessll 
				//function : fclose
			fclose(fileAccessPtr);
			// set end state message to line capture failure
				//function: copyString
			copyString(endStateMsg, "Configuration start line capture error");
			// return incomplete file error
			return False;
		}

		//find correct data line by number
				//function: getDataLineCode
		dataLineCode = getDataLineCode(dataBuffer);

		// check for data line found
		if (dataLineCode != CFG_CORRUPT_PROMT_ERR)
		{
			// Get data value
			// Check for version number(double value)
			if (dataLineCode == CFG_VERSION_CODE)
			{
				// Get version number
				// function: fscanf
				fscanf(fileAccessPtr, "%lf", &doubleData);
			}

			//Otherwise, check for metaData or LogTo file names
			//or CPU scheduling names(strings)
			else if (dataLineCode == CFG_MD_FILE_NAME_CODE
				|| dataLineCode == CFG_LOG_FILE_NAME_CODE
				|| dataLineCode == CFG_CPU_SCHED_CODE
				|| dataLineCode == CFG_LOG_TO_CODE)
			{
				//get string input
				// function: fscanf
				fscanf(fileAccessPtr, "%s ", dataBuffer);
			}
			// Otherwise, assume integer data
			else
			{
				// get integer input
				// function : fscanf
				fscanf(fileAccessPtr, "%d", &intData);
			}

			// check for data value in range
			//function : valueInRange
			if (valueInRange(dataLineCode,intData, doubleData, dataBuffer)
				== True)
			{
				// assign to data pointer depending on config item
				//function : getCpuSchedCode, getLogToCode
				switch (dataLineCode)
				{
				case CFG_VERSION_CODE:

					tempData->version = doubleData;
					break;

				case CFG_MD_FILE_NAME_CODE:

					copyString(tempData->metaDataFileName, dataBuffer);
					break;

				case CFG_CPU_SCHED_CODE:

					tempData->cpuSchedCode = getCpuSchedCode(dataBuffer);
					break;

				case CFG_QUANT_CYCLES_CODE:

					tempData->quantumCycles = intData;
					break;

				case CFG_MEM_AVAILABLE_CODE:

					tempData->memAvailable = intData;
					break;

				case CFG_PROC_CYCLES_CODE:

					tempData->procCycleRate = intData;
					break;

				case CFG_IO_CYCLES_CODE:

					tempData->ioCycleRate = intData;
					break;

				case CFG_LOG_TO_CODE:

					tempData->logToCode = getLogToCode(dataBuffer);
					break;

				case CFG_LOG_FILE_NAME_CODE:

					copyString(tempData->logToFileName, dataBuffer);
					break;

				};
			}

			// Otherwise, assume data value not in range
			else
			{
				// free temp struct memory
				//function : free
				free(tempData);

				//close file access
				// function : fclose
				fclose(fileAccessPtr);

				//set end state message to configuration data out of range
				//function : copyString
				copyString(endStateMsg, "Configuration item out of range");

				//Return data out of range
				return False;
			}
		}

		// Otherwise, assume data line not found
		else
		{
			// Free temp struct memory
			//function : free
			free(tempData);

			// close file access
			//function: fclose
			fclose(fileAccessPtr);

			// Set end state message to configuration corrupt prompt error
			// function: copyString
			copyString(endStateMsg, "Corrupted configuration prompt");

			//Return corrupt config file code
			return False;
		}
		// increment line counter
		lineCtr++;
	}
	// end master loop

	// acquire end of sim config string
	// function: getLineTo
	if (getLineTo(fileAccessPtr, MAX_STR_LEN,PERIOD,
		dataBuffer, IGNORE_LEADING_WS,dontStopAtNonPrintable) != NO_ERR
		|| compareString(dataBuffer, "End Simulator Configuration File")
		!= STR_EQ)
	{
		// Free temp struct memory
		// function: free
		free(tempData);

		// Close file access
		// function: fclose
		fclose(fileAccessPtr);

		// set end state message to corrupt 0uration end line
		// function: copyString
		copyString(endStateMsg,"Configuration end line capture error");

		// return correct file data
		return False;
	}

	// assign temporary pointer to parameter return pointer
	*configData = tempData;

	// close file access
	// function: fclose
	fclose(fileAccessPtr);

	// return no error code
	return True;
}

/*
Function Name: getCpuschedCode
Algorithm: converts string data (e.g., "SJF-N","SRTF-P")
		to constant code number to be stored as integer
Precondition: codeStr is a c-Style string with one
			of the specified cpu scheduling operations
Postcondition: returns code representing scheduling actions
Exceptions: defaults to fcfs code
Notes: none
*/
ConfigDataCodes getCpuSchedCode(char* codeStr)
{
	// initialize function / variables
	// set up temporary string for testing
	// fucntion : getStringLength, malloc
	int strLen = getStringLength(codeStr);
	char* tempStr = (char*)malloc(strLen + 1);

	// set default return to FCFS - N
	int returnVal = CPU_SCHED_FCFS_N_CODE;

	// set string to lower case for testing
	// function : setStrToLowerCase
	setStrToLowerCase(tempStr, codeStr);

	// check for SJF - N
	// function : compareString
	if (compareString(tempStr, "sjf-n") == STR_EQ) {
		// set return code to sjf
		returnVal = CPU_SCHED_SJF_N_CODE;
	}

	//  check for SRTF - P
	//  function: compareString
	if (compareString(tempStr, "srtf-p") == STR_EQ) {
		//  set return code to srtf
		returnVal = CPU_SCHED_SRTF_P_CODE;
	}

	//  check for FCFS - p
	//  function : compareString
	if (compareString(tempStr, "fcfs-p") == STR_EQ) {
		//  set return code to fcfs
		returnVal = CPU_SCHED_FCFS_P_CODE;
	}

	// check for RR - P
	// function : compareString
	if (compareString(tempStr, "rr-p") == STR_EQ) {
		//  set return code to rr
		returnVal = CPU_SCHED_RR_P_CODE;
	}
	//  free temp string memory
	// function : free
	free(tempStr);

	//  return resulting value
	return returnVal;
}


/*
Function Name: getDataLineCode
Algorithm: tests string for one of known leaer strings,returns line number
		if string is correct, returns cFG_CORRUPT_DATA_ERR
		if string is not found
Precondition: dataBuffer is valid C-Style string
Postcondition: returns line number of data item in terms of constant
				(e.g., CFG_VERSION_CODE,CFG_CPu_SCHED_CODE,etc.)
Exceptions: returns CFG_CORRUPT_FILE_ERR if string is not identified
Notes: none
*/
int getDataLineCode(char* dataBuffer) {
	// return appropriate code depending on prompt string provided
	// function : compareString
	if (compareString(dataBuffer, "Version/Phase") == STR_EQ) {
		return CFG_VERSION_CODE;
	}

	if (compareString(dataBuffer, "File Path") == STR_EQ) {
		return CFG_MD_FILE_NAME_CODE;
	}

	if (compareString(dataBuffer, "CPU Scheduling Code") == STR_EQ) {
		return CFG_CPU_SCHED_CODE;
	}

	if (compareString(dataBuffer, "Quantum Time (cycles)") == STR_EQ) {
		return CFG_QUANT_CYCLES_CODE;
	}

	if (compareString(dataBuffer,"Memory Available (KB)") == STR_EQ) {
		return CFG_MEM_AVAILABLE_CODE;
	}

	if (compareString(dataBuffer,"Processor Cycle Time (msec)") == STR_EQ) {
		return CFG_PROC_CYCLES_CODE;
	}

	if (compareString(dataBuffer, "I/O Cycle Time (msec)") == STR_EQ) {
		return CFG_IO_CYCLES_CODE;
	}

	if (compareString(dataBuffer, "Log To") == STR_EQ) {
		return CFG_LOG_TO_CODE;
	}

	if (compareString(dataBuffer, "Log File Path") == STR_EQ) {

		return CFG_LOG_FILE_NAME_CODE;
	}

	return CFG_CORRUPT_PROMT_ERR;
}


/*
Function Name: getLogToCode
Algorithm: converts string data (e.g.,"File","Monitor")
		to constant code number to be stored as integer
Precondition: codeStr is a C-style string with one
			of the specified log to operations
Postcondition: returns code representing log to actions
Exceptions: defaults to monitor code
Notes: none
*/
ConfigDataCodes getLogToCode(char* logToStr) {
	// initialize function / variables
	// 
	// create temporary string
	// function : getStringLength, malloc
	int strLen = getStringLength(logToStr);
	char* tempStr = (char*)malloc(strLen + 1);

	// set default to log to monitor
	int returnVal = LOGTO_MONITOR_CODE;

	// set temp string to lower case
	// function : setStrToLowerCase
	setStrToLowerCase(tempStr, logToStr);

	//  Check for BOTH
	//  function : compareString
	if (compareString(tempStr, "both") == STR_EQ) {
		//  set return calue to both code
		returnVal = LOGTO_BOTH_CODE;
	}
	//  Check for FILE
	// function : compareString
	if (compareString(tempStr, "file") == STR_EQ) {
		//  set return value to file code
		returnVal = LOGTO_FILE_CODE;
	}
	// Free temp string memory
	// function: free
	free(tempStr);
	// return found code
	return returnVal;
}

/*
Function Name: valueInRange
Algorithm: tests one of three values (int,double,string) for being
		in specified range, depending on data code
		(i.e., specified config value)
Precondition: one of the three data values is valid
Postcondition: returns True if data is within specified parameters,
				False otherwise
Exceptions: metaData or logfile names are ignored and return True
Notes: none
*/
Boolean valueInRange(int lineCode, int intVal, double doubleVal, char* stringVal) {
	// initialize function / variables
	Boolean result = True;
	char* tempStr;
	int strLen;

	// use line code to identify prompt line
	switch (lineCode)
	{
		//  for version code
	case CFG_VERSION_CODE:

		//  check if limits of version code are exceeded
		if (doubleVal < 0.00 || doubleVal > 10.0)
		{
			//  set Boolean result to false
			result = False;
		}
		break;

		//  for cpu scheduling code
	case CFG_CPU_SCHED_CODE:

		//  create temporary string and set to lower case
		//  function: getStringLength, malloc, setStrToLowerCase
		strLen = getStringLength(stringVal);
		tempStr = (char*)malloc(strLen + 1);
		setStrToLowerCase(tempStr, stringVal);

		//  check for not finding one of the scheduling strings
	//  function : compareString
		if (compareString(tempStr, "fcfs-n") != STR_EQ
			&& compareString(tempStr, "sjf-n") != STR_EQ
			&& compareString(tempStr, "srtf-p") != STR_EQ
			&& compareString(tempStr, "fcfs-p") != STR_EQ
			&& compareString(tempStr, "rr-p" ) != STR_EQ)
		{
			//  set boolean result to false
			result = False;
		}
		//  free temp string memory
		// function : free
		free(tempStr);
		break;

		//  for quantum cycles
	case CFG_QUANT_CYCLES_CODE:

		//  check for quantum cycles limits exceeded
		if (intVal < 0 || intVal > 100)
		{
			//  set boolean result to false
			result = False;
		}
		break;

		//  for memory avaible
	case CFG_MEM_AVAILABLE_CODE:

		// check for available memory limits exceeded
		if (intVal < 1024 || intVal > 102400)
		{
			// set Boolean result to false
			result = False;
		}
		break;

		// check for process cycles
	case CFG_PROC_CYCLES_CODE:

		//  check for process cycles limits exceeded
		if (intVal < 1 || intVal > 100)
		{
			//  set boolean result to False
			result = False;
		}
		break;

		//  check for I/O cycles
	case CFG_IO_CYCLES_CODE:
		// check for I/O cycles limits exceeded
		if (intVal < 1 || intVal > 1000)
		{
			//  set boolean result to False
			result = False;
		}
		break;

		//  check for Log to operation
	case CFG_LOG_TO_CODE:

		// create temporary string and set to lower set to lower case
		// function : getStringLength, malloc, setStrToLowerCase
		strLen = getStringLength(stringVal);
		tempStr = (char*)malloc(strLen + 1);
		setStrToLowerCase(tempStr, stringVal);

		// check for not finding one of the log to strings
		// function : compareString
		if (compareString(tempStr, "both") != STR_EQ
			&& compareString(tempStr, "monitor") != STR_EQ
			&& compareString(tempStr, "file") != STR_EQ)
		{
			//  set Boolean result to false
			result = False;
		}
		//  free temp string memory
	// function: free
		free(tempStr);
		break;
	}
	// return result of limits analysis
	return result;
}




