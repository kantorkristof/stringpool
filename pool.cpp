#include <iostream>
#include "memtrace.h"
#include "pool.h"
#include <cfloat>

StringPool::StringPool(size_t obj_num, size_t init_cap) {
	for (size_t i = 0; i < obj_num; i++) container.push_back(Element{ init_cap }); 
}

RString& StringPool::acquire(size_t capacity) {
	Element* result = nullptr;
	double min = DBL_MAX;

	for (auto& i : container) {
		if(!i.itsfree) continue;
		double tmp = i.rstring.capacity() / capacity;
		bool minflag = tmp < min && tmp >= 1;
		if(minflag){
			min = tmp;
			result = &i;
		}
	}
	if(result){
		result->itsfree = false;
		return result->rstring;
	}
	container.push_back(Element(capacity,false));
	return container.back().rstring;
}

RString& StringPool::acquire(const char* str) {
	size_t len = strlen(str);
	for (auto& i : container) {
		if (len <= (i.rstring.capacity() - i.rstring.size()) && i.itsfree) {
			strcat(i.rstring, str);
			i.itsfree = false;
			return i.rstring;
		}
	}
	container.push_back(Element(RString(str,strlen(str)+1), false));
	return container.back().rstring;
}

bool StringPool::acquireable(RString& str) {
	for (auto const& i : container) if (&i.rstring == &str) return i.itsfree;
	return false;
}

void StringPool::release(RString& str) {
	for (auto& i : container) {
		if (&(i.rstring) == &str) {
			i.rstring[0] = '\0';
			i.itsfree = true;
			return;
		}
	}
	throw "QVU4YG";
}

RString& StringPool::append(RString& str1, const RString& str2) {
	Element* s1 = nullptr; 
	Element* s2 = nullptr; 

	for(auto& i: container){
		if (!i.itsfree) {
			if (&i.rstring == &str1) s1 = &i;
			if (&i.rstring == &str2) s2 = &i;
		}
	}
	if(!s1 || !s2) throw "QVU4YG";

	size_t required = str1.size() + str2.size();
	if(str1.capacity() > required){
		strcat(str1, str2);
		return str1;
	}

	RString& newrstring = acquire(required + 1);
	strcat(newrstring, str1);
	strcat(newrstring, str2);
	return newrstring;
}

size_t StringPool::size() const { 
	return container.size(); 
}

size_t StringPool::free_size() const { 
	size_t frees = 0;
	for (auto& i : container) if (i.itsfree == true) frees++;
	return frees;
}