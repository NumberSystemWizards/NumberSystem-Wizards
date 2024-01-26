#include "NumberSystemWizards_v2.c"

/* This is a number system converter supports converting from and to (bin, oct, dec, hex) */
int main(){
    COLOR_INIT
    char response;  // To store user choice whether to leave the program or to try again.
    initWelcomeScreen(); // Display welcome screen and get the user name

    do {
        response = 0;
        conversionList(); // Calling the conversion list only each time the program starts over
        int chosenSystem = getChosenSystem(); // Get and validate the chosen input system

        if (chosenSystem == 5){ // Handle exiting the program.
            tryAgain('n'); 
            // This should pass 'n' to tryAgain function which will display the closing message.
            break; // Break from the loop (i.e close the program).
        }

        getInputNumberAndConvert(chosenSystem); // Get the input number from the user, validate it, and print its conversions

        printf("\nIf you want to try again, enter (Y/y).\nIf you want to end the program, enter (N/n).\n");
        response = getCharResponse(); // Get the response of the previous question

    }while(tryAgain(response)); // Loop the program again until the user enter (N,n).
    printf("Press Enter to exit..."); getchar(); // To stop the terminal from disappearing after the closing display
  return 0;
}