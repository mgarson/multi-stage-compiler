CC = g++
CFLAGS = -Wall -g

# Object files
OBJS = main.o scanner.o parser.o token.o symbolTable.o staticSemantic.o codeGen.o

# Target executable
TARGET = P4

# Build the final executable
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile main.o
main.o: main.cpp parser.h scanner.h token.h node.h symbolTable.h staticSemantic.h codeGen.h
	$(CC) $(CFLAGS) -c main.cpp

# Compile scanner.o
scanner.o: scanner.cpp scanner.h token.h
	$(CC) $(CFLAGS) -c scanner.cpp

# Compile parser.o
parser.o: parser.cpp parser.h scanner.h token.h node.h
	$(CC) $(CFLAGS) -c parser.cpp

# Compile token.o
token.o: token.cpp token.h
	$(CC) $(CFLAGS) -c token.cpp

#Compile symbolTable.o
symbolTable.o: symbolTable.cpp symbolTable.h
	$(CC) $(CFLAGS) -c symbolTable.cpp

#Compile staticSemantic.o
staticSemantic.o: staticSemantic.cpp staticSemantic.h
	$(CC) $(CFLAGS) -c staticSemantic.cpp

#Compile codeGen.o
codeGen.o: codeGen.cpp codeGen.h node.h symbolTable.h
	$(CC) $(CFLAGS) -c codeGen.cpp

# Clean up
clean:
	/bin/rm -f *.o $(TARGET)

