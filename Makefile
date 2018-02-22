

CC     = g++
OPT    = -O3
DEBUG  = -g
OTHER  = -Wall
CFLAGS = $(OPT) $(OTHER) -Wno-deprecated
# CFLAGS = $(DEBUG) $(OTHER)

MODULE = run
SRCS = Wavelet.cpp
OBJS = $(SRCS:.cpp=.o)

include ./Makefile.defs
