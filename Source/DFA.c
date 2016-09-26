//////////////////////////////////////////////
// CSC 173 - Project 1
// Chris Dalke
//////////////////////////////////////////////
// File: DFA Implementation
//////////////////////////////////////////////
// All work in this file is my own.
//////////////////////////////////////////////

//////////////////////////////////////////////
// Includes
//////////////////////////////////////////////

#include "DFA.h"

//////////////////////////////////////////////
// Allocation and Deallocation
//////////////////////////////////////////////

/*
Allocates a new DFA with the specified number of states.
Sets all the state transitions to -1, representing the empty set.
Sets the current state to DFA_START_STATE (0)
*/
DFA *DFA_new(int nstates){
	DFA *dfa = (DFA*)malloc(sizeof(DFA));
	dfa->nstates = nstates;
	dfa->states = (DFA_State*)malloc(sizeof(DFA_State)*dfa->nstates);
	for (int i = 0; i < dfa->nstates; i++){
		for (int n = 0; n < DFA_NSYMBOLS; n++){
			dfa->states[i].transitions[n] = -1;
		}
		dfa->states[i].is_accepting = 0;
	}
	dfa->current_state = DFA_START_STATE;
	return dfa;
}

/*
Deallocates the reference to DFA and all children.
*/
void DFA_free(DFA *dfa){
	free(dfa);
}

//////////////////////////////////////////////
// Transition Functions
//////////////////////////////////////////////

int DFA_get_transition(DFA *dfa, int statenum, char symbol){
	return dfa->states[statenum].transitions[(int)symbol];
}

void DFA_set_transition(DFA *dfa, int src, char symbol, int dst){
	dfa->states[src].transitions[(int)symbol]=dst;
}

void DFA_set_transition_str(DFA *dfa, int src, char *str, int dst){
	for (int i = 0; i < strlen(str); i++){
		dfa->states[src].transitions[(int)str[i]] = dst;
	}
}

void DFA_set_transition_all(DFA *dfa, int src, int dst){
	for (int n = 0; n < DFA_NSYMBOLS; n++){
		dfa->states[src].transitions[n] = dst;
	}
}

//Creates a transition of everything except the characters in the string
void DFA_set_transition_all_except(DFA *dfa, int src, char *str, int dst){

	for (int n = 0; n < DFA_NSYMBOLS; n++){
		int isInString = 0;	
		for (int i = 0; i < strlen(str); i++){
			if (str[i] == n){
				isInString = 1;
			}
		}
		if (isInString == 0){
			dfa->states[src].transitions[n] = dst;
		}
	}
}


//////////////////////////////////////////////
// State Functions
//////////////////////////////////////////////

int DFA_get_current_state(DFA *dfa){
	return dfa->current_state;
}

void DFA_set_current_state(DFA *dfa, int statenum){
	dfa->current_state = statenum;
}

int DFA_get_accepting(DFA *dfa, int statenum){
	return dfa->states[statenum].is_accepting;
}

void DFA_set_accepting(DFA *dfa, int statenum, int value){
	dfa->states[statenum].is_accepting = value;
}

int DFA_is_accepting(DFA *dfa){
	return dfa->states[dfa->current_state].is_accepting;
}

//////////////////////////////////////////////
// DFA Execution
/////////////////////////////////////////////

int DFA_execute(DFA *dfa, char *input){
	//Runs through every character in the input string through the dfa

	if (DFA_DEBUG_TRACE) printf("Executing... length %i\n",(int)strlen(input));
	int isAccepting = 0;
	int isRejected = 0;
	for (int c = 0; c < strlen(input); c++){
		if (DFA_DEBUG_TRACE) printf("char=%i, In state %i, ",c,dfa->current_state);
		if (DFA_DEBUG_TRACE) printf("Reading input character %c...",input[c]);

		int next = DFA_get_transition(dfa,dfa->current_state,input[c]);

		//Apply next transition and check acceptance
		//Also flag rejected string so we can decide what to do with it
		if (next != -1){
			if (DFA_DEBUG_TRACE) printf("result: %i\n",next);
			DFA_set_current_state(dfa,next);
			isAccepting = DFA_is_accepting(dfa);
		} else {
			if (DFA_DEBUG_TRACE) printf("result: fail\n");
			isAccepting = 0;
			break;
		}

		//Check if this is the last character
		//If this is the last character, break out of the loop.
		if (c == (strlen(input) - 1)){
			break;
		}

	}

	if (DFA_DEBUG_TRACE) printf("Landed in state %i, is accepting? %i\n",dfa->current_state,isAccepting);

	//Reset the state after completion
	DFA_set_current_state(dfa,0);

	return isAccepting;
}

//////////////////////////////////////////////
// Utility / Debug 
//////////////////////////////////////////////

/* 
Returns the size of the DFA. 
Unclear what this size is referencing...
*/
int DFA_get_size(DFA *dfa){
	//returns the size of the DFA, as what, number of states?
	return dfa->nstates;
}

/*
Prints the current DFA to the standard output.
Used only for debug purposes.
*/
void DFA_print(DFA *dfa){

}

//////////////////////////////////////////////
// End of File
//////////////////////////////////////////////