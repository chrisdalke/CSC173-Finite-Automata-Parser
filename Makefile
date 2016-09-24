Sources=Main.c
Executable=Project1

CFlags=-c -Wall -g -Iinc
LDFlags=
ObjectDir=Source/Bin/
SourceDir=Source/
BinDir=

CC=gcc
RM=rm

Objects=$(Sources:.c=.o)
CSources=$(addprefix $(SourceDir),$(Sources))
CObjects=$(addprefix $(ObjectDir),$(Objects))
CExecutable=$(addprefix $(BinDir),$(Executable))

all: $(CSources) $(CExecutable)

$(CExecutable): $(CObjects)
	$(CC) $(LDFlags) $(CObjects) -o $@

$(ObjectDir)%.o: $(SourceDir)%.c
	$(CC) $(CFlags) $< -o $@

clean:
	$(RM) $(CObjects)
	$(RM) $(CExecutable)