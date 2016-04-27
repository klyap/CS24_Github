OBJS = c_except.o my_setjmp.o ptr_vector.o divider.o
# .o files needed to run tests on my set jump
OBS2 = c_except.o my_setjmp.o ptr_vector.o test_setjmp.o
CC = gcc
CFLAGS=-g -Wall -Werror
ASFLAGS=-g

# Detect if the OS is 64 bits.  If so, request 32-bit builds.
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
  CFLAGS += -m32
  ASFLAGS += -32
endif


divider : $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

ptr_vector.c : ptr_vector.h
c_except.c : c_except.h my_setjmp.h
divider.c : c_except.h my_setjmp.h
# Dependencies for test file
test_setjmp.c : c_except.h my_setjmp.h 

# Added check rule for test file
check : $(OBS2)
	$(CC) $(LDFLAGS) $^ -o test_setjmp
	

clean :
	rm -f *.o *~ divider divider.exe

