CC = g++
CFLAGS = -g -Wall -lpthread -std=c++11
CCLINK = $(CC)
OBJS = bank.o thread.o  list.o 
RM = rm -f
# Creating the  executable
bank: $(OBJS)
	$(CCLINK) $(CFLAGS) -o bank $(OBJS)
# Creating the object files
thread_func.o: thread.cpp thread.h
bank.o: bank.cpp thread.h
list.o: list.cpp list.h

# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*
