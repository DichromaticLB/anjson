#include"anjson.hpp"
#include<iostream>

void anjson::variant::release(){
	switch(p){
		case type::nulltype:
			break;
		case type::object:
			catRel<mapType>();
			break;
		case type::string:
			catRel<string>();
			break;
		case type::fp:
			catRel<double>();
			break;
		case type::integer:
			catRel<int64_t>();
			break;
		case type::array:
			catRel<arrayType>();
			break;
		case type::booltype:
			catRel<bool>();
			break;
	}
}

double anjson::variant::doubleCast()const{
	if(p==type::fp)
		return get<double>();

	return get<uint64_t>();
}

std::string anjson::typestr(anjson::type t){
	switch(t){
		case type::object:
			return "object";
		case type::string:
			return "string";
		case type::fp:
			return "number";
		case type::array:
			return "array";
		case type::booltype:
			return "bool";
		default:
			return "null";
	}
}

anjson::variant::~variant(){
	release();
}

anjson::variant::variant(anjson::variant&&t){
	p=t.p;
	data=t.data;
	t.p=type::nulltype;
	t.data=nullptr;
}

anjson::variant& anjson::variant::operator=(anjson::variant&&t){
	p=t.p;
	data=t.data;
	t.p=type::nulltype;
	t.data=nullptr;
	return *this;
}

anjson::variant::variant(){
	data=nullptr;
	p=type::nulltype;
}

anjson::variant::variant(const anjson::variant&o):variant(){
	(*this)=o;
}

anjson::variant& anjson::variant::operator=(const variant&t){
	switch(t.p){
		case type::nulltype:
			release();
			break;
		case type::object:
			(*this)=t.get<mapType>();
			break;
		case type::string:
			(*this)=t.get<string>();
			break;
		case type::fp:
			(*this)=t.get<double>();
			break;
		case type::integer:
			(*this)=t.get<int64_t>();
			break;
		case type::array:
			(*this)=t.get<arrayType>();
			break;
		case type::booltype:
			(*this)=t.get<bool>();
			break;
	}
	return *this;
}

anjson::variant& anjson::variant::operator=(const string& s){
	set<string,type::string>(s);
	return *this;
}

anjson::variant& anjson::variant::operator=(const anjson::variant::arrayType&ar){
	set<arrayType,type::array>(ar);
	return *this;
}

anjson::variant& anjson::variant::operator=(const anjson::variant::mapType&ar){
	set<mapType,type::object>(ar);
	return *this;
}

anjson::variant& anjson::variant::operator=(double d){
	set<double,type::fp>(d);
	return *this;
}

anjson::variant& anjson::variant::operator=(uint64_t i){
	return (*this)=(int64_t)i;
}

anjson::variant& anjson::variant::operator=(int64_t i){
	set<int64_t,type::integer>(i);
	return *this;
}

anjson::variant& anjson::variant::operator=(bool b){
	set<bool,type::booltype>(b);
	return *this;
}

anjson::type anjson::variant::getType() const{
	return p;
}

std::ostream& anjson::operator<<(ostream&o,const variant&v){
	switch(v.getType()){
		case type::nulltype:
			o<<"null";
			break;
		case type::object:
			o<<"object with "<<v.get<variant::mapType>().size()<<" elements";
			break;
		case type::string:
			o<<"string: "<<v.get<string>();
			break;
		case type::fp:
			o<<"floating point: "<<v.get<double>();
			break;
		case type::integer:
			o<<"integer: "<<v.get<int64_t>();
			break;
		case type::array:
			o<<"array with "<<v.get<variant::arrayType>().size()<<" elements";
			break;
		case type::booltype:
			o<<"bool("<<v.get<double>()<<")";
			break;
	}
	return o;
}

static void pidentate(std::ostream&o,const anjson::variant&v,uint32_t indentationLevel,bool skip=false){
	using namespace anjson;
	string opad(indentationLevel,'\t');

	switch(v.getType()){
		case type::nulltype:
			o<<"null"<<endl;
			break;
		case type::object:
			if(v.get<variant::mapType>().size()==0){
				o<<"{}"<<endl;
				break;
			}

			o<<endl<<opad<<"{"<<endl;
			for(auto& vv:v.get<variant::mapType>()){
				o<<opad<<"\""<<vv.first<<"\""<<":";
				pidentate(o,vv.second,indentationLevel+1);
			}
			o<<opad<<"}"<<endl;
			break;
		case type::string:
			o<<"\""<<v.get<string>()<<"\""<<endl;
			break;
		case type::fp:
			o<<v.get<double>()<<endl;
			break;
		case type::integer:
			o<<v.get<int64_t>()<<endl;
			break;
		case type::array:
		{
			if(v.get<variant::arrayType>().size()==0){
				o<<"[]"<<endl;
				break;
			}
			o<<endl<<opad<<"["<<endl;
			unsigned it=0;
			for(auto& vv:v.get<variant::arrayType>()){
				o<<opad<<(it++)<<":";
				pidentate(o,vv,indentationLevel+1);
			}
			o<<opad<<"]"<<endl;
			break;
		}
		case type::booltype:
			o<<v.get<bool>()<<endl;
			break;
	}
}

std::ostream& anjson::operator<<(ostream&o,const anjson::pretty_variant&v){
	pidentate(o,v.v,0);
	return o;
}

const anjson::variant& anjson::variant::operator[](const string& key) const{
	switch(p){
		case type::object:
			return get<mapType>().at(key);
		case type::array:
			return get<arrayType>()[std::stol(key)];
		default:
			throw runtime_error("trying to access element of type "
					+typestr()+" with key:"+key);
	}
}

anjson::variant& anjson::variant::operator[](const string& key){
	switch(p){
		case type::object:
			return get<mapType>()[(key)];
		case type::array:
			return get<arrayType>()[std::stol(key)];
		default:
			throw runtime_error("trying to access element of type "
					+typestr()+" with key:"+key);
	}
}


const anjson::variant& anjson::variant::operator[](uint32_t index) const{
	switch(p){
		case type::object:
			return get<mapType>().at(to_string(index));
		case type::array:
			return get<arrayType>()[index];
		default:
			throw runtime_error("trying to access element of type "
					+typestr()+" with index:"+to_string(index));
	}
}

anjson::variant& anjson::variant::operator[](uint32_t index){
	switch(p){
		case type::object:
			return get<mapType>()[to_string(index)];
		case type::array:
			return get<arrayType>()[index];
		default:
			throw runtime_error("trying to access element of type "
					+typestr()+" with index:"+to_string(index));
	}
}

#include"syntools.hpp"
#include"anjson_accessor.hpp"
#include<sstream>


const anjson::variant anjson::variant::query(const string& s) const{
	stringstream ss(s);
	anjLexer e(ss);
	variant result;
	anjsonAccessor j(&e,*this,result);
	j.parse();

	return result;
}

