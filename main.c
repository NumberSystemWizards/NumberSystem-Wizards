#include "NumberSystemWizards_v2.c"

int main(){
    COLOR_INIT
    char response;  // Global variable to store user choice whether to leave the program or to try again.
    initWelcomeScreen();

    do {
        response = 0;
        int chosenSystem = getChosenSystem();

        if (chosenSystem == 5){ // Handle exiting the program.
            tryAgain('n'); 
            // This should pass 'n' to tryAgain function which will display the closing message.
            break; // Break from the loop (i.e close the program).
        }

        getInputNumberAndConvert(chosenSystem);

        COLOR_BLUE printf( "\n<------------------------------------------------------>\n"
                            "<------------------------------------------------------>\n"); COLOR_RESET
        
        printf("If you want to try again, enter (Y/y).\nIf you want to end the program, enter (N/n).\n");
        response = getCharResponse(); 

    }while(tryAgain(response)); // Loop the program again until the user enter (N,n).
    getchar();
  return 0;
}