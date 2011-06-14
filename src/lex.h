#include <string.h>

#ifndef _LEX_H_
#define _LEX_H_

void start_lexing(char *f);
bool lex();
void lex_save();
void lex_rewind();
int get_tid();
std::string get_tval();

#endif
