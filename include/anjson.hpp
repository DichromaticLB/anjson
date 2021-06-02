#ifndef INCLUDE_ANJSON_HPP_
#define INCLUDE_ANJSON_HPP_

#include<cstdint>
#include<string>
#include<vector>
#include<unordered_map>
#include<memory>
#include<iostream>

namespace anjson{

	using namespace std;
	enum class type:uint8_t{
		object=1,
		string=2,
		fp=4,
		array=8,
		nulltype=16,
		booltype=32,
		integer=64
	};

	string typestr(type t);

	class variant;
	struct pretty_variant{
		pretty_variant(const variant&vv):v(vv){}
		const variant &v;
	};

	class variant{
	public:
		using mapType=std::unordered_map<string,variant>;
		using arrayType=std::vector<variant>;


		variant();
		variant(type p);

		template<class T>
		variant(T t):variant(){
			(*this)=t;
		}

		~variant();
		variant(variant&&t);
		variant(const anjson::variant&);
		variant& operator=(variant&&t);
		variant& operator=(const variant&t);
		variant& operator=(const string& s);
		variant& operator=(const char* s);
		variant& operator=(const arrayType&ar);
		variant& operator=(const mapType&ar);
		variant& operator=(double d);
		variant& operator=(uint64_t i);
		variant& operator=(int64_t i);
		variant& operator=(bool b);
		void release();


		const variant& operator[](const string& key) const;
		variant& operator[](const string& key);
		const variant& operator[](uint32_t index) const;
		variant& operator[](uint32_t index);

		bool containsKey(const string&s) const;
		bool containsKeyType(const string&s,type t) const;
		const variant query(const string& s)const;
		const variant query(const string& s,const variant &def)const;
		double doubleCast()const;

		bool containsKeys() const{
			return true;
		}

		template<typename... Targs>
		bool containsKeys(const string&s, Targs... rem) const{
			return containsKey(s)&&containsKeys(rem...);
		}

		string typestr() const{
			return anjson::typestr(p);
		}

    	template<class T>
    	T& get(){
    		return *reinterpret_cast<T*>(data);
    	}


    	template<class T>
    	const T& get() const{
    		return *reinterpret_cast<const T*>(data);
    	}

    	type getType() const;

    	pretty_variant pretty() const{
    		return pretty_variant(*this);
    	}

	private:

		template<class T>
		void appropiate(const T& t){
			release();
			data=new T(t);
		}

		template<class T>
		void catRel(){
			delete reinterpret_cast<T*>(data);
			data=nullptr;
			p=type::nulltype;
		}

    	template<class T,type y>
    	variant& set(const T& t)
    	{
    		if(p==y)
    			get<T>()=t;
    		else
    		{
    			release();
    			appropiate(t);
    			p=y;
    		}

    		return *this;
    	}


		void *data;
		type p;
	};

	ostream& operator<<(ostream&o,const variant&v);
	ostream& operator<<(ostream&o,const pretty_variant&v);
	variant fromStream(istream& i);
	variant fromFile(const string& filename);
	variant fromString(const string&src);

	using jsonobject=variant;
}

#endif
