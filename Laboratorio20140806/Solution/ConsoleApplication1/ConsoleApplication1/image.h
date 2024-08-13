#if !defined IMAGE_H
#define IMAGE_H

#include<iostream>
#include <vector>

template<typename T>
class image {
	size_t w;
	size_t h;
	std::vector<T> _data;
public:
	image(size_t w, size_t h) :w(w), h(h), _data(w*h) {}
	size_t width() { 
		return w; 
	}
	size_t height() {
		return h; 
	}

	char* dataptr() {
		return reinterpret_cast<char*>(&_data[0]);
	}
	const char* dataptr() const {
		return reinterpret_cast<char*>(&_data[0]);
	}

	size_t datasize() {
		return w*h*sizeof(T);
	}
	T& operator ()(size_t x, size_t y) {
		return _data[y*w + x];
	}
	const T& operator () (size_t x, size_t y) const {
		return _data[y*w + x]; 
	}

};



#endif