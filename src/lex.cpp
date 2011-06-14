#include <string>
#include <fstream>
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>

using namespace boost;

std::ifstream *i;
std::string *is;

typedef wave::cpplexer::lex_token<> token_t;
typedef wave::cpplexer::lex_iterator<token_t > iter_t;
typedef token_t::position_type pos_t;

iter_t c, end;

pos_t pos;

int tok_id;
char* tok_val;

void start_lexing(char *f)
{
	i=new std::ifstream(f);
	is=new std::string(std::istreambuf_iterator<char>(i->rdbuf()),
			std::istreambuf_iterator<char>());

	pos=pos_t(f);

	c = iter_t(is->begin(), is->end(), pos, wave::language_support(wave::support_cpp) );
	end=iter_t();

/*	for(;c!=end;++c) {
		std::cout << wave::get_token_name(*c) << "=" << (*c).get_value() << std::endl;
	}
*/
}

bool lex()
{
	try {
		if(c==end) return false;
		tok_id=(wave::token_id)(*c);
		tok_val=(char*)c->get_value().c_str();
		++c;
	} catch(wave::cpplexer::lexing_exception const& e) {
		std::cerr << e.file_name() << "(" << e.line_no() << "): " << e.description() << std::endl;
		return false;
	}
	return true;
}

int get_tid()
{
	return tok_id;
}

std::string get_tval()
{
	return std::string(tok_val);
}

