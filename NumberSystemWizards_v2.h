#include <stdio.h>
#include <stdlib.h> 
#include <math.h>       // pow()
#include <stdbool.h>    // bool data type
#include <string.h>     // strlen(), strrev()
#include <ctype.h>      // isdigit()
#include <windows.h>    // To change text color in console

#include "BigInt.c"     // BigInt Library


// Macros to reset &change text colors: (instead or writing it multiple times)
  // Initiate handle for console, must be in every function uses color changing feature.
  #define COLOR_INIT      HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#define COLOR_RESET     SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE);
#define COLOR_RED       SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
#define COLOR_GREEN     SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
#define COLOR_BLUE      SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE|FOREGROUND_GREEN);
#define COLOR_YELLOW    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN|FOREGROUND_RED);
#define COLOR_PURPLE    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE|FOREGROUND_RED);


/*Global Decleration*/
//Initialize the indexes of the system to be used in the code
char *systemIndexes[5] = {"", "Binary", "Octal", "Decimal", "Hexadecimal"};


//A function to print a yellow smiley face and welcome the user with his name
void initWelcomeScreen();


//A function to print the options list.
void conversionList();


// To get the chosen system from the list
int getChosenSystem();


/*Check if 
    * 1- The system that the user chose is actually valid i.e from [1 - 4] or 5 to exit the program
    * 2- If the user unintentionally entered a character, string, float, or any other type
    * return a boolean expression (0,1) where 0 indicates the input is valid and terminate the while loop
    * 1 indecates the input is not valid and continue the while loop and get the input again
    */
bool validateChosenSystem(int chosenSystem, int numOfScannedVariables, int numOfFlushes);


// A function to scan, validate, convert, and print the converted number
void getInputNumberAndConvert(int chosenSystem);


/** Check The validity of the input number
 * Returns 0 in case of the validation is correct (to exit the do-while loop)
 * Returns 1 if the input is not valid
*/
bool validateInputNumber(char *inputNumberArray, int numOfFlushes, int chosenSystem);


//============================================================================
//! Math and conversion functions
//============================================================================


/**A function that raises a BigInt to a power (long long), by repeatedly multiplying the BigInt by itself.
 * Accepts The BigInt and the exponenet.
 * The function returns a BigInt, the result of base^exponenet (^ donates power not XOR)
*/
BigInt* BigInt_pow(BigInt* base, long long int exponent);


/** A function to convert an array of binary characters to decimal */
BigInt* binaryToDecimal(char* binaryArray, int compareToZero);


/**A function to convert a BigInt decimal number to binary number
 * assign the first digit in the binary array to '-' sign only if the number is negative
*/
void decimalToBinary(BigInt* decimalNumber, char *binaryArray, int compareToZero);


/** A function to convert an array of octal characters to decimal */
BigInt* octalToDecimal(char* octalArray, int compareToZero);


/**A function to convert a BigInt decimal number to octal number
 * assign the first digit in the octal array to '-' sign only if the number is negative*/
void decimalToOctal(BigInt* decimalNumber, char *octalArray, int compareToZero);


/** A function to convert an array of hex characters to decimal */
BigInt* hexToDecimal(char* hexArray, int compareToZero);


/**A function to convert a BigInt decimal number to hex number
 * Assign the first digit in the hex array to '-' sign only if the number is negative
 * Returns true if there was characters in the final hex number, false if there wasn't*/
bool decimalToHex(BigInt* decimalNumber, char* hexArrayUpper, char* hexArrayLower, int compareToZero);


//============================================================================
//! Printing functions
//============================================================================


// To convert and print the conversions if the user entered a binary number
void binaryConversions(char* inputNumber, char* octalArray, char* hexArrayU, char* hexArrayL, int compareToZero);

// To convert and print the conversions if the user entered an octal number
void octalConversions(char* inputNumber, char* binaryArray, char* hexArrayU, char* hexArrayL, int compareToZero);

// To convert and print the conversions if the user entered a decimal number
void decimalConversions(BigInt* inputNumber, char* binaryArray, char* octalArray, char* hexArrayU, char* hexArrayL, int compareToZero);

// To convert and print the conversions if the user entered a hexadecimal number
void hexConversions(char* inputNumber, char* binaryArray, char* octalArray, int compareToZero);


//============================================================================
//! Try again section
//============================================================================



/*A function to get the input character from the user (y,n). 
    Implemented here to free some space in main*/
char getCharResponse();


// Check the response of tryAgain() function
bool checkResponse(int numOfFlushes, char response);


/*A function to determine whether to exit the program or to start again based on user choice*/
bool tryAgain(char response);


/*A function to print the closing screen.*/
void displayClosingMessage();


//Removes unwanted characters from the input stream, return 1 if there was characters in the buffer, 0 if not
bool flushBufferReturnCounter();