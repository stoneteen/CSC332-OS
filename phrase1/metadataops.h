#ifndef META_DATA_ACCESS_H
#define META_DATA_ACCESS_H
// header files
#include "StringUtils.h"
#include <stdio.h>
// Constants.
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

extern const int BAD_ARG_VAL; 

// Functions prototypes
OpCodeType* addNode(OpCodeType* localPtr, OpCodeType* newNode);
int calcOptime(ConfigDataType* configPtr, OpCodeType* opCode);
OpCodeType* clearMetaDataList(OpCodeType* localPtr);
void copyOpcode(OpCodeType* destination, OpCodeType* source);
OpCodeType* createInitOpCode();
void displayMetaData(OpCodeType * localPtr);
int getCommand(char* cmd, char* inputstr, int index); 
int getOpCommand(FILE* filePtr, OpCodeType* inData);
Boolean getMetaData(char* fileName, OpCodeType** opCodeDataHead, char* endStateMsg);
int getNumberArg(int* number, char* inputStr, int index); 
int getStringArg(char* strArg, char* inputStr, int index); 
Boolean isDigit(char testChar);
Boolean verifyFirstStringArg(char* strArg); 
Boolean verifyValidCommand(char* testCmd); 
int updateEndCount(int count, char* opString); 
int updateStartCount(int count, char* opString);



#endif // META_DATA_ACCESS_H
