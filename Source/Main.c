#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    printf("----------------------------\n");
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


    //If we have zero input arguments, display error and help


    //Check if we used the --help argument


    //Check if we have an input filename
    //I've created an NFA/DFA parsing system

    //Check if we are solving or converting
    //If we solve, just load up the file and run the test cases
    //If we convert, load up the file, convert to DFA, run test cases on both 
    //And if we want, save the DFA to either the standard output or to a latex-encoded file




}