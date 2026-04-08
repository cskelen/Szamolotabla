PROG = hazi
PROG_O = main.o table.o
PROG_H = table.h
PROG_L =

MTRACE_O = memtrace.o
MTRACE_H = memtrace.h

CXX = g++
CXXFLAGS = -pedantic -Wall -Werror
CXXFLAGS += -g -DMEMTRACE
LDFLAGS = -g

OBJS = $(PROG_O) $(MTRACE_O)
HEADS = $(PROG_H) $(MTRACE_H)
LIBS = $(PROG_L) 

.PHONY: all
all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

$(OBJS): $(HEADS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(PROG)


