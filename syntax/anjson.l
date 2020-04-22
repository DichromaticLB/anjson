%{

#include<string>
#include<iostream>
using namespace std;
#include"anjson.hpp"
#include"syntools.hpp"
#define YYSTYPE anjson::variant
#define YY_DECL int anjLexer::ajlex(anjson::variant* yylval)

#include"anjson_parser.hpp"
using namespace anjson;

static char fastHex(char a,char b){
	a&=0xdf;
	b&=0xdf;
	static char table[]={
		0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15
	};
	return table[a-'0']*16+table[b-'0'];	
}

%}

%option noyywrap
%option nounput
%option c++
%x JSTRINGPARSE 

%%

false {*yylval=false;return anjsonParser::token_type::BOOL;}
true {*yylval=true;return anjsonParser::token_type::BOOL;}
null {yylval->release();return anjsonParser::token_type::_NULL; }

\" {BEGIN(JSTRINGPARSE);*yylval=string(""); }

0 {*yylval=string("0");return anjsonParser::token_type::DIGITS;}
[1-9][0-9]* {*yylval=string(yytext);return anjsonParser::token_type::DIGITS;}
0x([0-9a-fA-F]{1,16}) {*yylval=string(yytext);return anjsonParser::token_type::HEX;}
[eE]+? {*yylval=true;return anjsonParser::token_type::EXP;}

\n { linecount++;}
[ \t\r] {}



. {return yytext[0];}

<JSTRINGPARSE>{

\\\" {(*yylval).get<string>()+='"';}
\\\\ {(*yylval).get<string>()+='\\';}
\\b  {(*yylval).get<string>()+='\b';}
\\f  {(*yylval).get<string>()+='\f';}
\\n  {(*yylval).get<string>()+='\n';}
\\r  {(*yylval).get<string>()+='\r';}
\\t  {(*yylval).get<string>()+='\t';}
\\u([a-fA-F0-9]{4})  {
	(*yylval).get<string>()+=fastHex(yytext[2],yytext[3]);
	(*yylval).get<string>()+=fastHex(yytext[4],yytext[5]);
	}
\"   {BEGIN(INITIAL); return anjsonParser::token_type::STRING;}	
[^\x00-\x1F"\\]   {(*yylval).get<string>()+=yytext[0];}
. {
	auto errormsg=string("Failed to process char bulding string at line: ")+std::to_string(linecount);
	LexerError(errormsg.c_str());
	}
}

%%

/*Dummy*/
int anjFlexLexer::yylex(){
	return 0;
}


