#ifndef INCLUDE_SYNTOOLS_HPP_
#define INCLUDE_SYNTOOLS_HPP_

#define yyFlexLexer anjFlexLexer

#ifndef yyFlexLexerOnce
#include <FlexLexer.h>
#endif
#include"anjson.hpp"

#include<iostream>

class anjLexer:public anjFlexLexer{
public:
	anjLexer(std::istream& i):anjFlexLexer(&i),datasrc(i),linecount(1){}
	int ajlex(anjson::variant* v);

	std::istream &datasrc;
	int linecount;
};



#endif
