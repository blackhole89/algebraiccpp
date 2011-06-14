#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "lex.h"

#define OUTF stdout

class CConstructor;

class CType {
public:
	std::string name;
	std::vector<CConstructor*> constrs;
	std::vector<std::string> typeargs;

	void emit_template_short(FILE *fl);
	void emit_template_long(FILE *fl);

	void emit_declaration(FILE *fl);
	void emit_definition(FILE *fl);
	void emit_implementation(FILE *fl);
	void emit_identifier(FILE *fl);
};

std::map<std::string,CType*> types;
std::map<std::string,CConstructor*> constructors;

class CConstructor {
public:
	std::string name;
	CType *def;
	std::vector<std::string> params;
	std::vector<std::string> params_canonical;

	void emit_structure(FILE *fl) {
		fprintf(fl,"struct {\n");
		for(int i=0;i<params.size();++i) {
			if(types.count(params_canonical[i])) {
				fprintf(fl,"_minimal_ptr<%s > p%d;\n",params[i].c_str(),i);
			} else fprintf(fl,"%s p%d;\n",params[i].c_str(),i);
		}
		fprintf(fl,"} s%s;\n",name.c_str());
	}

	void emit_cstub(FILE *fl) {
/*		fprintf(fl,"static %s *ctor_%s(",def->name.c_str(),name.c_str());
		for(int i=0;i<params.size();++i) {
			if(types.count(params_canonical[i])) {
				fprintf(fl,"_alg_ptr<%s > &pp%d",params[i].c_str(),i);
			} else fprintf(fl,"%s *pp%d",params[i].c_str(),i);
			if(i!=params.size()-1) fprintf(fl,",");
		}
		fprintf(fl,");\n");*/
	}

	void emit_constructor(FILE *fl) {
		def->emit_template_long(fl);
		fprintf(fl,"%s",def->name.c_str());
		def->emit_template_short(fl);
		fprintf(fl," *"/*,def->name.c_str()*/);
//		def->emit_template_short(fl);
		fprintf(fl,"ctor_%s(",name.c_str());
		for(int i=0;i<params.size();++i) {
			if(types.count(params_canonical[i])) {
				fprintf(fl,"_alg_ptr<%s > const &pp%d",params[i].c_str(),i);
			} else fprintf(fl,"%s pp%d",params[i].c_str(),i);
			if(i!=params.size()-1) fprintf(fl,",");
		}
		fprintf(fl,") {\n");
		fprintf(fl,"%s",def->name.c_str());
		def->emit_template_short(fl);
		fprintf(fl,"*r=new %s",def->name.c_str());
		def->emit_template_short(fl);
		fprintf(fl,";\nr->_discr=%s;\n",name.c_str());
		for(int i=0;i<params.size();++i) {
			fprintf(fl,"r->s%s.p%d=pp%d;\n",name.c_str(),i,i);
			if(types.count(params_canonical[i])) fprintf(fl,"r->s%s.p%d._alg_ref();\n",name.c_str(),i);
		}
		fprintf(fl,"return r;\n}\n");
	}

	void emit_destructor(FILE *fl) {
		for(int i=0;i<params.size();++i) {
			if(types.count(params_canonical[i])) fprintf(fl,"s%s.p%d._alg_unref();\n",name.c_str(),i);
		}
	}

	void preemit_invocation(FILE *fl) {
		fprintf(fl,"_alg_ptr<%s >(%s::%s",def->name.c_str(),def->name.c_str(),name.c_str());
	}

	void postemit_invocation(FILE *fl) {
		fprintf(fl,"))");
	}
};

void CType::emit_template_long(FILE *fl) {
	if(typeargs.size()) {
		fprintf(fl,"template<");
		for(std::vector<std::string>::iterator i=typeargs.begin();i!=typeargs.end();) {
			fprintf(fl,"class %s",(*i).c_str());
			++i;
			if(i!=typeargs.end()) fprintf(fl,",");
		}
		fprintf(fl,"> ");
	}
}

void CType::emit_template_short(FILE *fl) {
	if(typeargs.size()) {
		fprintf(fl,"<");
		for(std::vector<std::string>::iterator i=typeargs.begin();i!=typeargs.end();) {
			fprintf(fl,"%s",(*i).c_str());
			++i;
			if(i!=typeargs.end()) fprintf(fl,",");
		}
		fprintf(fl,"> ");
	}
}

void CType::emit_declaration(FILE *fl) {
	emit_template_long(fl);
	fprintf(fl,"class %s;\n",name.c_str());
}

void CType::emit_definition(FILE *fl) {
	const char *cname=name.c_str();
	fprintf(fl,"typedef enum { ");
	for(std::vector<CConstructor*>::iterator i=constrs.begin();i!=constrs.end();++i) {
		if(i==constrs.begin()) fprintf(fl,"%s=1, ",(*i)->name.c_str());
		else fprintf(fl,"%s, ",(*i)->name.c_str());
	}
	fprintf(fl,"_%s_discr_max } _%s_discr;\n",cname,cname);

	emit_template_long(fl);

	fprintf(fl,"class %s : public _alg_tracked<%s",cname,cname);
	emit_template_short(fl);
	fprintf(fl,"> {\npublic:\n");
	fprintf(fl,"_%s_discr _discr;\n",cname);
	fprintf(fl,"union {\n");
	for(std::vector<CConstructor*>::iterator i=constrs.begin();i!=constrs.end();++i) {
		(*i)->emit_structure(fl);
	}
	fprintf(fl,"};\n");
	for(std::vector<CConstructor*>::iterator i=constrs.begin();i!=constrs.end();++i) {
		(*i)->emit_cstub(fl);
	}
	fprintf(fl,"~%s() {\n",cname);
	fprintf(fl,"switch(_discr) {\n");
	for(std::vector<CConstructor*>::iterator i=constrs.begin();i!=constrs.end();++i) {
		fprintf(fl,"case %s:\n",(*i)->name.c_str());
		(*i)->emit_destructor(fl);
		fprintf(fl,"break;\n");
	}
	fprintf(fl,"}\n}\n");
	fprintf(fl,"};\n");
}

void CType::emit_implementation(FILE *fl) {
	for(std::vector<CConstructor*>::iterator i=constrs.begin();i!=constrs.end();++i) {
		(*i)->emit_constructor(fl);
	}
}

void CType::emit_identifier(FILE *fl) {
	fprintf(fl,"_alg_ptr<%s >",name.c_str());
}

/* *** */

void error(char *e)
{
	printf("ERROR: %s (Last token was %s)\n",e,get_tval().c_str());
	exit(-2);
}

inline bool is_space(int t)
{
	return (t&0xFF000000)==0xD0000000 || (t&0xFF000000)==0xB0000000;
}

inline bool is_literal(int t)
{
	return (t&0xFF000000)==0x40000000;
}

inline bool is_identifier(int t)
{
	return (t&0xFF000000)==0x10000000;
}

inline bool is_keyword(int t)
{
	return (t&0xFF000000)==0x20000000;
}

void try_lex()
{
	do {
		if(!lex()) {
			error("Unexpected EOF.");
		}
	} while(is_space(get_tid())); // eat whitespace
}

/* *** */
int uniq=0;

class CMatch {
public:
	bool first;
	CMatch() {first=true;}

	std::vector<std::string> inputs;

	void try_unify(std::string lhs,
		       bool evolve, // type hack
		       std::vector<std::pair<int,std::string> >::iterator end,
		       std::vector<std::pair<int,std::string> >::iterator &i,std::map<std::string,std::string> *b,FILE *fl) {
//		printf("UNIF(%s,%s)\n",lhs.c_str(),(*i).second.c_str());
		if(is_literal((*i).first)) {
			fprintf(fl,"&& ((%s)==(%s))",lhs.c_str(),(*i).second.c_str());
		} else {
			if(constructors.count((*i).second)) {
				CConstructor *c=constructors[(*i).second];
				fprintf(fl,"&& ((%s.ptr->_discr)==(%s))",lhs.c_str(),c->name.c_str());
				++i; 
				if(i==end||(*i).second==","||(*i).second==")") return;
				++i;
				int k=0;
				for(;(*i).second!=")";) {
					if((*i).second==",") { ++k; ++i; }
					else {
						char buf[16];
						sprintf(buf,"%d",k);
						try_unify(lhs+".ptr->s"+c->name+".p"+buf,types.count(c->params_canonical[k]),end,i,b,fl);
					}
				}
				++i;
			} else {
				if(b->count((*i).second)) {
					fprintf(fl,"&& ((%s)==(%s))",lhs.c_str(),(*b)[(*i).second].c_str()); // check binding is consistent
				} else if((*i).second=="*") {
				} else if(is_identifier((*i).first)) {
					(*b)[(*i).second]=lhs+(evolve?".evolve()":"");
				} else {
					fprintf(fl,"&& ((%s)==(%s))",lhs.c_str(),(*i).second.c_str());
				}
				++i;
			}
		}
	}

	void emit_case(std::vector<std::pair<int,std::string> > pattern,FILE *fl) {
		std::map<std::string, std::string> binding;
		if(!first) fprintf(fl," else ");
		first=false;
		fprintf(fl,"if(true");

		int j=0;
		
		for(std::vector<std::pair<int,std::string> >::iterator i=pattern.begin();i!=pattern.end();) {
			if((*i).second==",") { ++j; ++i; }
			else try_unify(inputs[j],false,pattern.end(),i,&binding,fl);
		}

		fprintf(fl,") {\n");
		for(std::map<std::string,std::string>::iterator i=binding.begin();i!=binding.end();++i) {
			fprintf(fl,"auto %s=%s;\n",(*i).first.c_str(),(*i).second.c_str());
		}
	}
	void emit_caseend(FILE *fl) {
		fprintf(fl,"}\n");
	}
};

/* *** */

CConstructor *do_read_constructor(CType *t)
{
	CConstructor *c;
	if(get_tval()==";" || get_tval()=="and") return NULL;
	c=new CConstructor;
	c->def=t;
	c->name=get_tval();
	constructors[c->name]=c;
	try_lex();

	if(get_tval()!="|" && get_tval()!="and" && get_tval()!=";") {
		if(get_tval()!="(") error("Expected ( after constructor name.");
		do {
			try_lex();
			std::string s=get_tval();
			c->params_canonical.push_back(s);
			try_lex();
			while(get_tval()!="," && get_tval()!=")") {
				s+=get_tval();
				lex();
			}
			c->params.push_back(s);
		} while(get_tval()==",");
		if(get_tval()!=")") error("Expected ) at end of constructor argument list.");
		try_lex();
	}
	if(get_tval()=="|") try_lex();

	return c;
	
}

void do_read_datatype()
{
	CType *t=new CType;
	t->name=get_tval();
	types[t->name]=t;
	try_lex();

	if(get_tval()=="<") {
		do {
			try_lex();
			t->typeargs.push_back(get_tval());
			try_lex();
		} while(get_tval()==",");
		if(get_tval()!=">") error("Expected > at end of type argument list.");
		try_lex();
	}

	if(get_tval()!="=") error("Expecting = after datatype name.");
	try_lex();

	CConstructor *c;
	while(c=do_read_constructor(t)) t->constrs.push_back(c);
	
	t->emit_declaration(OUTF);
	
	if(get_tval()=="and") {
		try_lex();
		do_read_datatype();
	}
	
	t->emit_definition(OUTF);
	t->emit_implementation(OUTF);

	if(get_tval()!=";") error("Datatype declaration must end in ;.");

//	try_lex();
}

void do_read_type()
{
	fprintf(OUTF,"_alg_ptr<");
	fprintf(OUTF,get_tval().c_str());
	try_lex();
	if(get_tval()=="<") {
		int d=1;
		fprintf(OUTF,"<");
		do{
			try_lex();
			if(get_tval()=="<") ++d;
			else if(get_tval()==">") --d;
			fprintf(OUTF,get_tval().c_str());
		}while(d);
		try_lex();
	}
	fprintf(OUTF," >");
}

bool step();

void do_read_ctor()
{
	std::string nm=get_tval();
	fprintf(OUTF,"_algebraize(ctor_%s",nm.c_str());
	try_lex();
	if(get_tval()=="<") {
		int d=1;
		fprintf(OUTF,"<");
		do{
			try_lex();
			if(get_tval()=="<") ++d;
			else if(get_tval()==">") --d;
			fprintf(OUTF,get_tval().c_str());
		}while(d);
		try_lex();
	}
	if(!constructors[nm]->params.size()) fprintf(OUTF,"())");
	else {
		if(get_tval()=="(") {
			int d=0;
			do{
				if(get_tval()=="(") {
					++d;
					fprintf(OUTF,"(");
					try_lex();
				} else if(get_tval()==")") {
					--d;
					fprintf(OUTF,")");
					try_lex();
				} else {
					step();
					while(is_space(get_tid())) step();
				}
			} while(d);
		}
		fprintf(OUTF,")");
	}
}

/* *** */

int freshgen=0;

void pass_closed_expression()
{
	int d=0;
	do{
		if(get_tval()=="(") {
			++d;
			fprintf(OUTF,"(");
			try_lex();
		} else if(get_tval()==")") {
			--d;
			fprintf(OUTF,")");
			try_lex();
		} else {
			step();
			while(is_space(get_tid())) step();
		}
	} while(d || (get_tval()!="," && get_tval()!=")"));
}

void pass_case_or_scope()
{
	int d=0;
	do{
		if(get_tval()=="{") {
			++d;
			fprintf(OUTF,"{");
			try_lex();
		} else if(get_tval()=="}") {
			--d;
			fprintf(OUTF,"}");
			try_lex();
		} else {
			step();
			while(is_space(get_tid())) step();
		}
	} while(d || (get_tval()!="case" && get_tval()!="}"));

}

void do_read_match()
{
	CMatch m;

	if(get_tval()!="(") error("Expected ( after match.");
	try_lex();
	while(get_tval()!=")") {
		char buf[16];
		sprintf(buf,"_fv%d",freshgen);
		m.inputs.push_back(buf);
		fprintf(OUTF,"auto _fv%d=",freshgen);
		++freshgen;
		pass_closed_expression();
		fprintf(OUTF,";\n");
		if(get_tval()==",") try_lex();
	}
	try_lex();
	
	if(get_tval()!="{") error("Missing match body.");
	try_lex();

	while(get_tval()!="}") {
		if(get_tval()!="case") error("Expecting case.");
		try_lex();
		
		std::vector<std::pair<int,std::string> > pat;
		while(get_tval()!=":") {
			pat.push_back(std::pair<int,std::string>(get_tid(),get_tval()));
			try_lex();
		}
		try_lex();
		
		m.emit_case(pat,OUTF);
		if(get_tval()!="}" && get_tval()!="case") pass_case_or_scope();
		m.emit_caseend(OUTF);
	}
	try_lex();
}

void do_read_matchfun()
{
	CMatch m;

	while(get_tval()!="(") {
		if(types.count(get_tval())) {
			do_read_type();
		} else {
			fprintf(OUTF,"%s",get_tval().c_str());
			lex();
		}
	}
	while(get_tval()!=")") {
		if(get_tval()==",") {
			/* emit parameter */
			char buf[16];
			fprintf(OUTF," _p%d",freshgen);
			sprintf(buf,"_p%d",freshgen);
			m.inputs.push_back(buf);
			++freshgen;
		}
		if(types.count(get_tval())) {
			do_read_type();
		} else {
			fprintf(OUTF,"%s",get_tval().c_str());
			lex();
		}
	}
	fprintf(OUTF," _pl) {\n");
	m.inputs.push_back("_pl");

	do {
		try_lex();
		while(get_tval()!="(") try_lex();
		try_lex();

		std::vector<std::pair<int,std::string> > pat;

		int d=0;
		while(d || get_tval()!=")") {
			if(get_tval()=="(") ++d;
			else if(get_tval()==")") --d;
			pat.push_back(std::pair<int,std::string>(get_tid(),get_tval()));
			try_lex();
		}
		try_lex();

		if(get_tval()!="{") error("Expected { in matching function.");
		try_lex();
		
		m.emit_case(pat,OUTF);
		if(get_tval()!="}") pass_case_or_scope();
		m.emit_caseend(OUTF);

		try_lex();
	} while(get_tval()=="|");

	fprintf(OUTF,"}\n\n");
}

/* *** */

bool maybe_function=true;
bool maybe_acceptablescope=false;
int d=0,d_limit=0;
std::string last_token;

bool step()
{
	if(get_tval()=="datatype") {
		try_lex();
		do_read_datatype();
		lex_save();
		try_lex();
	} else if(get_tval()=="match") {
		try_lex();
		do_read_match();
		lex_save();
	} else if(types.count(get_tval())) {
		do_read_type();
	} else if(constructors.count(get_tval())) {
		do_read_ctor();
	} else if(d<=d_limit && (get_tval()=="namespace" || get_tval()=="class" || get_tval()=="struct" || get_tval()=="=")) {
		maybe_acceptablescope=true;
		d_limit++;
		fprintf(OUTF,"%s",get_tval().c_str());
		return lex();
	} else if(maybe_function && is_identifier(get_tid()) && d<=d_limit && last_token==")") {
		/* assume matching function */
		fprintf(OUTF,";\n");
		lex_rewind(); // go back to last } or ;
		lex();
		while(!is_identifier(get_tid()) && !is_keyword(get_tid())) lex();
		maybe_function=false; //prevent nasty recursion
		do_read_matchfun();
		last_token="";
		maybe_function=true;
	} else {
		if(get_tval()=="{") {
			maybe_acceptablescope=false;
			++d;
		} else if(get_tval()=="}") {
			lex_save();
			--d;
			if(d<d_limit) --d_limit; // leave a "good" scope (namespace, class, struct)
		} else if(get_tval()==";") {
			lex_save();
			if(maybe_acceptablescope) {
				maybe_acceptablescope=false;
				--d_limit; //class or struct was just a declaration
			}
		}
		fprintf(OUTF,"%s",get_tval().c_str());
		if(!is_space(get_tid())) last_token=get_tval();
		return lex();
	}
	return true;
}

int main(int argc,char* argv[])
{
	if(argc<2) return -1;
	start_lexing(argv[1]);

	lex();
	while(step());

	return 0;
}
