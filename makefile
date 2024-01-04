# Directories
export BINDIR = $(CURDIR)/bin
export LIBDIR = $(CURDIR)/lib
export INCDIR = $(CURDIR)/include
export SRCDIR = $(CURDIR)/src/producerconsumer

# files
export C_LIBSRC = $(wildcard $(INCDIR)/*.c)
export S_LIBSRC = $(wildcard $(INCDIR)/*.s)
export C_BINSRC = $(wildcard $(SRCDIR)/*.c)
export S_BINSRC = $(wildcard $(SRCDIR)/*.a)


#Mutex type
mutex_type = 1

# GCC flags
export CFLAGS = -g -I INCDIR
export CFLAGS += -Dmutex_type=$(mutex_type)
export CFLAGS += -std=c99 -m32
export CFLAGS += -pedantic-errors -Wall -Wextra -Werror
export CFLAGS += -Wlogical-op -Wredundant-decls -g
export ASMFLAGS = -Wa,--divide,--warn

.PHONY: clean

#Create bindir and libdir
mkdir: clean
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)

compLibrary: mkdir
	cd $(INCDIR) && make all

compExec: compLibrary
	gcc $(ASMFLAGS) $(CFLAGS) $(C_BINSRC) $(S_BINSRC) -o $(BINDIR)/main -lstack -I $(INCDIR) -L $(LIBDIR)
	
clean: 
	rm -rf $(BINDIR)
	rm -rf $(LIBDIR)
	rm -rf $(INCDIR)/*.o
