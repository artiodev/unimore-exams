#include "huffman.h"

#include <string>
#include <fstream>
#include <iterator>
#include <iomanip>

using namespace std;

string to_binary(uint8_t len, uint32_t code) {
	string s;
	while (len > 0) {
		s = (code & 1 ? '1' : '0') + s;
		code >>= 1;
		--len;
	}
	return s;
}

int main() {
	// Apro il file e non salto i whitespace
	ifstream is("bibbia.txt", ios::binary);
	is.unsetf(ios_base::skipws);

	// Stimo le frequenze dei byte nel file
	frequency<uint8_t> f;
	f = for_each(istream_iterator<char>(is), istream_iterator<char>(), f);
	// Aggiungo 1 per ogni possibile byte (per essere sicuro di avere 
	// un codice di Huffman per ogni possibile simbolo)
	for (int i = 0; i < 256; ++i)
		f(i);
	// Calcolo i codici canonici di Huffman
	huffman<uint8_t> huff(f);

	// Output di esempio dei codici
	ofstream os("huffman.txt");
	for (const auto& x : huff.table()) {
		if (x._sym >= 32)
			os.put(x._sym);
		else
			os << "\\x" << hex << setfill('0') << setw(2) << uint32_t(x._sym);

		os << " - " << to_binary(x._len, x._code) << "\n";
	}
}