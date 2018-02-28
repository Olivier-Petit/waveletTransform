

CC     = g++
OPT    = -O3
DEBUG  = -g
OTHER  = -Wall
CFLAGS = $(OPT) $(OTHER) -Wno-deprecated
# CFLAGS = $(DEBUG) $(OTHER)

MODULE = run
SRCS = Wavelet.cpp Sequencer.cpp Memory.cpp WaveletProc.cpp TestGene.cpp sc_main.cpp
OBJS = $(SRCS:.cpp=.o)

include ./Makefile.defs
