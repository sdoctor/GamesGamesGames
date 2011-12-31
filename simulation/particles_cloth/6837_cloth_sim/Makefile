INCFLAGS  = -I /mit/6.837/public/include/vecmath
INCFLAGS += -I /usr/include/GL


LINKFLAGS = -lglut -lGL -lGLU
LINKFLAGS += -L /mit/6.837/public/lib -lvecmath

#CFLAGS    = -O2 -Wall -ansi #optimized compiler commands
CFLAGS    = -g -Wall -Wextra -Wno-unused -ansi -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC #debugging compiler commands
CC        = g++ 
SRCS      = integrator.cpp particleSystem.cpp simpleSystem.cpp pendulumSystem.cpp camera.cpp main.cpp spring.cpp chainSystem.cpp clothSystem.cpp
OBJS      = $(SRCS:.cpp=.o)
PROG      = a3

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)