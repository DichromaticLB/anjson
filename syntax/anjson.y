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
%define parser_class_name {anjsonParser}
%parse-param {anjLexer *lexer}
%parse-param {variant *result}

%code{
#undef yylex
#define yylex lexer->ajlex
}

%token BOOL DIGITS EXP _NULL CHAR STRING
%left CHAR ',' ':' '-' EXP DIGITS
%%

input: %empty
	| value {*result=$1;}
	
object:  '{'  '}' {$$=variant::mapType();}
    | '{' members '}' {$$=$2;}
	

members: STRING ':' value {$$=variant::mapType();$$.get<variant::mapType>()[$1.get<string>()]=$3;}
	| members ',' STRING ':' value {$$=$1;$$.get<variant::mapType>()[$3.get<string>()]=$5;}


value: STRING 
	| number
	| object
	| array
	| BOOL
	| _NULL
	
number: DIGITS { $$=(uint64_t)stoull($1.get<string>());}
	| DIGITS '.' DIGITS {$$=stod($1.get<string>()+"."+$3.get<string>());}
	| number EXP DIGITS {$$=$1.doubleCast()*pow(10,stoi($3.get<string>()));}
	| number EXP '-' DIGITS {$$=$1.doubleCast()*pow(10,-stoll($4.get<string>()));}
	| '-' number {
			if($2.getType()==type::integer)
				$$=-$2.get<int64_t>();
			else
				$$=-$2.get<double>();
	}
	
array: '[' ']' {$$=variant::arrayType();}
	| '[' elements ']' {$$=$2.get<variant::arrayType>();}

elements: value {$$=variant::arrayType();$$.get<variant::arrayType>().emplace_back($1);}
	| elements ',' value {$$=$1;$$.get<variant::arrayType>().push_back($3);}

		
%%

void anjson::anjsonParser::error(const string& s){
	cerr<<s<<" line:"<<to_string(lexer->linecount)<<endl;
}