#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <ios>

template <typename _T>
struct image {
	unsigned _w, _h;
	std::vector<_T> _data;

	image() : _w(0), _h(0) {}
	image(unsigned w, unsigned h) : _w(w), _h(h), _data(w*h) {}

	unsigned width() const { return _w; }
	unsigned height() const { return _h; }
	unsigned size() const { return _w*_h; }

	void resize(unsigned w, unsigned h);
	template <typename _Other>
	void resize(const image<_Other>& img);

	const _T& operator() (unsigned x, unsigned y) const {
		return _data[y*_w + x];
	}
	_T& operator() (unsigned x, unsigned y) {
		return _data[y*_w + x];
	}

	const char* data_ptr() const {
		if (_data.size() == 0)
			return 0;
		else
			return reinterpret_cast<const char *>(&_data[0]);
	}
	char* data_ptr() {
		if (_data.size() == 0)
			return 0;
		else
			return reinterpret_cast<char *>(&_data[0]);
	}

	typedef typename std::vector<_T>::iterator iterator;
	typedef typename std::vector<_T>::const_iterator const_iterator;
	iterator begin() { return _data.begin(); }
	iterator end() { return _data.end(); }
	const_iterator begin() const { return _data.begin(); }
	const_iterator end() const { return _data.end(); }
};

template <typename _T>
void image<_T>::resize(unsigned w, unsigned h) {
	_w = w;
	_h = h;
	_data.resize(size());
}

template <typename _T>
template <typename _Other>
void image<_T>::resize(const image<_Other>& img) {
	resize(img.width(), img.height());
}

template <typename _T>
std::ostream& operator<< (std::ostream& os, const image<_T>& img) {
	os << img.width() << "x" << img.height();
	return os;
}

#endif /* IMAGE_H */
