# add printing macros for debugging messages:
# usage:
# //dbug "message"/variable eol;
# additional information printed:
# - file name the debug message is coming from,
# - function name the debug message is coming from,
# - line number in that file the debug message is coming from

MACROS    = -D'dvar(x)=\#x<<" = "<<x'

# MACROS   += -DWAIT='std::cin.get()'
MACROS   += -DWAIT=''

MACROS   += -Ddbug='std::cout<<__FILE__<<":"<<__FUNCTION__<<"-"<<__LINE__<<":							"<<'

MACROS   += -Deol='<<"\n"<<"\n"'
# MACROS   += -Deol='<<std::endl<<__PRETTY_FUNCTION__<<std::endl<<std::endl'

CC = g++
SRCS = $(wildcard *.cpp)
SRCS += $(wildcard vecmath/src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
PROG = a4
CFLAGS = -O2 -Wall -Wextra $(MACROS) -std=c++11
INCFLAGS = -Ivecmath/include

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

clean:
	rm -f *.bak vecmath/src/*.o *.o core.* $(PROG)
