#ifndef STRING_UTILS_H
#define STRING_UTILS_H

// Header files
#include <stdio.h> // for malloc
#include <stdlib.h> // for file operations

//GLOBAL CONSTANTS -maybe used in other files
typedef enum { False, True } Boolean;

//Config data structure
typedef struct ConfigDataType
{
	double version;
	char metaDataFileName[100];
	int cpuSchedCode;
	int quantumCycles;
	int memAvailable;
	int procCycleRate;
	int ioCycleRate;
	int logToCode;
	char logToFileName[100];
} ConfigDataType;

typedef struct OpCodeType
{
	int pid;
	char command[5];
	char inOutArg[5];
	char strArg1[15];
	int intArg2;
	int intArg3;

	double opEndTime;
	struct OpCodeType* nextNode;
}OpCodeType;

typedef enum 
{
	NO_ERR,
	INCOMPLETE_FILE_ERR,
	INPUT_BUFFER_OVERRUN_ERR

} StringManipCode;

extern const char NULL_CHAR;
extern const char SPACE;
extern const char COLON;
extern const char COMMA;
extern const char SEMICOLON;
extern const char PERIOD;
extern const char LEFT_PAREN;
extern const char RIGHT_PAREN;
extern const int SMALL_STR_LEN;
extern const int STD_STR_LEN;
extern const int MAX_STR_LEN;
extern const int STR_EQ;
extern const int SUBSTRING_NOT_FOUND;
extern const Boolean IGNORE_LEADING_WS;
extern const Boolean ACCEPt_LEADING_WS;




// Function prototypes
int getStringLength(char* str);
void copyString(char* destination, char* source);
void concatenateString(char* destination, char* source);
int compareString( char* oneStr,  char* otherStr);
void getSubString(char* destStr, char* sourceStr, int startIndex, int endIndex);
int findSubString(char* testStr, char* searchSubStr);
void setStrToLowerCase(char* destStr, char* sourceStr);
char setCharToLowerCase(char inputChar);

int getLineTo(FILE* filePtr, int bufferSize, char stopChar, char* buffer,
	Boolean omitLeadingWhiteSpace, Boolean stopAtNonPrintable);
Boolean isEndOfFile(FILE* filePtr);

#endif  // STRING_UTILS_H
