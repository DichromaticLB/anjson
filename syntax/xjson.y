%{
#include<string>
#include<cmath>
#include<fstream>
#include"anjson.hpp"
#include"syntools.hpp"
#define YYSTYPE anjson::variant
%}

%require "3.0.4"
%language "c++"
%define api.value.type {anjson::variant}
%define api.namespace {anjson} 
%define parser_class_name {anjsonAccessor}
%parse-param {anjLexer *lexer}
%parse-param {const variant &src}
%parse-param {variant &result}

%code{
#undef yylex
#define yylex lexer->ajlex
}

%token BOOL DIGITS EXP _NULL CHAR STRING
%left CHAR ',' ':' '-' EXP DIGITS
%%

input: elem {result=$1;}

elem: STRING {$$=src;}
	| elem '[' STRING ']' {$$=$1[$3.get<string>()];}
	| elem '[' DIGITS ']'  {$$=$1[stol($3.get<string>())];}


%%

void anjson::anjsonAccessor::error(const string& s){
	cerr<<s<<" line:"<<to_string(lexer->linecount)<<endl;
}