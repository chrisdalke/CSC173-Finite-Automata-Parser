//////////////////////////////////////////////
// CSC 173 - Project 1
// Chris Dalke
//////////////////////////////////////////////
// File: NFA Implementation
//////////////////////////////////////////////
// All work in this file is my own.
//////////////////////////////////////////////

//////////////////////////////////////////////
// Includes
//////////////////////////////////////////////

#include "NFA.h"

//////////////////////////////////////////////
// Allocation and Deallocation
//////////////////////////////////////////////

NFA *NFA_new(int nstates){
	NFA *nfa = (NFA*)malloc(sizeof(NFA));
	nfa->nstates = nstates;

	nfa->states = (NFA_State*)malloc(sizeof(NFA_State)*nfa->nstates);
	for (int i = 0; i < nfa->nstates; i++){
		for (int n = 0; n < NFA_NSYMBOLS; n++){
			//allocate new IntSet object
			nfa->states[i].transitions[n] = IntSet_new();
		}
		nfa->states[i].is_accepting = 0;
	}
	NFA_set_current_state(nfa,NFA_START_STATE);
	return nfa;
}

void NFA_free(NFA *nfa){
	free(nfa);
}


int NFA_get_size(NFA *nfa){
	return nfa->nstates;
}


IntSet *NFA_get_transitions(NFA *nfa, int statenum, char symbol){
	return nfa->states[statenum].transitions[(int)symbol];
}


void NFA_add_transition(NFA *nfa, int src, char symbol, int dst){
	IntSet_add(nfa->states[src].transitions[(int)symbol],dst);
}

void NFA_add_transition_str(NFA *nfa, int src, char *str, int dst){
	for (int i = 0; i < strlen(str); i++){
		IntSet_add(nfa->states[src].transitions[(int)str[i]],dst);
	}
}

void NFA_add_transition_all(NFA *nfa, int src, int dst){
	for (int n = 0; n < NFA_NSYMBOLS; n++){
		IntSet_add(nfa->states[src].transitions[n],dst);
	}
}

void NFA_add_transition_all_except(NFA *nfa, int src, char *str, int dst){
	//Add transition in of the symbols,
	//except the ones that are a part of the string.
	for (int n = 0; n < NFA_NSYMBOLS; n++){
		int inString = 0;
		for (int i = 0; i < strlen(str); i++){
			if (n == str[i]){
				inString = 1;
			}
		}
		if (inString == 0){
			printf("Adding transition on %c to %i\n",n,dst);
			IntSet_add(nfa->states[src].transitions[n],dst);
		}
	}
}

IntSet *NFA_get_current_states(NFA *nfa){
	return nfa->current_states;
}

void NFA_set_current_states(NFA *nfa, IntSet *states){
	//IntSet_free(nfa->current_states);
	nfa->current_states = states;
}

void NFA_set_current_state(NFA *nfa, int state){
	nfa->current_states = IntSet_new();
	IntSet_add(nfa->current_states,state);
}

bool NFA_get_accepting(NFA *nfa, int statenum){
	return nfa->states[statenum].is_accepting;
}

void NFA_set_accepting(NFA *nfa, int statenum, bool value){
	nfa->states[statenum].is_accepting = value;

}

bool NFA_is_accepting(NFA *nfa){
	//Look through all states and return true if one of them is an accepting state
	for (int i = 0; i < nfa->nstates; i++){
		if (nfa->states[i].is_accepting){
			if (IntSet_contains(nfa->current_states,i)){
				return true;
			}
		}
	}
	return false;
}


bool NFA_execute(NFA *nfa, char *input){
	//Runs through every character in the input string through the nfa

	if (NFA_DEBUG_TRACE) printf("Executing... length %i\n",(int)strlen(input));

	int isAccepting = 0;

	for (int c = 0; c < strlen(input); c++){


		if (NFA_DEBUG_TRACE) printf("char=%i, In states: \n",c);
		if (NFA_DEBUG_TRACE) IntSet_print(nfa->current_states);

		if (NFA_DEBUG_TRACE) printf("Reading input character %c...\n",input[c]);

		//Build a set of all the states we could be in, using all the states that we are currently in
		IntSet *next = IntSet_new();
		IntSetIterator *currentIterator = IntSet_iterator(nfa->current_states);
		while (IntSetIterator_has_next(currentIterator)){
			IntSet_union(next, NFA_get_transitions(nfa,IntSetIterator_next(currentIterator),input[c]));
		}

		if (NFA_DEBUG_TRACE) printf("Could go to any one of the following states: \n");
		if (NFA_DEBUG_TRACE) IntSet_print(next);

		//Apply next transitions and check acceptance
		//Also flag rejected string so we can decide what to do with it
		if (!IntSet_is_empty(next)){
			NFA_set_current_states(nfa,next);
			isAccepting = NFA_is_accepting(nfa);
		} else {
			isAccepting = 0;
			break;
		}

		//Check if this is the last character
		//If this is the last character, break out of the loop.
		if (c == (strlen(input) - 1)){
			break;
		}


	}

	if (NFA_DEBUG_TRACE){
		printf("Landed in states: \n");
		IntSet_print(nfa->current_states);
		printf("is accepting? %i\n",isAccepting);
	}

	//Reset the state after completion
	NFA_set_current_state(nfa,0);

	return isAccepting;



}

//////////////////////////////////////////////
// End of File
//////////////////////////////////////////////