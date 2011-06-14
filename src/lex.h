#include <string.h>

#ifndef _LEX_H_
#define _LEX_H_

void start_lexing(char *f);
bool lex();
int get_tid();
std::string get_tval();

#endif
