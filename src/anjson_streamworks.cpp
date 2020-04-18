#include"syntools.hpp"
#include"anjson.hpp"
#include"anjson_parser.hpp"
#include<sstream>
#include<fstream>
using namespace anjson;

variant anjson::fromStream(istream& i){
	variant res;
	anjLexer l(i);
	anjsonParser p(&l,&res);
	p.parse();
	return res;
}

variant anjson::fromFile(const string& filename){
	variant res;
	ifstream i(filename);
	if(i.is_open()){
		return fromStream(i);
	}
	return res;

}

variant anjson::fromString(const string&src){
	stringstream ss(src);
	return fromStream(ss);
}
