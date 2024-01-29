#include <stdio.h>
#include <stdlib.h> 
#include <math.h>       // pow()
#include <stdbool.h>    // bool data type
#include <string.h>     // strlen(), strrev()
#include <ctype.h>      // isdigit()
#include <windows.h>    // To change text color in console

#include "BigInt.c"     // BigInt Library
#include "NumberSystemWizards_v2.h" // Functions prototypes 

// Macros to reset &change text colors: (instead or writing it multiple times)
  // Initiate handle for console, must be in every function uses color changing feature.
  #define COLOR_INIT      HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#define COLOR_RESET     SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE);
#define COLOR_RED       SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
#define COLOR_GREEN     SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
#define COLOR_BLUE      SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE|FOREGROUND_GREEN);
#define COLOR_YELLOW    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN|FOREGROUND_RED);
#define COLOR_PURPLE    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE|FOREGROUND_RED);

/*Global Declaration*/
//Initialize the indexes of the system to be used in the code
char *systemIndexes[5] = {"", "Binary", "Octal", "Decimal", "Hexadecimal"};


//============================================================================
//! Welcome and Validation functions
//============================================================================


//A function to print a yellow smiley face and welcome the user with his name
void WelcomeScreen(){
    COLOR_INIT
    char userName[100];

    //Printing a Yellow smiley face.
    COLOR_YELLOW
    printf("\n                    *****************\n"
    "               ******               ******\n"
    "           ****                           ****\n"
    "        ****                                 ***\n"
    "      ***                                       ***\n"
    "     **           ""***               ***""           **\n"
    "   **           ""*******           *******""          ***\n"
    "  **            ""*******           *******""            **\n"
    " **             ""*******           *******""             **\n"
    " **               ""***               ***""               **\n"
    "**                                                     **\n"
    "**       *                                     *       **\n"
    "**      **                                     **      **\n"
    " **   ****                                     ****   **\n"
    " **      **                                   **      **\n"
    "  **       ***                             ***       **\n"
    "   ***       ****                       ****       ***\n"
    "     **         ******             ******         **\n"
    "      ***            ***************            ***\n"
    "        ****                                 ****\n"
    "           ****                           ****\n"
    "               ******               ******\n"
    "                    *****************\n\n");


    COLOR_RESET
    printf("OH! It is a pleasure to see you :D\nMay I know your name? -> : ");

    //Taking the user name from the user.
    gets(userName);
    COLOR_YELLOW
    //Printing a line depending on the username length to fit the message.
    printf("\n\n<");
    for(int i=0;i<50+strlen(userName);i++){
        printf("-");
    }
    printf(">\n");

    //Printing the Welcome message.
    printf("^ Welcome"); 
    COLOR_PURPLE printf(" %s ", userName); COLOR_YELLOW
    printf("To Our NumberSystem Converter Machine:) ^\n");
    printf("<");

    //Printing a line depending on the username length to fit the message.
    for(int i=0;i<50+strlen(userName);i++){
        printf("-");
    }
    printf(">\n\n");
    COLOR_RESET
}


//prints list and take the chosenSystem. and return the chosenSystem.
//Implemented here to free some space in main.
int beginningFunction( void ) {

    conversionList(); // Calling the conversion list only each time the program starts over
    int chosenSystem = getChosenSystem(); // Get and validate the chosen input system
    return chosenSystem;
}


//A function to print the options list.
void conversionList(){
    COLOR_INIT
    COLOR_YELLOW
    printf("\t\t--> Conversion Types List <--\n\n");
    COLOR_RESET
    printf(" 1- Binary Conversions.\n"
    " 2- Octal Conversions.\n"
    " 3- Decimal Conversions.\n"
    " 4- Hexadecimal Conversions.\n"
    " 5- Exit the program.\n");
    COLOR_BLUE
    printf("\n\tPlease select the required system from the previous list\n");
    COLOR_RESET
}

// To get the chosen system from the list using recursion
int getChosenSystem(){
    COLOR_INIT
        int chosenSystem = 0, 
        //The actual system the user chose
        numOfScannedVariables = 0, 
        //The number of scanned variables 
            //(return 0 if the variable wasn't scanned properly)
        numOfFlushes = 0; 
        //Check if there was additional unintended input 
            //(counts how many times a character has been removed from the input stream)

        COLOR_YELLOW printf("Enter here:  "); COLOR_RESET
        numOfScannedVariables = scanf("%d", &chosenSystem);
        numOfFlushes = flushBufferReturnCounter();
        if (validateChosenSystem(chosenSystem, numOfScannedVariables, numOfFlushes)){ 
            // Base case (the number is correct)
            COLOR_BLUE
            printf("\nIf you want to proceed press (1).\nIf you want to RETURN BACK to the List press (2).\n\n");
            COLOR_RESET
            return chosenSystem;
        }
        else return getChosenSystem(); // Recursive case (calls itself until the user enter a valid number)
}


/*Check if 
    * 1- The system that the user chose is actually valid i.e from [1 - 4] or 5 to exit the program
    * 2- If the user unintentionally entered a character, string, float, or any other type
    * return a boolean expression, where false indicates the input is valid and terminate the while loop
    * true indicates the input is not valid and continue the while loop and get the input again
    */
bool validateChosenSystem(int chosenSystem, int numOfScannedVariables, int numOfFlushes){

    COLOR_INIT
    if (numOfScannedVariables != 1 || numOfFlushes != 0){
        // If the user entered a string or any other datatype except the intended
        COLOR_RED
        printf("\aPlease enter a valid integer number!\n");
        COLOR_RESET
        return false;
    }
    else if (1 > chosenSystem || chosenSystem > 5){
        // If the user entered a number that is out of range [1-5]
        COLOR_RED
        printf("\aPlease enter a number from the list [1,2,3,4,5]\n");
        COLOR_RESET
        return false;
    }
    return true;
}


//Implemented here to free some space in main.
int getMakeSure() {
    COLOR_INIT
    int makeSure = 0;
    int numOfFlushes = 0;

    COLOR_YELLOW
    printf("Enter here: ");
    COLOR_RESET

    scanf("%d", &makeSure);
    numOfFlushes = flushBufferReturnCounter(); 

    if (checkMakeSure(makeSure, numOfFlushes)) {
        return getMakeSure();
    }
    else return makeSure;
}

//Implemented here to free some space in main.
bool checkMakeSure ( int makeSure, int numOfFlushes){
    COLOR_INIT // Initiate the color changing functions
    if((makeSure != 1 && makeSure != 2) || (numOfFlushes != 0)){
    COLOR_RED
    printf("\aInvalid input. Please try again and enter 1 Or 2!\n");
    COLOR_RESET
    return true;
    }
  return false;
}


// To define the size of the array instead of using a variable
#define sizeOfArrays 512 

// A function to scan, validate, convert, and print the converted number
void getInputNumberAndConvert(int chosenSystem){
    COLOR_INIT
    int numOfFlushes;
    COLOR_YELLOW
        printf("\t\tPlease Enter the %s number to be converted.\n", systemIndexes[chosenSystem]);
        COLOR_RESET
        char inputNumber[sizeOfArrays] = {0}; // Array to store user input
        // Get user input number and validate the input based on the system user chose
        do{
            COLOR_YELLOW printf("Enter here:  "); COLOR_RESET
            scanf("%511s", &inputNumber);  //! Change the number based on the size of the array
            numOfFlushes = flushBufferReturnCounter();
        }while(validateInputNumber(inputNumber, numOfFlushes, chosenSystem));
        
        // Initialize containers for the result of the conversions (very important to avoid garbage value)
        char binaryArray[sizeOfArrays] = {0};
        char octalArray[sizeOfArrays] = {0};
        char hexArrayU[sizeOfArrays] = {0};
        char hexArrayL[sizeOfArrays] = {0};
        int compareToZero = 1; // Initialize the compare variable to determine whether the input number is zero, positive or negative
        
        BigInt* decimalInput = BigInt_construct(0); // Initialize a number variable to store decimal conversions
        if (chosenSystem != 4){
            decimalInput =  BigInt_from_string(inputNumber); // Only convert user input number to BigInt if it's not hexadecimal
            compareToZero = BigInt_compare_int(decimalInput, 0); 
            // Result of comparing the user input by zero {to handle negatives}
                // The BigInt_compare_int() returns 0 if both equal, 1 if the first is bigger 
                    //(i.e positive) , and -1 if the latter is bigger (negative)
        } 
        else{
            // We can't do the previous algorithm with hexa as it contains non-digits character
            if (strtol(inputNumber, NULL, 16) == 0) compareToZero = 0; // Then check if it's zero
            else if (inputNumber[0] == '-') compareToZero = -1; // Check negativity
        }

    switch(chosenSystem){
        case 1:
            binaryConversions(inputNumber, octalArray, hexArrayU, hexArrayL, compareToZero);
            break;
        case 2:
            octalConversions(inputNumber, binaryArray, hexArrayU, hexArrayL, compareToZero);
            break;
        case 3:
            decimalConversions(decimalInput, binaryArray, octalArray, hexArrayU, hexArrayL, compareToZero);
            break;
        case 4:
            hexConversions(inputNumber, binaryArray, octalArray, compareToZero);
            break;
    }
}


/** Check The validity of the input number
 * Returns 0 in case of the validation is correct (to exit the do-while loop)
 * Returns 1 if the input is not valid
*/
bool validateInputNumber(char *inputNumberArray, int numOfFlushes, int chosenSystem){
    COLOR_INIT

    int arrayLength = strlen(inputNumberArray);
    if (numOfFlushes != 0){ // User entered multiple numbers separated by spaces
        COLOR_RED
        printf("\aPlease enter a valid %s number! Please try again:\n", systemIndexes[chosenSystem]);
        COLOR_RESET
        return true;
    }

    int indx = 0; // Index to iterate over the array
    switch (chosenSystem){
        case 1: // Check Binary system, only 0's and 1's
            for(indx; indx < arrayLength; ++indx){  
                char currentChar = inputNumberArray[indx]; // The current char in the iteration. 
                if (indx == 0 && currentChar == '-' && 1 < arrayLength) continue; // Check if there's a negative sign only in the start of the array
                                                                                        // and make sure the user didn't enter '-' sign only!
                if (currentChar != '0' && currentChar != '1'){
                    COLOR_RED
                    printf("\aPlease enter a valid %s number. \n\tBinary numbers consists only of 0's and 1's.\n", systemIndexes[chosenSystem]);
                    COLOR_RESET
                    return true;
                }
            }
            break;
        case 2: // Check Octal system, only numbers from 0 to 7
            for(indx; indx < arrayLength; ++indx){  
                char currentChar = inputNumberArray[indx];
                if (indx == 0 && currentChar == '-' && 1 < arrayLength) continue; // Check if there's a negative sign only in the start of the array
                                                                                        // and make sure the user didn't enter '-' sign only!
                if (currentChar < '0' || '7' < currentChar){
                    COLOR_RED
                    printf("\aPlease enter a valid %s number. \n\tOctal numbers' digits ranges from [0 - 7] inclusive.\n", systemIndexes[chosenSystem]);
                    COLOR_RESET
                    return true;
                }
            }
            break;
        case 3: // Check Decimal system, only digits
            for(indx; indx < arrayLength; ++indx){  
                char currentChar = inputNumberArray[indx];
                if (indx == 0 && currentChar == '-' && 1 < arrayLength) continue; // Check if there's a negative sign only in the start of the array
                                                                                        // and make sure the user didn't enter '-' sign only!
                if (!isdigit(currentChar)){
                    COLOR_RED
                    printf("\aPlease enter a valid %s number. \n\tDecimal numbers should not contain any non-digit characters.\n", systemIndexes[chosenSystem]);
                    COLOR_RESET
                    return true;
                }
            }
            break;
        case 4: 
        // Validate hexadecimal system {all characters must be either digits or range between [a-f] (inclusive, case doesn't matter) 
            //except the first character maybe (char)'-' if it's negative}
            for(indx; indx < arrayLength; ++indx){  
                char currentChar = inputNumberArray[indx]; // The current char in the iteration. 
                if (indx == 0 && currentChar == '-' && 1 < arrayLength) continue; // Check if there's a negative sign only in the start of the array
                                                                                        // and make sure the user didn't enter '-' sign only!
                if (currentChar > 'Z') currentChar -= ' '; // Convert the lower to upper case, for robust if condition
                if (!(isdigit(currentChar))){
                    if (currentChar > 'F' || currentChar < 'A'){
                        COLOR_RED
                        printf("\aPlease enter a valid %s number. \n\tHex digits ranges from [0 - 9] and [A - F] (inclusive, case doesn't matter).\n", systemIndexes[chosenSystem]);
                        COLOR_RESET
                        return true;
                    }
                }
            }
            break;
    }
    return false;
}


//============================================================================
//! Math and conversion functions
//============================================================================


/**A function that raises a BigInt to a power (long long), by repeatedly multiplying the BigInt by itself.
 * Accepts The BigInt and the exponent.
 * The function returns a BigInt, the result of base^exponent (^ donates power not XOR)
*/
BigInt* BigInt_pow(BigInt* base, long long int exponent){

    // Initialize a BigInt to the value 1 to begin the multiplication
    BigInt* multiplicationResult = BigInt_construct(1); 

    for (int i = 0; i<exponent; ++i){
        /*Repeatedly multiply the result by the base and store it in the result variable*/
        BigInt_multiply(multiplicationResult, base);
    }
    return multiplicationResult;
}


/** A function to convert an array of binary characters to decimal */
BigInt* binaryToDecimal(char* binaryArray, int compareToZero){

    BigInt* decimalBig = BigInt_construct(0);
    if (!compareToZero) return decimalBig; // Handle '0' input
    BigInt* base = BigInt_construct(2);
    int indx = strlen(binaryArray) - 1; long long power = 0;

    for (indx, power; indx >- 1; --indx, ++power){
        // On each iteration: get the current character, if '0' or '-' (negative sign), move to the previous character (--indx) and increase the power
        if (binaryArray[indx] == '0' || binaryArray[indx] == '-') continue;
        BigInt_add(decimalBig, BigInt_pow(base,power));
    }
    BigInt_free(base);
    if (compareToZero < 0) BigInt_multiply_int(decimalBig, -1); // If input is negative, multiply by -1
    return decimalBig;
}


/**A function to convert a BigInt decimal number to binary number
 * assign the first digit in the binary array to '-' sign only if the number is negative
*/
void decimalToBinary(BigInt* decimalNumber, char* binaryArray, int compareToZero){ 
    // Handle Input zero case
    if (!compareToZero){ // i.e exactly == zero
        binaryArray[0] = '0';
        return;
    }

    BigInt* divisor = BigInt_construct(2);
    BigInt* remainder = BigInt_construct(1);
    // Make a clone of the bigInt so when modifying it, it doesn't affect the original number
    BigInt* decimalNumberClone = BigInt_clone(decimalNumber, 512); // Maximum number of digits clones is 512 digit
    long long index = 0; 

    do{
        // Make the division, assign the quotient to decimalNumber, assign the remainder to the remainder variable {neat right?}
        BigInt_divide(decimalNumberClone, divisor, decimalNumberClone, remainder); 
        char rem[2] = {};   // Get the remainder in a string, important to use BigInt_to_string() function
        BigInt_to_string(remainder, rem, 2); //convert the remainder (as BigInt) to a string (to modify the binaryArray)        
        binaryArray[index] = rem[0];
        index++;
    }while (BigInt_compare_int(decimalNumberClone, 0) != 0);

    // It's very important to free up all BigInt number created, otherwise it's an elegant way to kill the RAM ^_^
    BigInt_free(decimalNumberClone);
    BigInt_free(divisor);
    BigInt_free(remainder);

    if (compareToZero < 0){ // Handle negative input
        // Make the last character a negative sign, remember! the string will be printed in reverse
        binaryArray[index] = '-'; 
    }
}


/** A function to convert an array of octal characters to decimal */
BigInt* octalToDecimal(char* octalArray, int compareToZero){

    BigInt* decimalBig = BigInt_construct(0); 
    if (!compareToZero) return decimalBig; // Handle '0' input
    BigInt* base = BigInt_construct(8);
    int indx = strlen(octalArray) - 1; long long power = 0; // Power of base 8

    for (indx, power; indx >- 1; --indx, ++power){
        // On each iteration: get the current character, if '0' or '-' (negative sign), move to the previous character (--indx) and increase the power
        if (octalArray[indx] == '0' || octalArray[indx] == '-') continue;
        BigInt* multiplicationOperand1 =  BigInt_pow(base,power); // The first operand in the multiplication process is set to the power result (8^sth)
        int multiplicationOperand2 = octalArray[indx]-'0'; // The second operand in the multiplication process is set to the current value in the octal number
        BigInt_multiply_int(multiplicationOperand1, multiplicationOperand2); 
        BigInt_add(decimalBig, multiplicationOperand1);
    }
    BigInt_free(base);
    if (compareToZero < 0) BigInt_multiply_int(decimalBig, -1); // If input is negative, multiply by -1
    return decimalBig;
}


/**A function to convert a BigInt decimal number to octal number
 * assign the first digit in the octal array to '-' sign only if the number is negative*/
void decimalToOctal(BigInt* decimalNumber, char *octalArray, int compareToZero){ 
    // Handle Input zero case
    if (!compareToZero){ // i.e exactly == zero
        octalArray[0] = '0';
        return;
    }

    BigInt* divisor = BigInt_construct(8);
    BigInt* remainder = BigInt_construct(1);
    // Make a clone of the bigInt so when modifying it, it doesn't affect the original number
    BigInt* decimalNumberClone = BigInt_clone(decimalNumber, 512); // Maximum number of digits clones is 512 digit
    long long index = 0;  // Keep track of the current index of the hexArray

    do{
        // Make the division, assign the quotient to decimalNumber, assign the remainder to the remainder variable {neat right?}
        BigInt_divide(decimalNumberClone, divisor, decimalNumberClone, remainder); 
        char rem[2] = {};   // Get the remainder in a string, important to be able to use BigInt_to_string() function
        BigInt_to_string(remainder, rem, 2); // Convert the remainder (as BigInt) to a string (to modify the octalArray)
        
        octalArray[index] = rem[0];
        index++;
    }while (BigInt_compare_int(decimalNumberClone, 0) != 0);

    // It's very important to free up all BigInt number created, otherwise it's an elegant way to kill the RAM ^_^
    BigInt_free(decimalNumberClone);
    BigInt_free(divisor);
    BigInt_free(remainder);

    if (compareToZero < 0){ // Handle negative input
        // Make the last character negative sign, remember! the string will be printed in reverse
        octalArray[index] = '-'; 
    }
}


/** A function to convert an array of hex characters to decimal */
BigInt* hexToDecimal(char* hexArray, int compareToZero){

    BigInt* decimalBig = BigInt_construct(0); // Initializing BigInt is very important
    if (!compareToZero) return decimalBig; // Handle '0' input
    BigInt* base = BigInt_construct(16);
    int indx = strlen(hexArray) - 1; long long power = 0; // Power of base 16

    for (indx, power; indx > -1; --indx, ++power){
        // On each iteration: get the current character, if '0' or '-' (negative sign), move to the previous character (--indx) and increase the power

        if (hexArray[indx] == '0' || hexArray[indx] == '-') continue;
        BigInt* weight =  BigInt_pow(base,power); // The first operand in the multiplication process is set to the power result (8^sth)
        int valueOfCharacter = -1; // Initialize the variable to a negative value for better debugging
            // To get the value of the current digit in the hexArray, we check if it's a number (return this value - '0' to convert from ASCII to int)
                // and if it's a character, convert it to an integer 
        if      ('0' <= hexArray[indx] && hexArray[indx] <= '9')  valueOfCharacter = hexArray[indx] - '0';
        else if ('a' <= hexArray[indx] && hexArray[indx] <= 'f')  valueOfCharacter = hexArray[indx] - 'a' + 10;
        else if ('A' <= hexArray[indx] && hexArray[indx] <= 'F')  valueOfCharacter = hexArray[indx] - 'A' + 10;        
        BigInt_multiply_int(weight, valueOfCharacter);    
        BigInt_add(decimalBig, weight);
    }
    BigInt_free(base);
    if (compareToZero < 0) BigInt_multiply_int(decimalBig, -1); // If input is negative, multiply by -1
    return decimalBig;
}


/**A function to convert a BigInt decimal number to hex number
 * Assign the first digit in the hex array to '-' sign only if the number is negative
 * Returns true if there's characters in the hex number to tell the printing function to print upper and lower case
 * If there're no characters return false, then only one version of the number will be printed*/
bool decimalToHex(BigInt* decimalNumber, char* hexArrayUpper, char* hexArrayLower, int compareToZero){ 
    
    bool haveCharacters = false; // If there's no characters in the hex numbers, print only the lowercase, as the uppercase would be identical
    // Handle Input zero case
    if (!compareToZero){ // i.e the input number exactly == zero
        hexArrayUpper[0] = '0';
        hexArrayLower[0] = '0';
        return false;
    }
    BigInt* divisor = BigInt_construct(16);
    BigInt* remainder = BigInt_construct(0);
    // Make a clone of the bigInt so when modifying it, it doesn't affect the original number
    BigInt* decimalNumberClone = BigInt_clone(decimalNumber, 512); // Maximum number of digits clones is 512 digit
    long long index = 0;  // Keep track of the current index of the hexArray

    do{
        // Make the division, assign the quotient to decimalNumber, assign the remainder to the remainder variable {neat right?}
        BigInt_divide(decimalNumberClone, divisor, decimalNumberClone, remainder); 
        char rem[2] = {}; // Get the remainder in a string, important to use BigInt_to_string() function
        BigInt_to_string(remainder, rem, 2); //convert the remainder (as BigInt) to a string (to modify the hexArray)
        int remInt = atoi(rem); // Convert the remainder to an integer
        if (remInt <= 9){
            hexArrayUpper[index] = rem[0]; // If the remainder is 9 at max, then it's kept as it's in the two hexArrays
            hexArrayLower[index] = rem[0];
        } 
        else{
            haveCharacters = true;
            // Getting the corresponding hex value of the remainder [A-F] for upper array
            hexArrayUpper[index] = remInt + 55; 
            // And store the same value but lower in the lower array
            hexArrayLower[index] = hexArrayUpper[index] + ' '; // Add space or 32 in ASCII to convert from upper to lower
        }         
        index++;
    }while (BigInt_compare_int(decimalNumberClone, 0) != 0);

    // It's very important to free up all BigInt number created, otherwise it's an elegant way to kill the RAM ^_^
    BigInt_free(decimalNumberClone);
    BigInt_free(divisor);
    BigInt_free(remainder);

    if (compareToZero < 0){ // Handle negative input
        // Make the last character negative sign, remember! the string will be printed in reverse
        hexArrayUpper[index] = '-'; 
        hexArrayLower[index] = '-';
    }
    return haveCharacters;
}


//============================================================================
//! Conversion functions
//============================================================================

// To convert and print the conversions if the user entered a binary number
void binaryConversions(char* inputNumber, char* octalArray, char* hexArrayU, char* hexArrayL, int compareToZero){
    COLOR_INIT
    BigInt* decimalInput = binaryToDecimal(inputNumber, compareToZero);
    decimalToOctal(decimalInput, octalArray, compareToZero);
    bool haveCharacters = decimalToHex(decimalInput, hexArrayU, hexArrayL, compareToZero);
    // Printing the other 3 systems: oct, dec, hex:
    COLOR_BLUE
    printf("\n-----------------------------------\n");
    COLOR_RESET
    printDecimal(decimalInput);
    printOctal(octalArray);
    printHex(hexArrayU, hexArrayL, haveCharacters);
}

// To convert and print the conversions if the user entered an octal number
void octalConversions(char* inputNumber, char* binaryArray, char* hexArrayU, char* hexArrayL, int compareToZero){
    COLOR_INIT
    BigInt* decimalInput = octalToDecimal(inputNumber, compareToZero);
    decimalToBinary(decimalInput, binaryArray, compareToZero);
    bool haveCharacters = decimalToHex(decimalInput, hexArrayU, hexArrayL, compareToZero);
    COLOR_BLUE
    printf("\n-----------------------------------\n");
    COLOR_RESET
    printBinary(binaryArray);
    printDecimal(decimalInput);
    printHex(hexArrayU, hexArrayL, haveCharacters);
}

// To convert and print the conversions if the user entered a decimal number
void decimalConversions(BigInt* inputNumber, char* binaryArray, char* octalArray, char* hexArrayU, char* hexArrayL, int compareToZero){
    COLOR_INIT
    decimalToBinary(inputNumber, binaryArray, compareToZero);
    decimalToOctal(inputNumber, octalArray, compareToZero);
    bool haveCharacters = decimalToHex(inputNumber, hexArrayU, hexArrayL, compareToZero);    
    COLOR_BLUE
    printf("\n-----------------------------------\n");
    COLOR_RESET
    printBinary(binaryArray);
    printOctal(octalArray);
    printHex(hexArrayU, hexArrayL, haveCharacters);
}

// To convert and print the conversions if the user entered a hexadecimal number
void hexConversions(char* inputNumber, char* binaryArray, char* octalArray, int compareToZero){
    COLOR_INIT
    BigInt* decimalInput = hexToDecimal(inputNumber, compareToZero);
    decimalToBinary(decimalInput, binaryArray, compareToZero);
    decimalToOctal(decimalInput, octalArray, compareToZero);
    COLOR_BLUE
    printf("\n-----------------------------------\n");
    COLOR_RESET
    printBinary(binaryArray);
    printOctal(octalArray);
    printDecimal(decimalInput);
}


//============================================================================
//! Printing functions
//============================================================================

// To print the binary number formatted
void printBinary(char* binaryArray){
    COLOR_INIT
    printf("\nBinary = "); 
    COLOR_YELLOW
    printf("%s\n", strrev(binaryArray)); 
    COLOR_BLUE
    printf("\n-----------------------------------\n");
    COLOR_RESET
}

// To print the octal number formatted
void printOctal(char* octalArray){
    COLOR_INIT
    printf("\nOctal = "); 
    COLOR_YELLOW
    printf("%s\n", strrev(octalArray)); 
    COLOR_BLUE
    printf("\n-----------------------------------\n");
    COLOR_RESET
}

// To print the decimal number formatted
void printDecimal(BigInt* decimalInput){
    COLOR_INIT
    printf("\nDecimal = "); 
    COLOR_YELLOW
    BigInt_print(decimalInput);
    COLOR_BLUE
    printf("\n\n-----------------------------------\n");
    COLOR_RESET
}

// To print the Hex number formatted, uppercase or upper and lowercase
void printHex(char* hexArrayU, char* hexArrayL, bool haveCharacters){
    COLOR_INIT
    if (!haveCharacters){
        printf("\nHexadecimal = "); 
        COLOR_YELLOW
        printf("%s\n", strrev(hexArrayU)); // Any of the hex arrays will work, it doesn't matter
    }
    else{
        printf("\nHexadecimal Upper = "); 
        COLOR_YELLOW
        printf("%s\n", strrev(hexArrayU));
        COLOR_RESET
        printf("----\nHexadecimal Lower = "); 
        COLOR_YELLOW
        printf("%s\n", strrev(hexArrayL));
    }
    COLOR_BLUE
    printf("\n-----------------------------------\n");
    COLOR_RESET
}


//============================================================================
//! Try again section
//============================================================================


//Implemented here to free some space in main.
char optionList (int chosenSystem){
    char response = 0;  // To store user choice whether to leave the program or to try again.
     printf("\nIf you want to Return Back to the list , enter (R/r)."
            "\nIf you want to try another %s number , enter (Y/y)"
            "\nIf you want to end the program, enter (N/n).\n", systemIndexes[chosenSystem]);
        response = getCharResponse(); // Get the response of the previous question and validates it.
    return response;
}

/*A function to get the input character from the user (y,n,r). 
    Implemented here to free some space in main*/
char getCharResponse(){
    COLOR_INIT
    char response = 0; // To store the response of the user about leaving the program
    int numOfFlushes = 0;
    do {
        COLOR_YELLOW printf("Enter here:  "); COLOR_RESET
        scanf(" %c", &response);
        numOfFlushes = flushBufferReturnCounter(); // Read the buffer, if not zero, the user entered more than one character.
    } while (checkResponse(numOfFlushes, response));
    return response;
}

// Check the response of optionList() function
bool checkResponse(int numOfFlushes, char response){
    COLOR_INIT // Initiate the color changing 
    if (((response != 'r' && response != 'R') && 
         (response != 'y' && response != 'Y') && 
         (response != 'n' && response != 'N')) ||   
         numOfFlushes != 0){
        COLOR_RED
        printf("\aInvalid input. Please try again and enter R/r OR Y/y OR N/n.\n");
        COLOR_RESET
        return true;
    }
    return false;
}

/*A function to print the closing screen.*/
void displayClosingMessage() {
    COLOR_INIT
    COLOR_GREEN
    printf("\n\t\tMade By: ^NumberSystem Wizards^\n\n"
            "Master of welcomes and creative formatter: "); COLOR_RED printf("Abdelrahman Nader\n"); COLOR_GREEN
            printf("Troubleshoot wizard: ");                COLOR_RED printf("Mohannad Elsayed \n"); COLOR_GREEN
            printf("Pioneer of Transformation: ");          COLOR_RED printf("Hania Hisham\n"); COLOR_GREEN
            printf("Conversion Engineer: ");                COLOR_RED printf("Nada Ahmed\n"); COLOR_GREEN
            printf("Transformation Specialist: ");          COLOR_RED printf("Hannah Ramadan\n"); COLOR_GREEN
            printf("Farewell Formatter: ");                 COLOR_RED printf("Ganaa Attia\n"); COLOR_GREEN
            printf("Closing Scene Stylist: ");              COLOR_RED printf("Yara Hossam\n\n"); COLOR_GREEN
            printf("\t\t\t Bye Bye!\n");
    COLOR_RESET
}


//Removes unwanted characters from the input stream, return true if there was characters in the buffer, false if not
bool flushBufferReturnCounter(){
    char dummyChar; // Create a dummy character to consume the buffer 
    int numOfFlushes = false; // Count how many characters consumed
    // Create a loop to repeatedly consume the characters in input stream buffer
        // exit the loop when newline or endOfLine is encountered
    while((dummyChar = getchar()) != '\n' && dummyChar != EOF){
        // If the flushing process encountered some characters which were not white space
        if (dummyChar != ' ') numOfFlushes = true; 
    }
    return numOfFlushes;
}