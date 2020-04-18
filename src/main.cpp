#include"anjson.hpp"
#include<iostream>
#include <unistd.h>

void printhelp(){
	using namespace std;
	cout<<"Usage: anjson [-f filename] [queries]"<<endl;
	cout<<"Parse a json file and navigate it, if no file is provided read json objects from stdin"<<endl;
	cout<<"Example: anjson -f test.json '\"this\"[\"key\"]'"<<endl;

}

int main(int argc,char **argv) {
	using namespace anjson;
	using namespace std;

	string file;
	string query;

	int opt;
	while((opt=getopt(argc, argv, "f:h")) != -1){
		switch(opt){
			case 'f':
				file=optarg;
				break;
			case 'h':
				printhelp();
				return 0;
		}
	}

	variant v;
	if(file.empty())
		v=fromStream(cin);
	else
		v=fromFile(file);

	for(;optind< argc;optind++)
		cout<<v.query(argv[optind]).pretty();


	return 0;
}
