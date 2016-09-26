//////////////////////////////////////////////
// CSC 173 - Project 1
// Chris Dalke
//////////////////////////////////////////////
// File: Main Source File
//////////////////////////////////////////////
// All work in this file is my own.
//////////////////////////////////////////////

//////////////////////////////////////////////
// Includes
//////////////////////////////////////////////

#include "Main.h"

//////////////////////////////////////////////
// Constants
//////////////////////////////////////////////

//Default arguments:
const char *helpArg = "--help";
const char *inputArg = "-F";
const char *interactArg = "-I";
const char *outputArg = "-O";
const char *convertArg = "-C";

//Declare constants for different file fields
const char *sizeLabel = "size:";
const char *joinLabel = "join:";
const char *testLabel = "test:";
const char *goalLabel = "goal:";

//////////////////////////////////////////////
// Main Function
//////////////////////////////////////////////

int main(int argc, char **argv) {

    displayHeader();

    //Loop through all the command line arguments, saving any arguments that are important to us.

    char *inputFile;
    int helpMode = 0;
    int inputMode = 0;
    int outputMode = 0;
    int convertMode = 0;
    int interactMode = 0;

    for (int i = 0; i < argc; i++){
        if (strcmp(argv[i],helpArg) == 0){
            //Set the help-mode flag
            helpMode = 1;
        }
        if (strcmp(argv[i],interactArg) == 0){
            //Set the interact-mode flag
            interactMode = 1;
        }
        if (strcmp(argv[i],convertArg) == 0){
            //Set the convert-mode flag
            convertMode = 1;
        }
        if (strcmp(argv[i],inputArg) == 0){
            //Make sure next item is in bounds before saving it as the input file
            if ((i+1)<argc){
                inputFile = argv[i+1];
                inputMode = 1;
            }
        }

    }

    //If we are in help-mode, print the help and then exit.
    if (helpMode == 1){
        displayHelp();
    } else {
        //Not help mode, handle the loading
        if (inputMode == 1){
            printf("Input File: %s\n",inputFile);

            //Try to open our input file
            FILE *fp;
            fp = fopen(inputFile,"r");
            printf("Loading................ ");
            if (fp != NULL){
                //Hand off this data to the parser function which will read the file and simulate it
                printf(" DONE\n");

                int isNFA = 0; //0 for DFA, 1 for NFA
                int validFile = 0; //1 when it is a valid file (valid extension)

                //Read filename to determine if it's an NFA or DFA
                char *extension = strrchr(inputFile, '.');
                if (!extension) {
                } else {
                    if (strcmp("dfa", extension + 1) == 0){
                        validFile = 1;
                        isNFA = 0;
                    } else if (strcmp("nfa", extension + 1) == 0){
                        validFile = 1;
                        isNFA = 1;
                    } else {
                        //Error
                    }
                }

                printf("Parsing................ ");
                if (validFile == 1){
                    //////////////////////////////////////////////////////////
                    //Actually start parsing the file here 

                    //Temp files for reading the line
                    char *tempLine = malloc(sizeof(char) * 128);
                    char *tempLineTrimmed = malloc(sizeof(char) * 128);

                    //Loop line by line. Each line can be one of three things:
                    //First line should be a size parameter, setting the size of the NFA
                    //Next lines should be "join" command, stands for a line between nodes of the autonoma
                    //Syntax for join:
                    //join:1,*,2 or join:<a>,<character to transition>,<b>
                    //Third possibility is a test: line
                    //This line runs a test case on the autonoma
                    //We store these tests in an array of strings

                    //Declare automata properties
                    int size = -1;

                    char *trimmedLabel =malloc(sizeof(char) * 6);

                    if ((fgets(tempLine,128,fp) != NULL)) {
                        //Look at first line of code for the size: argument
                        strncpy(trimmedLabel,tempLine,5);
                        trimmedLabel[5] = 0;

                        if (strcmp(trimmedLabel,sizeLabel) == 0){
                            size=atoi(tempLine + 5);
                        } else {
                            goto parsingError;
                        }
                    } else {
                        //Error with the size...
                        goto parsingError;
                    }

                    //Build the initial object since we now know the size
                    DFA *autonoma_DFA = NULL;
                    NFA *autonoma_NFA = NULL;
                    if (isNFA){
                        autonoma_NFA = NFA_new(size);
                    } else {
                        autonoma_DFA = DFA_new(size);
                    }

                    //Initialize a linked list of strings that we will use for test cases
                    LinkedList *testCases = LinkedList_new();

                    //Now, loop line by line through the file
                    while (fgets(tempLine,128,fp) != NULL){
                        //Read trimmed beginning of line
                        strncpy(trimmedLabel,tempLine,5);
                        trimmedLabel[5] = 0;
                        //Copy everything to the right of the label
                        strcpy(tempLineTrimmed,tempLine + 5);
                        //Removing newline...
                        tempLineTrimmed = strtok(tempLineTrimmed,"\n");

                        //Join
                        if (strcmp(trimmedLabel,joinLabel) == 0){
                            //Grab first, second, third parameters
                            char *first = strtok(tempLineTrimmed,",");
                            char *second = strtok(NULL,",");
                            char *third = strtok(NULL,",");

                            int source = atoi(first);
                            int dest = atoi(third);

                            char *wildcardChar = "*";
                            char *filteredWildcardChar = "*-"; //Symbol signifying we want everything EXCEPT the following characters
                            char *trimmedFilteredWildcardTest = malloc(sizeof(char) * 3);

                            strncpy(trimmedFilteredWildcardTest,second,2);

                            if (strcmp(second,wildcardChar) == 0){
                                //Wildcard character
                                if (autonoma_DFA != NULL) DFA_set_transition_all(autonoma_DFA,source,dest);
                                if (autonoma_NFA != NULL) NFA_add_transition_all(autonoma_NFA,source,dest);
                                //printf("Adding wildcard transition from %i to %i on %s\n",source,dest,second);
                            } else if (strcmp(trimmedFilteredWildcardTest,filteredWildcardChar) == 0) {
                                //Wildcard, minus whatever follows
                                printf("Adding filtered wildcard: %s\n",second+2);
                                if (autonoma_DFA != NULL) DFA_set_transition_all_except(autonoma_DFA,source,second + 2,dest);
                                if (autonoma_NFA != NULL) NFA_add_transition_all_except(autonoma_NFA,source,second + 2,dest);

                            } else {
                                if (autonoma_DFA != NULL) DFA_set_transition_str(autonoma_DFA,source,second,dest);
                                if (autonoma_NFA != NULL) NFA_add_transition_str(autonoma_NFA,source,second,dest);
                                //printf("Adding transition from %i to %i on %s\n",source,dest,second);
                            }
                        }

                        //Goal
                        if (strcmp(trimmedLabel,goalLabel) == 0){
                            if (autonoma_DFA != NULL) DFA_set_accepting(autonoma_DFA,atoi(tempLineTrimmed),1);
                            if (autonoma_NFA != NULL) NFA_set_accepting(autonoma_NFA,atoi(tempLineTrimmed),1);
                            //printf("Adding accepting state for %s\n",tempLineTrimmed);
                        }

                        //Test Case
                        if (strcmp(trimmedLabel,testLabel) == 0){
                            char *savedTestCase = malloc(sizeof(char) * 128);
                            strcpy(savedTestCase,tempLineTrimmed);
                            LinkedList_add_at_end(testCases,savedTestCase);
                        }
                    }
                    printf(" DONE\n");

                    //////////////////////////////////////////////
                    // NFA-to-DFA Conversion
                    //////////////////////////////////////////////

                    //Convert if convert-mode is on
                    if (convertMode && isNFA == 1){
                        printf("Converting................ DONE\n");
                        //If we are an NFA, convert to a DFA
                        autonoma_DFA = NFA_to_DFA(autonoma_NFA);
                    }

                    //////////////////////////////////////////////
                    // Test Case Evaluation
                    //////////////////////////////////////////////

                    //Run test cases
                    printf("----------------------------------\n");
                    printf("Test Cases:");

                    int testCaseResult = 0;

                    LinkedListIterator *testCaseIterator = LinkedList_iterator(testCases);

                    if (LinkedListIterator_has_next(testCaseIterator) == 0){
                        printf(" *None*\n");
                    } else {
                        printf("\n");

                        char *currentTestCase = LinkedListIterator_next(testCaseIterator);
                        while (currentTestCase != NULL){

                            if (autonoma_NFA != NULL){
                                testCaseResult = NFA_execute(autonoma_NFA,currentTestCase);
                                
                                if (testCaseResult == 1){
                                    printf("(NFA) %s => true\n",currentTestCase);
                                } else {
                                    printf("(NFA) %s => false\n",currentTestCase);
                                }
                            }
                            if (autonoma_DFA != NULL){
                                testCaseResult = DFA_execute(autonoma_DFA,currentTestCase);
                                
                                if (testCaseResult == 1){
                                    printf("(DFA) %s => true\n",currentTestCase);
                                } else {
                                    printf("(DFA) %s => false\n",currentTestCase);
                                }
                            }


                            //Get next item
                            currentTestCase = LinkedListIterator_next(testCaseIterator);
                        }
                    }

                    free(testCaseIterator);

                    //////////////////////////////////////////////
                    // Interact Mode
                    //////////////////////////////////////////////

                    //Run interactive-mode if it is enabled
                    if (interactMode){
                        printf("----------------------------------\n");
                        printf("Interactive Mode - Enter any string to test.\n");
                        printf("Press Ctrl-C or type 'exit' to finish.\n");

                        char *exitString = "exit";

                        while (1){
                            printf(">>> ");
                            fgets(tempLine,128,stdin);
                            if (strlen(tempLine) > 1){
                                tempLine = strtok(tempLine,"\n");

                                if (strcmp(exitString,tempLine)==0){
                                    break;
                                }

                                if (autonoma_NFA != NULL){
                                    testCaseResult = NFA_execute(autonoma_NFA,tempLine);
                                    
                                    if (testCaseResult == 1){
                                        printf("(NFA) %s => true\n",tempLine);
                                    } else {
                                        printf("(NFA) %s => false\n",tempLine);
                                    }
                                }
                                if (autonoma_DFA != NULL){
                                    testCaseResult = DFA_execute(autonoma_DFA,tempLine);
                                    
                                    if (testCaseResult == 1){
                                        printf("(DFA) %s => true\n",tempLine);
                                    } else {
                                        printf("(DFA) %s => false\n",tempLine);
                                    }
                                }
                            }
                        }
                    }

                    //Done 
                    //Cleanup data here
                    LinkedList_free(testCases,true);


                    if (autonoma_DFA != NULL) DFA_free(autonoma_DFA);
                    if (autonoma_NFA != NULL) NFA_free(autonoma_NFA);
                    

                    //////////////////////////////////////////////////////////

                } else {
                    parsingError:
                    printf(" ERROR\n");
                    printf("Error: Invalid file format. \nPlease read README.txt for help!\n");
                }


            } else {
                printf(" ERROR\n");
                printf("----------------------------------\n");
                printf("Error: Could not open file. \nPlease read README.txt for help!\n");
            }
            fclose(fp);

        } else {
            printf("Error: Could not understand command-line arguments. Use --help to view options.\n");
        }
    }

    displayFooter();
    
}

//////////////////////////////////////////////
// Display Helper Functions
//////////////////////////////////////////////

/*
Displays a command-line help screen.
*/
void displayHelp(){
    printf("Command-Line Options:\n");
    printf("--help: Displays this help text.\n");
    printf("-F <Input File>: Sets the input filename (Required)\n");
    printf("-C: Enables NFA->DFA Conversion mode (Optional)\n");
    printf("-I: Enables Interactive Mode, see README.txt for details (Optional)\n");
}

/*
Displays a header, intended for the beginning of the program.
*/
void displayHeader(){
    printf("----------------------------------\n");
    printf("CSC 173 - Project 1\n");
    printf("Chris Dalke\n");
    printf("----------------------------------\n");
}

/*
Displays a bar for the footer at the end of the program.
*/
void displayFooter(){
    printf("----------------------------------\n");
}

//////////////////////////////////////////////
// End of File
//////////////////////////////////////////////