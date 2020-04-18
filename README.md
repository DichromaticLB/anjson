# anjson
c++ lib to parse json objects

### Prerequisites

The library is self contained, requiring only GNU flex & bison to build

### Installing

```
make 
sudo make install
```
by default installs headers to /usr/include and shared lib to /usr/lib.

### Usage

You can build a standalone executable for testing with some minor formating 

```
make exec
echo '{"test":["one",1,{"one":1}]}'| ./anjson '"this"["test"][2]' 
```
should output 
```
{
"one":1
}

```

Once installed
```
#include<anjson/anjson.hpp>

int main()
{
	using namespace anjson;
  	using namespace std;
  
	jsonobject json=fromString("{\"test\":[1,2,3,4],\"key\":{\"key2\":{}}}");
	jsonobject jsonFile=fromFile("test.json");//Load object from file
	jsonobject jsonStream=fromStream(cin);//Load object stdin

	cout<<json["test"]<<endl;
	cout<<json["test"].pretty()<<endl;
	cout<<json["key"]["key2"].pretty()<<endl;
	json["key"]["key2"]["newElement"]=string("string value");
	cout<<json["key"]["key2"].pretty()<<endl;
}
```
build with 
```
g++ -std=c++1z -lanjson -o jsontest jsontest.cpp && ./jsontest
```
should output 
```
array with 4 elements

[
0:1
1:2
2:3
3:4
]

{}


{
"newElement":"string value"
}
```

### license 
This project is licensed under the GPL2 License - see the LICENSE.txt file for details
