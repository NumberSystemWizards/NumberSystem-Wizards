#include <stdio.h>
#include <stdlib.h> 
#include <math.h>       // pow()
#include <stdbool.h>    // bool data type
#include <string.h>     // strlen(), strrev()
#include <conio.h>      
#include <ctype.h>      // isdigit()
#include <windows.h>    // To change text color in console

#include "BigInt.c"     // BigInt Library


// Macros to reset &change text colors: (instead or writing it multiple times)
  // Initiate handle for console, must be in every function uses color changing feature.
  #define COLOR_INIT      HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#define COLOR_RESET     SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE);
#define COLOR_RED       SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
#define COLOR_GREEN     SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
#define COLOR_BLUE      SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
#define COLOR_YELLOW    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN|FOREGROUND_RED);
#define COLOR_PURPLE    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE|FOREGROUND_RED);

/*Global Decleration*/
//Initialize the indexes of the system to be used in the code
char *systemIndexes[5] = {"", "Binary", "Octal", "Decimal", "Hexadecimal"}; 
int maxInputPerSystem[5] = {0, 18, 17, 8, 7}; // Restrict the maximum length of user input to avoid over flow.

void initWelcomeScreen(){
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
    //scanf("%s",&userName);
    gets(userName);
    COLOR_YELLOW
    //Printing a line depending on the username length to fit the messege.
    printf("\n\n<");
    for(int i=0;i<50+strlen(userName);i++){
        printf("-");
    }
    printf(">\n");


    //Printing the Welcome messege.
    printf("^ Welcome"); 
    COLOR_PURPLE printf(" %s ", userName); COLOR_YELLOW
    printf("To Our NumberSystem Converter Machine:) ^\n");
    printf("<");

    //Printing a line depending on the username length to fit the messege.
    for(int i=0;i<50+strlen(userName);i++){
        printf("-");
    }
    printf(">\n\n");
    COLOR_RESET
}

void conversionList(){
    /*A function to print the options list.*/
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


int flushBufferReturnCounter(){
    /* Removes unwanted characters from the input stream, return the number of flushed characters */
    char dummyChar; // Create a dummy character to consume the buffer 
    int numOfCharactersConsumed=0; // Count how many characters consumed
    // Create a loop to repeatedly exit the loop when newline or endOfLine is encountered
    while((dummyChar = getchar()) != '\n' && dummyChar != EOF){
        if (dummyChar != ' ') numOfCharactersConsumed++; 
    }
    return numOfCharactersConsumed;
}


bool checkSystemIndex(int chosenSystem, int numOfScannedVariables, int numOfFlushes){
    /*Check if 
     * 1- The System index the user chose is actually valid i.e from [1 - 4]
     * 2- If the user unintentionally entered a character, string, float, or any other type
     * return a boolean expression (0,1) where 0 indicates the input is valid and terminate the while loop
     * 1 indecates the input is not valid and continue the while loop and get the input again
     */

    COLOR_INIT
    if (numOfScannedVariables != 1 || numOfFlushes != 0){
        // If the user entered a string or any other datatype except the intended
        COLOR_RED
        printf("\aPlease enter a valid integer number!\n");
        COLOR_RESET
        return true;
    }
    else if (1 > chosenSystem || chosenSystem > 5){
        // If the user entered a number that is out of range [1-5]
        COLOR_RED
        printf("\aPlease enter a number from the list [1,2,3,4,5]\n");
        COLOR_RESET
        return true;
    }
    return false;
}

bool validateSystem(char *inputNumberArray, int numOfFlushes, int chosenSystem){
    /*Check the validity of the entered Hex number
     * first it checks if the character is digit, if not, it checks if it's {a,b,c,d,e,f}.
     * returns a boolean expression (0,1)where 0 indicates the input is valid and terminate the while loop
     * and 1 indecates the input is not valid and continue the while loop and get the input again.
     * */
    COLOR_INIT

    int arrayLength = strlen(inputNumberArray);

    if (numOfFlushes != 0){ // User entered spaces in the number which wasn't scaned by "scanf()"
        COLOR_RED
        printf("\aPlease enter a valid %s number! Try again with no spaces:\n", systemIndexes[chosenSystem]);
        COLOR_RESET
        return true;
    }

    int indx = 0; // Index to iterate the array
    switch (chosenSystem){
        case 1: 
        // Validate binary system {all characters must be either '0' or '1' except the first character maybe the sign '-' if it's negative}
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
        case 2: 
        // Validate octal system {all characters must range between [0-7] inclusive except the first character maybe the sign '-' if it's negative}
            for(indx; indx < arrayLength; ++indx){  
                char currentChar = inputNumberArray[indx]; // The current char in the iteration. 
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
        case 3: 
        // Validate decimal system {all characters must be digits except the first character maybe the sign '-' if it's negative}
            for(indx; indx < arrayLength; ++indx){  
                char currentChar = inputNumberArray[indx]; // The current char in the iteration. 
                if (indx == 0 && currentChar == '-' && 1 < arrayLength) continue; // Check if there's a negative sign only in the start of the array
                                                                                        // and make sure the user didn't enter '-' sign only!
                if (!isdigit(currentChar)){
                    COLOR_RED
                    printf("\aPlease enter a valid %s number. \n\tDecimal numbers should not contain any non-digits characters.\n", systemIndexes[chosenSystem]);
                    COLOR_RESET
                    return true;
                }
            }
            break;
        case 4: 
        // Validate hexadecimal system {all characters must be digits or range between [a-f] (inclusive, case doesn't matter) 
            //except the first character maybe the sign '-' if it's negative}
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


//! power function
BigInt* BigInt_pow(BigInt* base, long long int exponent){
    /**A function that raises a BigInt to a power (long long), by repeatedly multiplying the BigInt by itself.
     * Accepts The BigInt and the exponenet.
     * It's crucial to free up the memory allocated to the BigInt in this function.
     * The function returns a BigInt, the result of base^exponenet (^ donates power not XOR)
    */

    // Initialize a BigInt to the value 1 to begint the multiplication
    BigInt* multiplicationResult = BigInt_construct(1); 

    for (int i = 0; i<exponent; ++i){
        /*Repeatidly multiply the result by the base and store it in the result variable*/
        BigInt_multiply(multiplicationResult, base);
    }
    return multiplicationResult;
    BigInt_free(multiplicationResult); // Have no effect ?
}


//! ****************************************************************************************************Binary
BigInt* binaryToDecimal(char* binaryArray, int signComparer){
    /** A function to convert an array of binary characters to decimal */

    BigInt* decimalBig = BigInt_construct(0); // Initializing BigInt is very important
    if (!signComparer) return decimalBig; // Handle '0' input

    BigInt* exp = BigInt_construct(2);

    int indx = strlen(binaryArray) - 1; long long p = 0;

    for (indx, p; indx >- 1; --indx, ++p){
        // On each iteration: get the current character, if '0' or '-' (negative sign), move to the previous character (--indx) and increase the power
        if (binaryArray[indx] == '0' || binaryArray[indx] == '-') continue;
        BigInt_add(decimalBig, BigInt_pow(exp,p));
    }
    BigInt_free(exp);
    if (signComparer < 0) BigInt_multiply_int(decimalBig, -1); // If input is negative, multiply by -1
    return decimalBig;
}



void decimalToBinary(BigInt* decimalNumber, char *binaryArrayReturn, int signComparer){ 
    /**A function to convert a BigInt decimal number to binary number
     * assign the first digit in the binary array to '-' sign only if the number is negative
    */
    
    // Handle Input zero case
    if (!signComparer){ // i.e exactly == zero
        binaryArrayReturn[0] = '0';
        return;
    }

    BigInt* divisor = BigInt_construct(2);
    BigInt* remainder = BigInt_construct(1);
    // Make a clone of the bigInt so when modifying it, it doesn't affect other conversions
    BigInt* decimalNumberClone = BigInt_clone(decimalNumber, 512); 
    long long index = 0;  // Keep track of the current index of the hexArray

    do{
        // Make the division, assign the quotient in decimalNumber, assign the remainder to the remainder variable {neat right?}
        BigInt_divide(decimalNumberClone, divisor, decimalNumberClone, remainder); 

        char rem[2] = {}; // Get the remainder in a string, important to use BigInt_to_string() function

        BigInt_to_string(remainder, rem, 2); //convert the remainder (as BigInt) to a string (to modify the binaryArray)
        
        binaryArrayReturn[index] = rem[0];
        index++;
    }while (BigInt_compare_int(decimalNumberClone, 0) != 0);

    // It's very important to freeup all BigInt number created, otherwise it's an elegant way to kill the RAM ^_^
    BigInt_free(decimalNumberClone);
    BigInt_free(divisor);
    BigInt_free(remainder);

    if (signComparer < 0){ // Handle negative input
        // Make the last character negative sign, remember! the string will be printed in reverse
        binaryArrayReturn[index] = '-'; 
    }
}





//! **************************************************************************************************Octa
BigInt* octalToDecimal(char* octal, int signComparer){
    /** A function to convert an array of octal characters to decimal */

    BigInt* decimalBig = BigInt_construct(0); // Initializing BigInt is very important
    if (!signComparer) return decimalBig; // Handle '0' input

    BigInt* exp = BigInt_construct(8);

    int indx = strlen(octal) - 1; long long p = 0; // Power of 8

    for (indx, p; indx >- 1; --indx, ++p){
        // On each iteration: get the current character, if '0' or '-' (negative sign), move to the previous character (--indx) and increase the power
        if (octal[indx] == '0' || octal[indx] == '-') continue;
        BigInt* multiplicationOperand1 =  BigInt_pow(exp,p); // The first oprand in the multiplication process is set to the power result (8^sth)
        int multiplicationOperand2 = octal[indx]-'0'; // The secon oprand in the multiplication process is set to the current value in the octal number
        BigInt_multiply_int(multiplicationOperand1, multiplicationOperand2); 
        BigInt_add(decimalBig, multiplicationOperand1);
    }

    BigInt_free(exp);
    if (signComparer < 0) BigInt_multiply_int(decimalBig, -1); // If input is negative, multiply by -1
    return decimalBig;
}


void decimalToOctal(BigInt* decimalNumber, char *octalArrayReturn, int signComparer){ 
    /**A function to convert a BigInt decimal number to octal number
     * assign the first digit in the octal array to '-' sign only if the number is negative
    */
    
    // Handle Input zero case
    if (!signComparer){ // i.e exactly == zero
        octalArrayReturn[0] = '0';
        return;
    }

    BigInt* divisor = BigInt_construct(8);
    BigInt* remainder = BigInt_construct(1);
    // Make a clone of the bigInt so when modifying it, it doesn't affect other conversions
    BigInt* decimalNumberClone = BigInt_clone(decimalNumber, 512); 
    long long index = 0;  // Keep track of the current index of the hexArray

    do{
        // Make the division, assign the quotient in decimalNumber, assign the remainder to the remainder variable {neat right?}
        BigInt_divide(decimalNumberClone, divisor, decimalNumberClone, remainder); 

        char rem[2] = {}; // Get the remainder in a string, important to use BigInt_to_string() function

        BigInt_to_string(remainder, rem, 2); //convert the remainder (as BigInt) to a string (to modify the octalArray)
        
        octalArrayReturn[index] = rem[0];
        index++;
    }while (BigInt_compare_int(decimalNumberClone, 0) != 0);

    // It's very important to freeup all BigInt number created, otherwise it's an elegant way to kill the RAM ^_^
    BigInt_free(decimalNumberClone);
    BigInt_free(divisor);
    BigInt_free(remainder);

    if (signComparer < 0){ // Handle negative input
        // Make the last character negative sign, remember! the string will be printed in reverse
        octalArrayReturn[index] = '-'; 
    }
}


//! **************************************************************************************************Hexadecimal
BigInt* hexToDecimal(char* hex, int signComparer){
    /** A function to convert an array of hex characters to decimal */

    // printf("sc%d\n", signComparer);
    BigInt* decimalBig = BigInt_construct(0); // Initializing BigInt is very important
    if (!signComparer) return decimalBig; // Handle '0' input

    BigInt* exp = BigInt_construct(16);
    int indx = strlen(hex) - 1; long long p = 0; // Power of 16

    for (indx, p; indx > -1; --indx, ++p){
        // On each iteration: get the current character, if '0' or '-' (negative sign), move to the previous character (--indx) and increase the power

        if (hex[indx] == '0' || hex[indx] == '-') continue;
        BigInt* multiplicationOperand1 =  BigInt_pow(exp,p); // The first oprand in the multiplication process is set to the power result (8^sth)
        int multiplicationOperand2 = -1; // Initialize the variable to a negative for better debugging
            // To get the value of the current digit in the hexArray, we check if it's a number (return this value - '0' to conver from ASCII to int)
                // and if it's a character, convert it to an integer 
        if ('0' <= hex[indx] && hex[indx] <= '9')  multiplicationOperand2 = hex[indx] - '0';
        else if ('a' <= hex[indx] && hex[indx] <= 'f')  multiplicationOperand2 = hex[indx] - 'a' + 10; // Lowercase must be checked first
        else if ('A' <= hex[indx] && hex[indx] <= 'F')  multiplicationOperand2 = hex[indx] - 'A' + 10;        
        BigInt_multiply_int(multiplicationOperand1, multiplicationOperand2);    
        BigInt_add(decimalBig, multiplicationOperand1);
    }

    BigInt_free(exp);
    if (signComparer < 0) BigInt_multiply_int(decimalBig, -1); // If input is negative, multiply by -1
    return decimalBig;
}



bool decimalToHex(BigInt* decimalNumber, char* hexArrayUpper, char* hexArrayLower, int signComparer){ 
    /**A function to convert a BigInt decimal number to hex number
     * Assign the first digit in the hex array to '-' sign only if the number is negative
    */
    
    bool haveCharacters = false; // To know if there's characters in the hex number and pring upper and lower or only upper
    // Handle Input zero case
    if (!signComparer){ // i.e the input number exactly == zero
        hexArrayUpper[0] = '0';
        hexArrayLower[0] = '0';
        return false;
    }
    BigInt* divisor = BigInt_construct(16);
    BigInt* remainder = BigInt_construct(0);
    // Make a clone of the bigInt so when modifying it, it doesn't affect other conversions
    BigInt* decimalNumberClone = BigInt_clone(decimalNumber, 512); 
    long long index = 0;  // Keep track of the current index of the hexArray

    do{
        // Make the division, assign the quotient in decimalNumber, assign the remainder to the remainder variable {neat right?}
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
            // Otherwise, we get the corresponding hex value of the remainder [A-F] for upper array
            hexArrayUpper[index] = remInt + 55; 
            // And store the same value but lower in the lower array
            hexArrayLower[index] = hexArrayUpper[index] + ' '; // Add space or 32 in ASCII to convert from upper to lower
        }         
        index++;
    }while (BigInt_compare_int(decimalNumberClone, 0) != 0);
    // Prompt the user to pring the number in 

    // It's very important to freeup all BigInt number created, otherwise it's an elegant way to kill the RAM ^_^
    BigInt_free(decimalNumberClone);
    BigInt_free(divisor);
    BigInt_free(remainder);

    if (signComparer < 0){ // Handle negative input
        // Make the last character negative sign, remember! the string will be printed in reverse
        hexArrayUpper[index] = '-'; 
        hexArrayLower[index] = '-';
    }
    return haveCharacters;
}



//! *************************************************************** End of conversions *****************************************************************************/
void displayClosingMessage() {
    /*A function to print the closing screen.*/
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

bool checkResponse(int numOfFlushes, char response){
    COLOR_INIT // Initiate the color changing 
    if (((response != 'y' && response != 'Y') && (response != 'n' && response != 'N')) || numOfFlushes != 0){
        COLOR_RED
        printf("\aInvalid input. Please try again and enter Y/y OR N/n.\n");
        COLOR_RESET
        return true;
    }
    return false;
}

char getCharResponse(){
    /*A function to check the input character from the user (y,n). 
      Implemented here to free some space in main*/
    char response = 0; // To store the response of the user about leaving the program
    int numOfFlushes = 0;
    do {
        scanf(" %c", &response);
        numOfFlushes = flushBufferReturnCounter(); // Read the buffer, if not zero, the user entered more than one character.
    } while (checkResponse(numOfFlushes, response));
    return response;
}

bool tryAgain(char response){
    /*A function to determine whether to exit the program or to start again based on user choice*/
    if (response == 'n' || response == 'N'){ 
        displayClosingMessage(); // If the user entered (N,n), Then display the message and exit the program.
        return false;
    }
    else{
        system("cls"); // To clear the screen and try again.
        return true;
    }
}

void binaryConversions(char* inputNumber, char* octalArray, char* hexArrayU, char* hexArrayL, int compareToZero){
    BigInt* decimalInput = binaryToDecimal(inputNumber, compareToZero);
    printf("decimal: "); BigInt_print(decimalInput);
    decimalToOctal(decimalInput, octalArray, compareToZero);
    bool haveCharacters = decimalToHex(decimalInput, hexArrayU, hexArrayL, compareToZero);
    printf("\n\noct: %s\nhexU: %s\n", strrev(octalArray), strrev(hexArrayU));
    if (haveCharacters) printf("hexL: %s", strrev(hexArrayL));
}

void octalConversions(char* inputNumber, char* binaryArray, char* hexArrayU, char* hexArrayL, int compareToZero){
    BigInt* decimalInput = octalToDecimal(inputNumber, compareToZero);
    printf("decimal: "); BigInt_print(decimalInput);
    decimalToBinary(decimalInput, binaryArray, compareToZero);
    bool haveCharacters = decimalToHex(decimalInput, hexArrayU, hexArrayL, compareToZero);
    printf("\n\nbin: %s\nhexU: %s\n", strrev(binaryArray), strrev(hexArrayU));
    if (haveCharacters) printf("hexL: %s", strrev(hexArrayL));
}

void decimalConversions(BigInt* inputNumber, char* binaryArray, char* octalArray, char* hexArrayU, char* hexArrayL, int compareToZero){
    decimalToBinary(inputNumber, binaryArray, compareToZero);
    decimalToOctal(inputNumber, octalArray, compareToZero);
    bool haveCharacters = decimalToHex(inputNumber, hexArrayU, hexArrayL, compareToZero);    
    printf("\n\nbin: %s\noct: %s\nhexU: %s\n", strrev(binaryArray), strrev(octalArray), strrev(hexArrayU));
    if (haveCharacters) printf("hexL: %s", strrev(hexArrayL));
}

void hexConversions(char* inputNumber, char* binaryArray, char* octalArray, int compareToZero){
    BigInt* decimalInput = hexToDecimal(inputNumber, compareToZero);
    decimalToBinary(decimalInput, binaryArray, compareToZero);
    decimalToOctal(decimalInput, octalArray, compareToZero);
    printf("\n\nbin: %s\noct: %s\ndec: ", strrev(binaryArray), strrev(octalArray));
    BigInt_print(decimalInput);

}



void getInputAndConvert(int chosenSystem){
    COLOR_INIT
    int numOfFlushes;
    COLOR_YELLOW
        printf("\t\tPlease Enter the %s number to be converted.\n", systemIndexes[chosenSystem]);
        COLOR_RESET
        int sizeOfArrays = 512; //! Change in "scanf()" as well
        char inputNumber[sizeOfArrays]; // Array to store user input
        // Get user input and validate the input based on the system user chose
        do{
            scanf("%511s", &inputNumber);
            numOfFlushes = flushBufferReturnCounter();
        }while(validateSystem(inputNumber, numOfFlushes, chosenSystem));
        
        // Initialize containers for the result of the conversions
        char binaryArray[sizeOfArrays];
        char octalArray[sizeOfArrays];
        char hexArrayU[sizeOfArrays];
        char hexArrayL[sizeOfArrays];
        int compareToZero = 1; // Initialize the compare variable to determine whether the variable is zero, positive or negative
        
        BigInt* decimalInput = BigInt_construct(0); // Initialize a BigInt variable to store decimal conversions

        // printf("Debug567  %s\n", inputNumber);

        if (chosenSystem != 4){
            decimalInput =  BigInt_from_string(inputNumber); // Only convert user input number to BigInt if it's not hexadecimal
            compareToZero = BigInt_compare_int(decimalInput, 0); 
            // Result of comparing the user input by zero {to handle negatives}
                // The BigInt_compare_int() returns 0 if both equal, 1 if the first is bigger 
                    //(i.e positive) , and -1 if the latter is bigger (negative)
        } 
        else{
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


//! ******************************************** main ******************************************************************/
//! ******************************************** main ******************************************************************/
int main(){

    COLOR_INIT
    char response;  // Global variable to store user choice whether to leave the program or to try again.
    initWelcomeScreen();

    do {
        conversionList(); // Calling the conversion list only each time the program starts over
        response = 0;
        int chosenSystem = 0, 
        //The actual system the user chose
        numOfScannedVariables = 0, 
        //The number of scanned variables 
            //(return 0 if the above variable wasn't scanned)
        numOfFlushes = 0; 
        //Check if there was additional unintended input 
            //(counts how many times a character has been removed from the input stream)

        do{

            numOfScannedVariables = scanf("%d", &chosenSystem);
            numOfFlushes = flushBufferReturnCounter();
        }while(checkSystemIndex(chosenSystem, numOfScannedVariables, numOfFlushes));

        if (chosenSystem == 5){ // Handle exiting the program.
            tryAgain('n'); 
            // This should pass 'n' to tryAgain function which will display the closing display.
            break; // Break from the loop (i.e return 0 to system and end the program).
        }

        getInputAndConvert(chosenSystem);




        COLOR_BLUE
        printf( "\n<------------------------------------------------------>\n"
                "<------------------------------------------------------>\n");
        COLOR_RESET
        printf("If you want to try again, enter (Y/y).\nIf you want to end the program, enter (N/n).\n");
        response = getCharResponse(); 

        // Freeing bigints
            // BigInt_free(decimalBigInt);
    }while(tryAgain(response)); // Loop the program again until the user enter (N,n).
  return 0;
}
