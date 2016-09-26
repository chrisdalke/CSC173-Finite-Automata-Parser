//////////////////////////////////////////////
// CSC 173 - Project 1
// Chris Dalke
//////////////////////////////////////////////
// File: NFA to DFA Converter
//////////////////////////////////////////////
// All work in this file is my own.
//////////////////////////////////////////////

//////////////////////////////////////////////
// Includes
//////////////////////////////////////////////

#include "NFAConverter.h"

//////////////////////////////////////////////
// Conversion Algorithm
//////////////////////////////////////////////

//Uses the Subset Construction algorithm
//To convert a given NFA to an equivalent DFA.
DFA* NFA_to_DFA(NFA *nfa){

	//Initialize the NFA's state to 0 (default)
	NFA_set_current_state(nfa,NFA_START_STATE);

	//Create a list of states that we will use to build the NFA
	LinkedList *states = LinkedList_new();

	//Allocate an initial state 
	ConvertState *first = malloc(sizeof(ConvertState));
	first->states = IntSet_new();
	first->isNew = 1;
	IntSet_union(first->states,nfa->current_states);
	LinkedList_add_at_end(states,first);

	int hasNewStates = 1;
	while (hasNewStates == 1){

		ConvertState *newState = NULL;
		//Find the first state we can find that hasn't been state-propagated
		//Perform state propagation on it
		LinkedListIterator *newStateIterator = LinkedList_iterator(states);
		hasNewStates = 0;
		while (LinkedListIterator_has_next(newStateIterator)){
			newState = LinkedListIterator_next(newStateIterator);
			if (newState->isNew == 1){
				//This is a new state, we will perform state propogation on it after we exit this loop.
				hasNewStates = 1;
				break;
			}
		}

		if (hasNewStates == 1){
			//In state propogation, we look at the current states, 
			//and for every possible symbol for this "new" state
			//The state propogation function will also add any new states it finds to the master state list

			doStatePropagate(nfa,states,newState);

		} 
		//If we have no more "new" states, we have finished the algorithm
	}

	//Count up how many states there here
	int numStates = 0;
	LinkedListIterator *stateCounterIterator = LinkedList_iterator(states);
	while (LinkedListIterator_has_next(stateCounterIterator)){
		LinkedListIterator_next(stateCounterIterator);
		numStates++;
	}


	printf("Finished generating states, there were %i\n",numStates);

	//Build the DFA out of the list of data that we have
	DFA *dfa = DFA_new(numStates);

	LinkedListIterator *currentStatesIterator = LinkedList_iterator(states);
	while (LinkedListIterator_has_next(currentStatesIterator)){
		ConvertState *tempState = LinkedListIterator_next(currentStatesIterator);
		IntSet_print(tempState->states);

		//First, convert the temp state's IntSet representation into an ID
		int id = find_id_for_Setid(states,tempState->states);

		//Now, examine the transitions for each character from this state and convert that into the DFA form.
		for (int n = 0; n < NFA_NSYMBOLS; n++){
			int transitionId = find_id_for_Setid(states,tempState->transitions[n]);
			//Set the transition to the item with the id that matches this
			DFA_set_transition(dfa,id,n,transitionId);
		}

		//Determine if this state contains a finish state, and if it does, we want to set the finish state in the NFA
		IntSetIterator *acceptingStatesIterator = IntSet_iterator(tempState->states);
		while (IntSetIterator_has_next(acceptingStatesIterator)){
			int current = IntSetIterator_next(acceptingStatesIterator);
			if (NFA_get_accepting(nfa,current)){
				DFA_set_accepting(dfa,id,1);
			}
		}

	}

	return dfa;
}

//////////////////////////////////////////////
// Helper Functions
//////////////////////////////////////////////

void doStatePropagate(NFA *nfa, LinkedList *states, ConvertState *currentState){
	//Given a "new" DFA state:
	//printf("Performing state-propagation on the following set-state:\n");
	//IntSet_print(currentState->states);

	//Iterate through all current states and generate a list of all the states that the current state could possibly link to
	//We will do this for MULTIPLE input symbols (In fact, all possible input symbols!!)
	//Loop through every possible symbol!
	for (int n = 0; n < NFA_NSYMBOLS; n++){
		//Figure out the set of output states for this symbol
		//This transition needs to be later marked in the DFA
		//For now, we will save this in a transition

		//Allocate a new IntState for outputs from this symbol
		currentState->transitions[n] = IntSet_new();

		IntSetIterator *currentStatesIterator = IntSet_iterator(currentState->states);
		while (IntSetIterator_has_next(currentStatesIterator)){
			int current = IntSetIterator_next(currentStatesIterator);
			//Get transitions out of the current item in the state set, with the current symbol, add to the outbound transition set
			IntSet_union(currentState->transitions[n],NFA_get_transitions(nfa, current, n));
		}

		//This set is now the next state!
		//Check if this node already exists
		int stateAlreadyExists = 0;
		LinkedListIterator *statesListIterator = LinkedList_iterator(states);
		while (LinkedListIterator_has_next(statesListIterator)){
			if (IntSet_equals(((ConvertState*)LinkedListIterator_next(statesListIterator))->states,currentState->transitions[n])){
				stateAlreadyExists = 1;
			}
		}

		//If this state does not already exist, add it to the list
		if (stateAlreadyExists == 0){

			//Make new state
			ConvertState *tempState = malloc(sizeof(ConvertState));
			tempState->states = currentState->transitions[n];
			tempState->isNew = 1;
			LinkedList_add_at_end(states,tempState);

			//printf("While propagating, generated new set-state:\n");
			//IntSet_print(tempState->states);
		}
	}

	//Mark the state as no longer new
	currentState->isNew = 0;
}

//Finds the numerical ID based on the set ID
int find_id_for_Setid(LinkedList *list, IntSet *stateSet){

	LinkedListIterator *currentStatesIterator = LinkedList_iterator(list);
	int i = 0;
	while (LinkedListIterator_has_next(currentStatesIterator)){
		ConvertState *tempState = LinkedListIterator_next(currentStatesIterator);
		
		if (IntSet_equals(tempState->states,stateSet)){
			return i;
		}
		i++;

	}
	//If we found no id, use -1
	return -1;

}

//////////////////////////////////////////////
// End of File
//////////////////////////////////////////////