#include "memtrace.h"
#include "rstring.h"



RString::RString(size_t capacity) {
	arr = new char[capacity];
	cap = capacity;
	arr[0] = '\0';
}
RString::RString(const char* str) {
	size_t tempcap = strlen(str) + 1;
	arr = new char[tempcap];
	strcpy(arr, str);
	cap = tempcap;
}
RString::RString(const char* str, size_t capacity) {
	arr = new char[capacity];
	if (strlen(str) >= capacity) throw "QVU4YG";
	strcpy(arr, str);
	cap = capacity;
}
size_t RString::size() const { return strlen(arr); }
size_t RString::capacity() const { return cap; }
RString::operator char* () const { return arr; }
RString& RString::operator=(const char* str) {
	if (strlen(str) >= cap) throw "QVU4YG";
	strcpy(arr, str);
	return *this;
}