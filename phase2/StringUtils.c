// header files
#include "StringUtils.h"

//constant declaration
const char NULL_CHAR = '\0';
const char SPACE = ' ';
const char COLON = ':';
const char COMMA = ',';
const char SEMICOLON = ';';
const char PERIOD = '.';
const char LEFT_PAREN = '(';
const char RIGHT_PAREN = ')';
const int SMALL_STR_LEN = 30;
const int STD_STR_LEN = 60;
const int MAX_STR_LEN = 200;
const int STR_EQ = 0;
const int SUBSTRING_NOT_FOUND = -1001;
const Boolean IGNORE_LEADING_WS = True;
const Boolean ACCEPt_LEADING_WS = False;

/*
 Name: getStringLength
 Process: find the length of a string
          by counting characters up to the NULL_CHAR character
 Function Iutput/Parameters: c-style string (char *)
 Function Output/Parameters: none
 Function Output/Returned: length of string
 Device Input/Keyboard: none
 Device Output/Monitor: none
 Dependencies: none
 */
int getStringLength(char* str)
{
    // initialize function/variables
    int index = 0;

    // loop until null character found
    while (str[index] != NULL_CHAR)
    {
        // update index
        index++;
    }
    // end loop

 // return index/length
    return index;
}

/*
 Name: copyString
 Process: copies one string into another,
          overwriting data in the destination string
 Function Iutput/Parameters: c-style source string (char *)
 Function Output/Parameters: c-style destination sring (char *)
 Function Output/Returned: none
 Device Input/Keyboard: none
 Device Output/Monitor: none
 Dependencies: getStringLength
 */
void copyString(char* destination, char* source)
{
    // initialize function/variables
    int index = 0;

    //loop until null character is found in source string
    //loop limited to MAX_STR_LEN
    while (source[index] != NULL_CHAR && index < MAX_STR_LEN)
    {
        // assign character to end of destination string
        destination[index] = source[index];

        // update index
        index++;

        // set temporart end of destination string
        destination[index] = NULL_CHAR;
    }
    // end loop
}

/*
 Name: concatenateString
 Process: appends one string onto another
 Function Iutput/Parameters: c-style source strings (char *)
 Function Output/Parameters: c-style destination string (char *)
 Function Output/Returned: none
 Device Input/Keyboard: none
 Device Output/Monitor: none
 Dependencies: getStringLength
 */
void concatenateString(char* destination, const char* source)
{
    // initialize function/variables

       // set destination index
          // function: getStringLength
    int destIndex = getStringLength(destination);

    // set other variables
    int sourceIndex = 0;

    // loop to end of source string
    while (source[sourceIndex] != NULL_CHAR && destIndex < MAX_STR_LEN)
    {
        // assign characters to end of destination string
        destination[destIndex] = source[sourceIndex];

        // update indices
        destIndex++; sourceIndex++;

        // set temporary end of destination string
        destination[destIndex] = NULL_CHAR;
    }
    //end loop
}




/*
 Name: compareString
 Process: compares two strings with the following results
          if left string less than right string, returns less than zero
          if left string greater than right string, returns grater than zero
          if left string equals right/string, returns zero
          - equals test includes length
 Function Iutput/Parameters: c-style left and right strings (char *)
 Function Output/Parameters: none
 Function Output/Returned: result as specified (int)
 Device Input/Keyboard: none
 Device Output/Monitor: none
 Dependencies: getStringLength
 */
int compareString(char *oneStr, char *otherStr)
  {
   // initialize function/variables
   int difference, index = 0;

   // loop to end of shortest string
   //  with overrun protection
   while( oneStr[index] != NULL_CHAR
             && otherStr[index] != NULL_CHAR
             && index < MAX_STR_LEN)
      {
       // get difference in characters
       difference = oneStr[index] - otherStr[index];

       // check for  difference between characters
       if(difference != 0)
          {
           // return  difference
           return difference;
          }

       // increment index
       index++;
      }
      //end loop

      //return difference in lengths, if any
      // function: getStringLength
   return getStringLength(oneStr) - getStringLength(otherStr);
  }

/*
Function Name: getSubString
Process: captures sub string within larger string
         between two inclusive indices
         returns empty string if either index is out of range,
         assumes enough memory in destination string
Function Input/Parameters: c-style source string (char *)£¬
                           start and end indices (int)
Function Output/Parameters: c-style destination string (char *)
Function Output/Rettrned: none
Device Input/Keyboard: noneDevice output/Monitor: none
Dependencies: getStringLength, malloc,copyString,free
*/
void getSubString(char* destStr, char* sourceStr,
    int startIndex, int endIndex)
{
    // initialize function / variables
        // set length of source string
        // functionL getStringLength
    int sourceStrLen = getStringLength(sourceStr);

    // initializs destination index to zero
    int destIndex = 0;

    // initialize source index to start index(parameter)
    int sourceIndex = startIndex;

    // create pointer to temp string
    char* tempSourceStr;

    // check for indices within limits
    if (startIndex >= 0 && endIndex >= startIndex && endIndex < sourceStrLen)
    {
        // create temporary string
        // function : malloc, copyString
        tempSourceStr = (char*)malloc(sourceStrLen + 1);
        copyString(tempSourceStr, sourceStr);


        // loop across requested substring(indices)
        while (sourceIndex <= endIndex && destIndex < MAX_STR_LEN)
        {
            // assign source character to destination element
            destStr[destIndex] = tempSourceStr[sourceIndex];

            // increment indices
            destIndex++; sourceIndex++;

            // set temporary end of destination string
            destStr[destIndex] = NULL_CHAR;
        }

        // release memoryfor temporary stringll function : free
        free(tempSourceStr);
    }
    else
    {
        //create empty string with null character
        destStr[0] = NULL_CHAR;
    }
}


int findSubString(char* testStr, char* searchSubStr)
{
    //initialize function/variables

    //initialize test string length
        //function: getstringLength
    int testStrLen = getStringLength(testStr);

    //initialize master index - location of sub string start point
    int masterIndex = 0;
    //initialize other variables
    int searchIndex, internalIndex;

    //loop across test string
    while (masterIndex < testStrLen)
    {
        //set internal loop index to current test string index
        internalIndex = masterIndex;

        //set internal search index to zero
        searchIndex = 0;

        //loop to end of test string
            //  while test string/sub string characters are the same
        while (internalIndex <= testStrLen
            && testStr[internalIndex] == searchSubStr[searchIndex])
        {
            //increment test string, substring indices
            internalIndex++, searchIndex++;
            //check for end of subsrring(search completed)
            if (searchSubStr[searchIndex] == NULL_CHAR)
            {
                //return beginning location of sub string
                return masterIndex;
            }
        }
        //end internal comparison loop

    //increment current beginning locationindex
        masterIndex++;
    }
    //end loop across test string

    //assumetest have failed at this point, return SUBSTRING_NOT_FOUND;
    return SUBSTRING_NOT_FOUND;
 }

/*
Name : setStrToLowerCase
Process: iterates through string, sets ant upper case letter
to lower case; no effect on other letters
Function Iutput/Parameters: c-style source string (char *)
Function output/Paqameters: c-style destination string (char *)Function output/Returned: none
Device Input/Keyboard: noneDevice output/Monitor: noneDependencies: toLowerCase
*/
void setStrToLowerCase(char* destStr, char* sourceStr)
{
    // initialize function / variables
        // get source string length
        // function : getStringLength
    int strLen = getStringLength(sourceStr);

    // create temporary string pointer
    char* tempStr = (char*)malloc(strLen + 1);

    // create other variables
    int index = 0;

    // copy source string in case of aliasing
        // function : malloc, copyString
    copyString(tempStr, sourceStr);

    // loop acorss source string
    while (tempStr[index] != NULL_CHAR && index < MAX_STR_LEN)
    {
        // set individual character to lower case as needed, 
        // assign to destination string
        destStr[index] = setCharToLowerCase(tempStr[index]);

        // update index
        index++;

        //  set temporary end of destination string
        destStr[index] = NULL_CHAR;
    }

    //  release memory used for temp string
        // function : free
    free(tempStr);
}

/*
Function Name: toLowerCase
Process: if character is upper case,sets it to lower case;
otherwise returns character unchanged
Function Input/Parameters: test character (char)Function output/Parameters:none
Function output/Returned: character to set to lower case£¬if appropriateDevice Input/Keyboard: none
Device output/Monitor: noneDependencies: none
*/
char setCharToLowerCase(char inputChar)
{
    // check for upper case letter
    if (testChar >= 'A' && testChar <= 'Z')
    {
        // return lower case letter
        testChar = testChar - 'A' + 'a';
    }

    // otherwise, assume no upper case letter, 
    // return character unchaged
    return testChar;
}

int getLineTo(FILE* filePtr, int bufferSize, char stopChar, char* buffer,
    Boolean omitLeadingWhiteSpace, Boolean stopAtNonPrintable)
{
    //initialize function / variables
    int charAsInt, charIndex = 0;
    int statusReturn = NO_ERR;
    Boolean bufferSizeAvailable = True;
    Boolean nonPrintableFound = False;

    // get first character(as integer)
    // function : fgetc
    charAsInt = fgetc(filePtr);

    // consume leading white space, if flagged
    while (omitLeadingWhiteSpace == True
        && charAsInt != (int)stopChar
        && charIndex < bufferSize
        && charAsInt <= (int)SPACE)
    {
        // get next character(as integer)
            // function : fgetc
        charAsInt = fgetc(filePtr);
    }

    // check for stop at non printable if needed
    if (stopAtNonPrintable == True && charAsInt < (int)SPACE)
    {
        // set non printable flag to true
        nonPrintableFound = True;
    }

    // capture string
    // loop while character is not stop characterand buffer size available
    while (charAsInt != (int)stopChar
        && nonPrintableFound == False && bufferSizeAvailable == True)
    {
        // check for input failure
            // function : isEndOfFile
        if (isEndOfFile(filePtr) == True)
        {
            // return incomplete file error
            return INCOMPLETE_FILE_ERR;
        }

        // check for usable(printable) character
        if (charAsInt >= (int)SPACE)
        {
            // assign input character to buffer string
            buffer[charIndex] = (char)charAsInt;

            // increment index
            charIndex++;
        }

        // set next buffer element to null character
        buffer[charIndex] = NULL_CHAR;

        // check for not at end of buffer size
        if (charIndex < bufferSize - 1)
        {
            // get a new character
            charAsInt = fgetc(filePtr);

            // check for stop at non printable if needed
            if (stopAtNonPrintable == True && charAsInt < (int)SPACE)
            {
                // set non printable flag to true
                nonPrintableFound = True;
            }
        }

        // otherwise, assume end of buffer size
        else
        {
            // set buffer size Boolean to false to end loop
            bufferSizeAvailable = False;

            // set status return to buffer overrun
            statusReturn = INPUT_BUFFER_OVERRUN_ERR;
        }
    }
    // return operation status
    return statusReturn;
}




Boolean isEndOfFile(FILE* filePtr)
{
    // check for feof end of file response
    if(feof(filePtr) != 0)
    {
        // return true
        return True;
    }

    // return false
    return False; 
}



