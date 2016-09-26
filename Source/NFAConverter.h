
#ifndef _nfaconverter_h_gf
#define _nfaconverter_h_gf

#include "NFA.h"
#include "DFA.h"
#include "IntSet.h"
#include "LinkedList.h"

typedef struct {
    IntSet *states;
    IntSet *transitions[NFA_NSYMBOLS];
    int isNew;
} ConvertState;

extern DFA* NFA_to_DFA(NFA *nfa);
extern int find_id_for_Setid(LinkedList *list, IntSet *stateSet);
extern void doStatePropagate(NFA *nfa, LinkedList *states, ConvertState *currentState);

#endif
