#include "NumberSystemWizards_v2.c"

/* This is a number system converter supports converting from and to (bin, oct, dec, hex) */
int main(){
    COLOR_INIT
    initWelcomeScreen(); // Display welcome screen and get the user name
    bool flag = 1;
    do{
        int chosenSystem = beginningFunction(); // begin program and print the conversion list for the user to choose from
        int makeSure = 0; // To make sure he chose the system right.

        makeSure = getMakeSure();
        // 1 means the user meant to choose that system.
        // 2 means the user want to change what he chose.
        
        if(makeSure == 2){ //return back to the list
            system("cls"); // To clear the screen.
            continue;      // To  return back to the list.
        }
        else if (makeSure == 1){ // The user meant to chose that system.

            if (chosenSystem == 5){ // If he chose to exit the program, display the closing message and exit the loop
                displayClosingMessage();
                break; 
                }

            else { // The user doesn't want to exit the program, we enter another loop to get the input number to convert
                    do{
                        getInputNumberAndConvert(chosenSystem); // Get the input number from the user, validate it, and print its conversions
                        char response = optionList(chosenSystem); // Prompt the user to convert another number of the same system, return to the list, or to exit
                        
                        if (response == 'N' || response =='n') { // The user want to exit the program
                            displayClosingMessage(); // Display the closing message and exit the loop
                            flag = 0; // Set to 0 to exit the parent loop
                            break; 
                        }
                        else if (response == 'R' ||response == 'r'){
                            system("cls"); // To clear the screen and try again.
                            break;
                        }
                        // Else if (response =='y'|| response =='Y')
                        // He want to try that system again
                        // No need to write another else, the other 2 above are sufficient

                        }while(flag);
                    }
        }
    }while(flag); // Loop the program again until the user enter (N,n) or (5) in the first list.

    printf("Press Enter to exit..."); getchar(); // To stop the terminal from disappearing after the closing display
  return 0;
}
