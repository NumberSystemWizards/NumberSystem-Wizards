#include "NumberSystemWizards_v2.c"

int main(){
    COLOR_INIT
    char response;  // To store user choice whether to leave the program or to try again.
    initWelcomeScreen(); // Display welcome screen and get the user name

    do {
        response = 0;
        int chosenSystem = getChosenSystem(); // Get and validate the chosen input system

        if (chosenSystem == 5){ // Handle exiting the program.
            tryAgain('n'); 
            // This should pass 'n' to tryAgain function which will display the closing message.
            break; // Break from the loop (i.e close the program).
        }

        getInputNumberAndConvert(chosenSystem); // Get the input number from the user, validate it, and print its conversions

        COLOR_BLUE printf( "\n<------------------------------------------------------>\n"
                            "<------------------------------------------------------>\n"); COLOR_RESET
        
        printf("If you want to try again, enter (Y/y).\nIf you want to end the program, enter (N/n).\n");
        response = getCharResponse(); // Get the response of the previous question

    }while(tryAgain(response)); // Loop the program again until the user enter (N,n).
    printf("Press Enter to exit..."); getchar(); // To stop the terminal from dissappearing after the closing display
  return 0;
}