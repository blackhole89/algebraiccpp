all: pp

.PHONY: all

pp: lex.o algtypes.o
	g++ -lboost_system -lboost_wave -lboost_thread -lboost_filesystem -o pp lex.o algtypes.o	

.cpp.o:
	g++ -c -o $@ $<
