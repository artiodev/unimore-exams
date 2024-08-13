#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>

class bitwriter {
	std::vector<uint8_t>& v;
	uint8_t _buf;
	int _n=0;


	std::vector<uint8_t>&  write(uint32_t val) {
		_buf = (_buf << 1) | (val & 1);
		_n++;
		if (_n == 8) {
			v.push_back(_buf);
			_n = 0;
		}
		return v;
	}

public :
	bitwriter(std::vector<uint8_t>& ofs): v(ofs) {}
	bitwriter(const bitwriter& rhs) = delete;
	bitwriter& operator =(const bitwriter& rhs) = delete;
	~bitwriter() {
		flush();
	}

	std::vector<uint8_t>& operator()(uint32_t code, uint32_t len) {
		while (len-- > 0) {
			write(code >> len);
		}
		return v;
	}

	std::vector<uint8_t>& flush(uint32_t bit=1) {
		while (_n) {
			write(bit);
		}
		return v;
	}
};

class bitreader {
	const std::vector<uint8_t>& v;
	uint8_t _buf;
	int _n = 0;
	int c = 0;

	uint32_t  read() {
		if (_n == 0) {
			_buf = v.at(c);
			c++;
			_n = 8;
		}
		_n--;
		return (_buf >> _n) & 1;
	}

public:
	bitreader(const std::vector<uint8_t>& ofs) : v(ofs) {}
	bitreader(const bitreader& rhs) = delete;
	bitreader& operator =(const bitreader& rhs) = delete;

	std::uint32_t operator()(uint32_t n) {
		uint32_t val=0;
		while (n-- > 0) {
			val = (val << 1) | read();
		}
		return val;
	}

	int getIndex() {
		return c;
	}
};