CC = g++
CFLAGS = -Wall -g

# Object files
OBJS = main.o scanner.o testScanner.o token.o

# Target executable
TARGET = P1

# Build the final executable
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

# Compile main.o
main.o: main.cpp testScanner.h
	$(CC) $(CFLAGS) -c main.cpp

# Compile scanner.o
scanner.o: scanner.cpp scanner.h token.h
	$(CC) $(CFLAGS) -c scanner.cpp

# Compile testScanner.o
testScanner.o: testScanner.cpp testScanner.h scanner.h
	$(CC) $(CFLAGS) -c testScanner.cpp

# Compile token.o
token.o: token.cpp token.h
	$(CC) $(CFLAGS) -c token.cpp

# Clean up
clean: 
	/bin/rm -f *.o $(TARGET)

