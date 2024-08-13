#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>

class bitwriter {
	std::ofstream& of;
	uint8_t _buf;
	int _n = 0;


	void  write(uint32_t val) {
		_buf = (_buf << 1) | (val & 1);
		_n++;
		if (_n == 8) {
			of.write(reinterpret_cast<char *>(&_buf), sizeof(_buf));
			_n = 0;
		}
	}
public:
	bitwriter(std::ofstream& ofs) : of(ofs) {}
	bitwriter(const bitwriter& rhs) = delete;
	bitwriter& operator =(const bitwriter& rhs) = delete;
	~bitwriter() {
		flush();
	}

	std::ofstream& operator()(uint32_t code, uint32_t len) {
		while (len-- > 0) {
			write(code >> len);
		}
		return of;
	}

	std::ofstream& flush(uint32_t bit = 1) {
		while (_n) {
			write(bit);
		}
		return of;
	}
};