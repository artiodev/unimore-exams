#if !defined BIT_H
#define BIT_H

#include <cstdint>
#include <fstream>
#include <iostream>

class bitreader {
	std::ifstream& it;
	uint8_t _buf;
	int n=0;

	std::uint32_t read() {
		if (n == 0) {
			_buf = it.get();
			n = 8;
		}
		n--;
		return (_buf >> n) & 1;
	}
public:
	bitreader(std::ifstream& ig): it(ig) {}
	bitreader(const bitreader& rhs) = delete;
	bitreader& operator=(const bitreader& rhs) = delete;

	uint32_t operator()(uint32_t len){
		uint32_t val = 0;
		while (len-- > 0) {
			val = (val << 1) | read();
		}
		return val;
	}
};



#endif