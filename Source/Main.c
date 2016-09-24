#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

    printf("----------------------------------\n");
    printf("CSC 173 - Project 1\n");
    printf("Chris Dalke\n");
    printf("----------------------------------\n");
/*
    printf("Input File: \n");
    printf("----------------------------\n");
    printf("Loading................ DONE \n");
    printf("Converting to DFA...... DONE \n");
    printf("Exporting to LaTeX..... DONE \n");
    printf("----------------------------\n");
    printf("Running Test Cases on NFA: \n");
    printf("ab => true \n");
    printf("abab => false \n");
    printf("Running Test Cases on DFA: \n");
    printf("ab => true \n");
    printf("abab => false \n");
    printf("2/2 Test Cases match! \n");
    printf("----------------------------\n");

*/

    //Loop through all the command line arguments, saving any arguments that are important to us.
    //Default arguments:
    char *helpArg = "--help";
    char *inputArg = "-I";
    //char *outputArg = "-O";
    char *convertArg = "-C";

    char *inputFile;
    int helpMode = 0;
    int inputMode = 0;
    int outputMode = 0;
    int convertMode = 0;

    for (int i = 0; i < argc; i++){
        if (strcmp(argv[i],helpArg) == 0){
            //Set the help-mode flag
            helpMode = 1;
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
        printf("Command-Line Options:\n");
        printf("--help: Displays this help text.\n");
        printf("-I <Input File>: Sets the input filename (Required)\n");
        printf("-C: Enables NFA->DFA Conversion mode (Optional)\n");
    } else {
        //Not help mode, handle the loading
        if (inputMode == 1){
            printf("Input File: %s\n\n",inputFile);
            //Try to open our input file
            FILE *fp;
            fp = fopen(inputFile,"r");
            printf("Loading................ ");
            if (fp != NULL){
                //Hand off this data to the parser function which will read the file and simulate it
                printf(" DONE\n");
                printf("Parsing................ \n");



            } else {
                printf(" ERROR\n");
                printf("Error: Could not open file. \nPlease read README.txt for help!\n");
            }
            fclose(fp);

        } else {
            printf("Error: Could not understand command-line arguments. Use --help to view options.");
        }
    }


    printf("----------------------------------\n");

}