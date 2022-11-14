#ifndef META_DATA_ACCESS_H
#define META_DATA_ACCESS_H
// header files
#include "StringUtils.h"
#include <stdio.h>
// Constants.
extern const int BAD_ARG_VAL; 

// op code messages, 
typedef enum {
	MD_FILE_ACCESS_ERR = 3,
	MD_CORRUPT_DESCRIPTOR_ERR,
	OPCMD_ACCESS_ERR,
	CORRUPT_OPCMD_ERR,
	CORRUPT_OPCMD_ARG_ERR,
	UNBALANCED_START_END_ERR,
	COMPLETE_OPCMD_FOUND_MSG,
	LAST_OPCMD_FOUND_MSG
}OpCodeMessages;


// Functions prototypes
OpCodeType* addNode(OpCodeType* localPtr, OpCodeType* newNode);
OpCodeType* clearMetaDataList(OpCodeType* localPtr);
void displayMetaData(OpCodeType* localPtr);
int getCommand(char* cmd, char* inputstr, int index);
int getOpCommand(FILE* filePtr, OpCodeType* inData);
int getNumberArg(int* number, char* inputStr, int index);
int getStringArg(char* strArg, char* inputStr, int index);
int updateEndCount(int count, char* opString);
int updateStartCount(int count, char* opString);
Boolean isDigit(char testChar);
Boolean verifyFirstStringArg(char* strArg); 
Boolean verifyValidCommand(char* testCmd); 
Boolean getMetaData(char* fileName, OpCodeType** opCodeDataHead, char* endStateMsg); 



#endif // META_DATA_ACCESS_H
