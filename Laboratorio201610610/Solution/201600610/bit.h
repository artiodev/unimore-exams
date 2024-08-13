#if !defined BIT_H
#define BIT_H

#include <iostream>
#include <fstream>
#include <cstdint>


class bitreader {
	std::ifstream& it;
	std::uint8_t _buf;
	int _n = 0;


	uint32_t read() {
		if (_n == 0) {
			_buf = it.get();
			_n = 8;
		}
		_n--;
		return (_buf >> _n) & 1;
	}

public:
	bitreader(std::ifstream& its) : it(its) {}
	bitreader(const bitreader& rhs) = delete;
	bitreader& operator=(const bitreader& rhs) = delete;


	uint32_t operator()(uint32_t len) {
		uint32_t val = 0;
		while (len-- > 0) {
			val = (val << 1) | read();
		}
		return val;
	}


};
#endif