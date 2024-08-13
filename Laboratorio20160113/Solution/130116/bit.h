#include <fstream>
#include <cstdint>


class bitwriter {
	std::ostream& of;
	std::uint8_t _buf;
	int _n=0;

	std::ostream&  write(uint32_t bit) {
		_buf = (_buf << 1) | (bit & 1);
		if (++_n == 8) {
			of.put(_buf);
			_n = 0;
		}
		return of;
	}

public:
	bitwriter(std::ostream& o) : of(o) {}
	bitwriter(const bitwriter& rhs) = delete;
	bitwriter& operator =(const bitwriter& rhs) = delete;
	~bitwriter() {
		flush();
	}

	std::ostream& operator()(uint32_t val, uint32_t n) {
		while (n --> 0) 
			write(val >> n);
		
		return of;
	}

	std::ostream& flush(uint32_t bit = 0) {
		while (_n) {
			write(bit);
		}
		return of;
	}
};

class bitreader {
	std::istream& _is;
	std::uint8_t _buf;
	int _n = 0;

	uint32_t read() {
		if (_n == 0) {
			_buf = _is.get();
			_n = 8;
		}
		_n--;
		return (_buf >> _n) & 1;
	}

public:
	bitreader(std::istream& is) : _is(is) {}
	bitreader(const bitreader& rhs) = delete;
	bitreader& operator=(const bitreader& rhs) = delete;
	

	uint32_t operator()(uint32_t n) {
		uint32_t val = 0;
		while (n-- > 0) {
			val = (val << 1) | read();
		}
		return val;
	}
};
	
	