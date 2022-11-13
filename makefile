###########################################################
#		                                                  #
# Simple Makefile for Operating Systems Project 4         #
# Banker's Algorithm                                      #
#                                                         #
# note: this requires 'module load gcc' has been executed #
# in order to compile correctly using gcc 8.4.0 or later  #
#                                                         #
###########################################################
.SUFFIXES: .h .c .cpp .l .o

banker: Matrix.h System.h banker.o
	g++ -std=c++1y System.h Matrix.h banker.o -o banker

banker.o: banker.cpp
	g++ -std=c++1y -c banker.cpp	

clean:
	/bin/rm -f *.o *~ core banker


