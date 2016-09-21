# CSC 173 Project 1

PROGRAMS = FiniteAutomata

CFLAGS = -g

programs: $(PROGRAMS)

FiniteAutomata: FiniteAutomata

clean:
	-rm $(PROGRAMS) *.o
	-rm -r *.dSYM
