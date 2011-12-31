# Makefile for "vecmath" library

PROJ = libvecmath
OUT = lib

CXX = g++
CXXFLAGS = -c -Wall -I include

HEADERS = $(wildcard *.h)
SOURCES = $(wildcard *.cpp)

all: objects
	ar -cr $(PROJ).a *.o

objects: $(SOURCES)
	$(CXX) $(CXXFLAGS) $?

clean:
	rm -f *.o
	rm -f $(OUT)/$(PROJ).a
