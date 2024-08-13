#if !defined BIT_H
#define BIT_H

#include <ostream>
#include <cstdint>

class bitwriter {
	std::ostream& _os;
	std::uint8_t _buf;
	int _n = 0;

	// Aggiungo il bit meno significativo di bit a _buf
	// Se _buf ha 8 bit lo scrivo su file
	std::ostream& write(uint32_t bit) {
		_buf = (_buf << 1) | (bit & 1);
		// _buf = (_buf * 2) + (bit % 2);
		++_n;
		if (_n == 8) {
			_os.put(_buf);
			_n = 0;
		}
		return _os;
	}

public:
	bitwriter(std::ostream& os) : _os(os) {}
	bitwriter(const bitwriter& rhs) = delete;
	bitwriter& operator=(const bitwriter& rhs) = delete;
	~bitwriter() {
		flush();
	}

	std::ostream& operator()(uint32_t val, uint32_t n) {
		while (n-- > 0)
			write(val >> n);
		return _os;
	}

	std::ostream& flush(uint32_t bit = 0) {
		while (_n)
			write(bit);
		return _os;
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
		while (n--> 0)
			val = (val << 1) | read();
		return val;
	}

	std::istream& operator()(uint32_t& val, uint32_t n) {
		val = 0;
		while (n--> 0)
			val = (val << 1) | read();
		return _is;
	}

};

#endif // BIT_H
