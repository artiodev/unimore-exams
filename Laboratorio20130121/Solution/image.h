#if !defined IMAGE_H
#define IMAGE_H

#include <iostream>
#include <cstdint>
#include <array>
#include <vector>

template<typename T>
class image {
	size_t w;
	size_t h;
	std::vector<T> _data;

public:
	image(size_t _w, size_t _h) :w(_w), h(_h), _data(_w*_h) {}
	image() {}
	size_t width() { return w; }
	size_t height() { return h; }

	T& operator()(size_t x, size_t y){ 
		return _data[y*w + x];
	}

	const T& operator()(size_t x, size_t y)const{
		return _data[y*w + x];
	}

	char * dataptr() { 
		return reinterpret_cast<char*>(&_data[0]);
	}
	const char * dataptr() const {
		return reinterpret_cast<char*>(&_data[0]);
	}

	size_t datasize() { 
		return w*h*sizeof(T);
	}

	auto begin() -> decltype(_data.begin()) { return _data.begin(); }
	auto begin() const -> decltype(_data.begin()) { return _data.begin(); }
	auto end() -> decltype(_data.end()) { return _data.end(); }
	auto end() const -> decltype(_data.end()) { return _data.end(); }
};


using rgb = std::array<uint8_t, 3>;

#endif
