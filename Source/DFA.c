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
			dfa->states[i]->transitions[n] = -1;
		}
		dfa->states[i]->is_accepting = 0;
	}
	dfa->current_state = DFA_START_STATE;
}

/*
Deallocates the reference to DFA and all children.
*/
void DFA_free(DFA *dfa){
	//First, remove all the states
	for (int i = 0; i < dfa->nstates; i++){
		free(dfa->states[i]);
	}
	//Then, remove the DFA itself
	free(dfa);
}

/* 
Returns the size of the DFA. 
Unclear what this size is referencing...
*/
int DFA_get_size(DFA *dfa){
	//returns the size of the DFA, as what, number of states?
	return dfa->nstates;
}

//////////////////////////////////////////////
// Transition Functions
//////////////////////////////////////////////

int DFA_get_transition(DFA *dfa, int statenum, char symbol){
	return dfa->states[statenum]->transitions[symbol];
}

void DFA_set_transition(DFA *dfa, int src, char symbol, int dst){
	dfa->states[statenum]->transitions[symbol]=dst;
}

void DFA_set_transition_str(DFA *dfa, int src, char *str, int dst){
	for (int i = 0; i < sizeof(str); i++){
		for (int n = 0; n < DFA_NSYMBOLS; n++){
			dfa->states[src]->transitions[str[i]] = dst;
		}
	}
}

void DFA_set_transition_all(DFA *dfa, int src, int dst){
		for (int n = 0; n < DFA_NSYMBOLS; n++){
			dfa->states[src]->transitions[n] = dst;
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
	return dfa->states[statenum]->is_accepting;
}
void DFA_set_accepting(DFA *dfa, int statenum, int value){
	dfa->states[statenum]->is_accepting = value;
}

int DFA_is_accepting(DFA *dfa){
	return dfa->states[dfa->current_state]->is_accepting;
}

//////////////////////////////////////////////
// DFA Execution
/////////////////////////////////////////////

int DFA_execute(DFA *dfa, char *input){
	//Runs through every character in the input string through the dfa
	for (int c = 0; c < sizeof(input); c++){
		int next = DFA_get_transition(dfa,dfa->current_state,input[c]);
		if (next != -1){
			DFA_set_current_state(dfa,next);
		} else {
			break;
		}
	}
	return DFA_is_accepting(dfa);
}

//////////////////////////////////////////////
// Utility / Debug 
//////////////////////////////////////////////

/*
Prints the current DFA to the standard output.
Used only for debug purposes.
*/
void DFA_print(DFA *dfa){

}

//////////////////////////////////////////////
// End of File
//////////////////////////////////////////////