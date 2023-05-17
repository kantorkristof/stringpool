



#include <iostream>
#include "memtrace.h"
#include "pool.h"

StringPool::StringPool(size_t obj_num, size_t init_cap) {
	for (size_t i = 0; i < obj_num; i++) { container.push_back(Element{ init_cap }); }
}

RString& StringPool::acquire(size_t capacity) {
	for (size_t i = 0; i < container.size(); i++) {
		if (container[i].rstring.capacity() >= capacity && container[i].itsfree) {
			container[i].itsfree = false;	
			return container[i].rstring;
		}
	}
	container.push_back(Element(capacity,false));
	return container[container.size()-1].rstring;
}

RString& StringPool::acquire(const char* str) {
	size_t len = strlen(str);
	for (size_t i = 0; i < container.size(); i++) {
		if (len <= (container[i].rstring.capacity() - container[i].rstring.size()) && container[i].itsfree) {
			strcat(container[i].rstring, str);
			container[i].itsfree = false;
			return container[i].rstring;
		}
	}
	container.push_back(Element(RString(str,strlen(str)+1), false));
	container[container.size() - 1].rstring[strlen(str)] = '\0';
	return container[container.size() - 1].rstring;
}

bool StringPool::acquireable(RString& str) {
	for (size_t i = 0; i < container.size(); i++) {
		if (container[i].rstring == str) return container[i].itsfree;
	}
	return false;
}

void StringPool::release(RString& str) {
	for (size_t i = 0; i < container.size(); i++) {
		if (container[i].rstring == str) {
			container[i].rstring[0] = '\0';
			container[i].itsfree = true;
			return;
		}
	}
}
/*
RString& StringPool::append(RString& str1, const RString& str2) {
	
}*/

size_t StringPool::size() const { return container.size(); }
size_t StringPool::free_size() const { 
	size_t frees = 0;
	for (size_t i = 0; i < container.size(); i++) {
		if (container[i].itsfree == true) frees++;
	}
	return frees;
}