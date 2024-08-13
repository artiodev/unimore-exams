#ifndef FREQUENCIES_H
#define FREQUENCIES_H

#include <map>

template <typename T>
struct frequences {
	typedef unsigned int uint;

	typedef std::map<T, uint> cont_t;
	cont_t _f;

	typedef typename cont_t::iterator iterator;
	typedef typename cont_t::const_iterator const_iterator;

	void operator() (T b) { _f[b]++; }

	uint& operator[] (T b) { return _f[b]; }
	const uint& operator[] (T b) const { return _f[b]; }

	iterator begin() { return _f.begin(); }
	const_iterator begin() const { return _f.begin(); }
	iterator end() { return _f.end(); }
	const_iterator end() const { return _f.end(); }
};

#endif // FREQUENCIES_H