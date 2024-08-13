#if !defined IMAGE_H
#define IMAGE_H

#include <cstdint>
#include <array>
#include <vector>

template <typename T>
class image {
	size_t _h;
	size_t _w;
	std::vector<T> _data;
public:
	image(size_t width, size_t height) : _w(width), _h(height), _data(width*height) {}

	T& operator()(size_t x, size_t y) { return _data[y*_w + x]; }
	const T& operator()(size_t x, size_t y) const { return _data[y*_w + x]; }

	char* dataptr() { return reinterpret_cast<char>(&_data[0]); }
	const char* dataptr() const { return reinterpret_cast<char>(&_data[0]); }

	size_t datasize() const { return _h*_w*sizeof(T); }

	auto begin() ->decltype (_data.begin()) { return _data.begin();}
	auto begin() const  ->decltype (_data.begin()) { return _data.begin(); }
	auto end() ->decltype (_data.end()) { return _data.end(); }
	auto end() const  ->decltype (_data.end()) { return _data.end(); }


	size_t width() {
		return _w;
	}
	size_t height() {
		return _h;
	}
};

using rgb = std::array<uint8_t, 3>;
#endif