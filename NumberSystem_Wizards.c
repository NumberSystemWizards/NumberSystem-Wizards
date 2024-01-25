#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <windows.h>

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

// A function to welcome the user with a smiley face and print it's name
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

/*A function to print the options list.*/
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
    printf("\n\tPlease select the required system from the previous list\n"); COLOR_YELLOW printf("Enter here:  ");
    COLOR_RESET
}


/* Removes unwanted characters from the input stream, return the number of flushed characters */
int flushBufferReturnCounter(){
    char dummyChar; // Create a dummy character to consume the buffer 
    int numOfCharactersConsumed=0; // Count how many characters consumed
    // Create a loop to repeatedly exit the loop when newline or endOfLine is encountered
    while((dummyChar = getchar()) != '\n' && dummyChar != EOF){
        if (dummyChar != ' ') numOfCharactersConsumed++; 
    }
    return numOfCharactersConsumed;
}


/*Check if 
    * 1- The System index the user chose is actually valid i.e from [1 - 4]
    * 2- If the user unintentionally entered a character, string, float, or any other type
    * return a boolean expression (0,1) where 0 indicates the input is valid and terminate the while loop
    * 1 indecates the input is not valid and continue the while loop and get the input again
    */
bool checkSystemIndex(int chosenSystem, int numOfScannedVariables, int numOfFlushes){

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


/*Check the validity of the entered Hex number
    * first it checks if the character is digit, if not, it checks if it's {a,b,c,d,e,f}.
    * returns a boolean expression (0,1)where 0 indicates the input is valid and terminate the while loop
    * and 1 indecates the input is not valid and continue the while loop and get the input again.
    * */
bool checkHexSystem(char *hexArray, int numOfFlushes){
    COLOR_INIT
    int maxDigits = maxInputPerSystem[4];
    int arrayLength = strlen(hexArray);
    if (numOfFlushes != 0 || arrayLength > maxDigits){
        COLOR_RED
        printf("\aMax digits allowed for Hexadecimal system is %d! Please try again:\n", maxDigits);
        COLOR_RESET
        return true;
    }
    while(arrayLength--){  
        char currentChar = hexArray[arrayLength]; // The current char in the iteration. 
        if (currentChar > 'Z') currentChar -= ' '; // Convert the lower to upper case, for robust if condition
        if (!(isdigit(currentChar))){
            if (currentChar > 'F' || currentChar < 'A'){
                COLOR_RED
                printf("\aPlease enter a valid Hexadecimal number. Hex digits ranged from [1 - 9] and [A - F] inclusive.\n");
                COLOR_RESET
                return true;
            }
        }
    }
    return false;
}


/*Check the validity of the entered binary, octal, decimal number
    * the reason for the seperate function is than hex's input is a string where other system's is long int
    * returns a boolean expression (0,1)where 0 indicates the input is valid and terminate the while loop
    * and 1 indecates the input is not valid and continue the while loop and get the input again 
    * */
bool checkBinOctDecSystems(unsigned long long int inputNumber, int numOfFlushes, int numOfScannedVariables, int chosenSystem){
    COLOR_INIT
    if (numOfScannedVariables != 1 || numOfFlushes != 0){
        // If the user entered a string or any other datatype except the intended
        COLOR_RED
        printf("\aPlease enter a valid %s number!\n", systemIndexes[chosenSystem]);
        COLOR_RESET
        return true;
    }

    int maxDigits = maxInputPerSystem[chosenSystem];
    int digitsCount = 0; // A container to count the number of digits

    if (inputNumber == 0) digitsCount = 1;
    else digitsCount = log10(inputNumber) + 1; // A formula to get the number of digits in a variable

    if (digitsCount > maxDigits){ 
        // If the user entered more digits than the max allowed
        COLOR_RED
        printf("\aMax digits allowed for %s system is %d! Please try again:\n", systemIndexes[chosenSystem], maxDigits);
        COLOR_RESET
        return true;
    }

    int lastDigit = 0; // The last digit in the number
    unsigned long long int tempInputSystem = inputNumber; // A temporary variable to check all of its digits
    switch (chosenSystem)
    {
        // Make appropriate validations with for each input system
        case 1: // Binary input
            while(tempInputSystem != 0){
                lastDigit = tempInputSystem % 10; // Get the last digit
                if (lastDigit > 1){
                    // Not a valid binary number
                    COLOR_RED
                    printf("\aPlease enter a valid binary number, consists only of 1's and 0's!\n");
                    COLOR_RESET
                    return true;
                }
                tempInputSystem /= 10;
            }
            return false; // If it's out of the while loop without getting any errors, then the nubmer is valid
            break;

        case 2:
            while(tempInputSystem != 0){
                lastDigit = tempInputSystem % 10;
                if (lastDigit > 7){
                    // Not a valid octal number
                    COLOR_RED
                    printf("\aPlease enter a valid octal number, consists only of digits from 0 to 7 only (inclusive)!\n");
                    COLOR_RESET
                    return true;
                }
                tempInputSystem /= 10;
            }
            return false; // If it's out of the while loop without getting any errors, then the nubmer is valid
            break;

        case 3: 
            // We are sure that if the user entered a number and it passed the length, number of scanned variables, and buffer test,
                // then it's a decimal number for sure!
            return false;
            break;
    }
    return false;
}


/** Converting decimal to binary.
 * Accepts a long long decimal representation &
     a pointer to the binary array in the main to be modified.
    * The function doesn't return anything.*/
void decimalToBinary(long long decimalNum, char *binaryNumReturn){
    long long index = 0, remainder = 0;
    do{
        remainder = decimalNum % 2;
        binaryNumReturn[index] = remainder + '0';
        decimalNum /= 2;
        index++;
    }while (decimalNum > 0);
    // No need to return anything since we're modifying the array directly.
}

/* Converting binary to decimal
    * Accepts a long long binary number (i.e consists only of 1 and 0)
    * Returns a long long decimal*/
long long binaryToDecimal(long long binaryNum){
    long long decimalNumReturn = 0, power = 0, remainder = 0, base = 0;
    do{
        remainder = binaryNum % 10;
        base = remainder * (long long)pow(2,power);
        decimalNumReturn += base;
        binaryNum /= 10;
        power++;
    }while(binaryNum > 0);

    return decimalNumReturn;
}


/** Converting octal number to decimal.
 * Accepts an octal number represented in long long.
 * Returns a long long decimal number.*/
long long octalToDecimal(long long octalNumber){
    long long decimal = 0; // Decimal of type long long to avoid overflow
    int power = 0, // Power of 8 multipler
    reminder = 0; // Reminder and decimal equals zero to avoid garbage value

    // Iterate through each digit of the octal number
    while (octalNumber > 0){
        reminder = octalNumber % 10; // Extract the last digit of the octal number
        decimal = decimal + reminder * (long long)pow(8, power); // Convert the octal digit to decimal and accumulate
        octalNumber = octalNumber / 10; // Remove the last digit from the octal number
        power++; // Move to the next power of 8
    }
    return decimal; // Return the final decimal result
}

/** Converting decimal number to octal.
 * Accepts a decimal number represented in long long.
 * Returns an octal number represented in long long.*/
long long decimalToOctal(long long decimalNum){
    long long octalNumber = 0;
    int multiplier = 1, 
    reminder = 0;
    while (decimalNum > 0){
        reminder = decimalNum % 8; // Extract the remainder when dividing by 8
        // Accumulate the octal number with the remainder at the appropriate position
        octalNumber = octalNumber + reminder * multiplier; 
        multiplier = multiplier * 10; // Move to the next position in the octal number
        decimalNum = decimalNum / 8; // Remove the last digit from the decimal number
    }
    return octalNumber; // Return the final octal result
}


/**Converting hexdecimal to decimal.
 * Accepts a pointer to the begging of the hexdecimal array.
 * Retruns the decimal number represented in long long.
*/
long long hexDecimalToDecimal(char* hexDecimal){

    // Getting the length of the string.
    long long len = strlen(hexDecimal);
    long long decimal = 0, i = 0;

    for(int index = len-1; index >= 0; index--){
    /* The index now is the value of the rightmost character in the string.
     * If we have a string of 3 characters(length of string) that mean the ->
         rightmost character's index is 2(length-1){'a'(0),'b'(1),'c'(2)}*/

        if(hexDecimal[index] >= '0' && hexDecimal[index] <= '9'){
            /* Now we have to convert the character's ASCII Code to the real value
                 corresponding this character, and then multiply it by the weight.*/
            decimal += (hexDecimal[index] - 48) * (long long)pow(16,i);
            i++;
        }
        else if(hexDecimal[index] >= 'A' && hexDecimal[index] <= 'F'){

            decimal += (hexDecimal[index] - 55) * (long long)pow(16,i);
            i++;
        }
        else if(hexDecimal[index] >= 'a' && hexDecimal[index] <= 'f'){
            decimal += (hexDecimal[index] - 87) * (long long)pow(16,i);
            i++;
        }
    }
    return decimal;
}


/** Converting decimal to hexdecimal.
 * Accepts the decimal number to be converted & 
    a pointer to the hexdecimal array in the main function to be modified. 
    * The function doesn't return anything.
*/
void decimalToHexDecimal(long long decimal, char *hexDecimal){
    long long rem, i = 0;
    do{
        char digit;
        rem = decimal % 16;
        decimal = decimal / 16;
        if (rem >= 0 && rem <= 9 ){
            digit = rem + 48; /*the result value on the right  is converted to the character
                                corresponding to this ascii code value :{0,1,2,3,4,5,6,7,8,9}*/
        }
        else if (rem >= 10 && rem <= 15){
            //the result value on the right is converted to the character corresponding to this ascii code value: {A,B,C,D,E,F}
            digit = rem + 55;
        }
        hexDecimal[i] = digit; // Now we have to assign the characters in reversed.
        i++; // "i" ends with the length of the string of the hexDecimal number
    }while (decimal > 0);
    // So now we have to reverse the number to get the right answer.
    int length = i;
    char temp = 0;
    for(int index = 0; index < length/2; index++){
        temp = hexDecimal[index];
        hexDecimal[index] = hexDecimal[length - index - 1];
        hexDecimal[length - index - 1] = temp;
    } // By swapping, we have the right order.
}


/*A function to print the closing screen.*/
void displayClosingMessage() {
    COLOR_INIT
    COLOR_GREEN
    printf("\n\t\tMade By: ^NumberSystem Wizards^\n\n"
            "Master of welcomes and creative formatter: "); COLOR_RED printf("Abdulrahman Nader\n"); COLOR_GREEN
            printf("Troubleshoot wizard: ");                COLOR_RED printf("Mohannad Elsayed \n"); COLOR_GREEN
            printf("Pioneer of Transformation: ");          COLOR_RED printf("Hania Hisham\n"); COLOR_GREEN
            printf("Conversion Engineer: ");                COLOR_RED printf("Nada Ahmed\n"); COLOR_GREEN
            printf("Transformation Specialist: ");          COLOR_RED printf("Hannah Ramadan\n"); COLOR_GREEN
            printf("Farewell Formatter: ");                 COLOR_RED printf("Ganaa Attia\n"); COLOR_GREEN
            printf("Closing Scene Stylist: ");              COLOR_RED printf("Yara Hossam\n\n"); COLOR_GREEN
            printf("\t\t\t Bye Bye!\n");
    COLOR_RESET
}


// Check the response of the user to enter (y,n). Case doesn't matter
bool checkResponse(int numOfFlushes, char response){
    COLOR_INIT // Initiate the color changing 
    if (((response != 'y' && response != 'Y') && (response != 'n' && response != 'N')) || numOfFlushes != 0){
        COLOR_RED
        printf("\aInvalid input. Please try again and enter Y/y OR N/n.\n"); COLOR_YELLOW printf("Enter here:  ");
        COLOR_RESET
        return true;
    }
    return false;
}


/*A function to check the input character from the user (y,n). 
    Implemented here to free some space in main*/
char getCharResponse(){
    char response = 0; // To store the response of the user about leaving the program
    int numOfFlushes = 0;
    do {
        scanf(" %c", &response);
        numOfFlushes = flushBufferReturnCounter(); // Read the buffer, if not zero, the user entered more than one character.
    } while (checkResponse(numOfFlushes, response));
    return response;
}

/*A function to determine whether to exit the program or to start again based on user choice*/
bool tryAgain(char response){
    if (response == 'n' || response == 'N'){ 
        displayClosingMessage(); // If the user entered (N,n), Then display the message and exit the program.
        return false;
    }
    else{
        system("cls"); // To clear the screen and try again.
        return true;
    }
}


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

        unsigned long long int inputNumber = 0;
        COLOR_BLUE
        printf("\nPlease Enter, in one line, the number to be converted.\n"); 
        printf("\tThe %s number should not exceed ", systemIndexes[chosenSystem]); COLOR_RED printf("%d", maxInputPerSystem[chosenSystem]); COLOR_BLUE printf(" digits!\n");
        if (chosenSystem != 4){
            // 4 is the hex index in the systemIndexes array
            do{
                COLOR_YELLOW printf("Enter here:  "); COLOR_RESET
                numOfScannedVariables = scanf("%llu", &inputNumber);
                numOfFlushes = flushBufferReturnCounter();
            }while (checkBinOctDecSystems(inputNumber, numOfFlushes, numOfScannedVariables, chosenSystem));
        }
        char hexArray[maxInputPerSystem[4]]; // Arbitrary size, should not be exceeded! 
        int indx = 0; // Iterator used in for loops
        if (chosenSystem == 4){
            do{
                COLOR_YELLOW printf("Enter here:  "); COLOR_RESET
                scanf("%s", &hexArray);
                numOfFlushes = flushBufferReturnCounter();
            }while(checkHexSystem(hexArray, numOfFlushes));
        }

        // Initialize the variables to store the conversion values for each conversion type.
        long long inputToDecimalConvered = 0, 
            octalResult = 0; // To store the result of Octal conversions.
        // Initialize the arrays to zeros (important for "printf" to terminate the string)
        char decimalToHexdecimalArray[100] = {0};
        char decimalTobinaryArray[100] = {0};
        COLOR_YELLOW 
        printf("\n\tConversions:\n");
        COLOR_RESET
        switch(chosenSystem){
            case 1: // Convert to Oct, Dec, Hex
                inputToDecimalConvered = binaryToDecimal(inputNumber); // Get the decimal representation.
                octalResult = decimalToOctal(inputToDecimalConvered); // Use the decimal representation to get the other 2 systems.
                decimalToHexDecimal(inputToDecimalConvered, decimalToHexdecimalArray);
                printf("Octal: %15lld\nDecimal: %13lld\nHexadecimal: %9s\n", octalResult, 
                                                                            inputToDecimalConvered, 
                                                                            decimalToHexdecimalArray);
                break;
            case 2: // Convert to Bin, Dec, Hex
                inputToDecimalConvered = octalToDecimal(inputNumber);
                decimalToBinary(inputToDecimalConvered, decimalTobinaryArray);
                decimalToHexDecimal(inputToDecimalConvered, decimalToHexdecimalArray);
                printf("Binary: %51s\nDecimal: %50lld\nHexadecimal: %46s\n", strrev(decimalTobinaryArray),
                                                                            inputToDecimalConvered,
                                                                            decimalToHexdecimalArray);
                break;
            case 3: // Convert to Bin, Oct, Hex
                inputToDecimalConvered = inputNumber;
                decimalToBinary(inputNumber, decimalTobinaryArray);
                octalResult = decimalToOctal(inputNumber);
                decimalToHexDecimal(inputNumber, decimalToHexdecimalArray);
                printf("Binary: %30s\nOctal: %31lld\nHexadecimal: %25s\n",  strrev(decimalTobinaryArray),
                                                                            octalResult, 
                                                                            decimalToHexdecimalArray);
                break;
            case 4: // Convert to Bin, Oct, Dec
                inputToDecimalConvered = hexDecimalToDecimal(hexArray);
                decimalToBinary(inputToDecimalConvered, decimalTobinaryArray);
                octalResult = decimalToOctal(inputToDecimalConvered);
                printf("Binary: %29s\nOctal: %30lld\nDecimal: %28lld\n", strrev(decimalTobinaryArray),
                                                                        octalResult,
                                                                        inputToDecimalConvered);
                break;
        }
        COLOR_YELLOW
        printf( "\n<------------------------------------------------------>\n"
                "<------------------------------------------------------>\n\n");
        COLOR_BLUE
        printf("If you want to try again, enter (Y/y).\nIf you want to end the program, enter (N/n).\n"); COLOR_YELLOW printf("Enter here:  "); COLOR_RESET
        COLOR_RESET
        response = getCharResponse(); 
    }while(tryAgain(response)); // Loop the program again until the user enter (N,n).
  return 0;
}