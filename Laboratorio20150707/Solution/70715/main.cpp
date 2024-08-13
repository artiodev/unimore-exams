#include "huffman.h"

#include <string>
#include <fstream>
#include <iterator>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>
#include <utility>
#include <string>
#include <sstream>

#include "bit.h"
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

struct vlc {
	uint8_t _sym;
	uint8_t _len = 0;
	uint32_t _code = 0;

	vlc() {}
	vlc(uint8_t sym) : _sym(sym) {}
	vlc(uint8_t sym, uint8_t len, uint32_t code) : _sym(sym), _len(len), _code(code) {}
};



class hufstr {
private:
	std::vector<vlc> _table;

public:
	hufstr(vector<vlc>& table) :_table(table) {}
	hufstr(const string& filename) {
		ifstream is(filename, ios::binary);
		if (!is)
			exit(EXIT_FAILURE);

		uint8_t sym, len;
		uint32_t code;

		while (true) {
			if (is.eof()) {
				break;
			}

			sym = is.get();
			len = is.get();
			is.read(reinterpret_cast<char*>(&code), 4);

			_table.push_back(vlc(sym, len, code));
		}

	}

	vector<uint8_t> compress(const string& s) const {
		vector<uint8_t> byte;
		uint8_t symbol;
		bitwriter bw(byte);
		bool trovato = false;
		for (const auto& x : s) {
			symbol = x;
			for (const auto& x : _table) {
				if (x._sym == symbol) {
					bw(x._code, x._len);
					trovato = true;
					cout << ".";
					break;
				}
			}
			if (trovato == false) {
				cout << "stringa non comprimibile con la tabella specificata \n";
				exit(2);
			}
			else {
				trovato = false;
			}
		}
		bw.~bitwriter();
		return byte;
	}



	string decompress(const vector<uint8_t>& v) const {
		bitreader br(v);
		uint32_t buf=0;
		string decodfica;
		int n = 0;
		int c = 0;
		bool trovato = false;
		while (br.getIndex() < v.size()) {
			c = 0;
			while (!trovato || c % 8 != 0) {
				if (br.getIndex() > v.size()) break;
				buf = (buf << 1) | br(1);
				n++;
				c++;
				for (const auto& x : _table) {
					if (x._code == buf && n == x._len) {
						n = 0;
						buf = 0;
						trovato = true;
						decodfica.push_back(x._sym);
						break;
					}
				}
			}
			trovato = false;
		}

		return decodfica;
	}
};

vector<vlc> vlc_table{
	vlc(32,3,0),
	vlc(101,3,1),
	vlc(97,4,4),
	vlc(105,4,5),
	vlc(108,4,6),
	vlc(110,4,7),
	vlc(111,4,8),
	vlc(114,4,9),
	vlc(99,5,20),
	vlc(100,5,21),
	vlc(115,5,22),
	vlc(116,5,23),
	vlc(117,5,24),
	vlc(39,6,50),
	vlc(44,6,51),
	vlc(103,6,52),
	vlc(109,6,53),
	vlc(112,6,54),
	vlc(118,6,55),
	vlc(9,7,112),
	vlc(10,7,113),
	vlc(13,7,114),
	vlc(46,7,115),
	vlc(91,7,116),
	vlc(93,7,117),
	vlc(98,7,118),
	vlc(102,7,119),
	vlc(104,7,120),
	vlc(34,8,242),
	vlc(49,8,243),
	vlc(83,8,244),
	vlc(113,8,245),
	vlc(122,8,246),
	vlc(50,9,494),
	vlc(51,9,495),
	vlc(58,9,496),
	vlc(59,9,497),
	vlc(65,9,498),
	vlc(68,9,499),
	vlc(69,9,500),
	vlc(71,9,501),
	vlc(73,9,502),
	vlc(48,10,1006),
	vlc(52,10,1007),
	vlc(53,10,1008),
	vlc(54,10,1009),
	vlc(55,10,1010),
	vlc(56,10,1011),
	vlc(57,10,1012),
	vlc(63,10,1013),
	vlc(67,10,1014),
	vlc(76,10,1015),
	vlc(77,10,1016),
	vlc(78,10,1017),
	vlc(80,10,1018),
	vlc(33,11,2038),
	vlc(66,11,2039),
	vlc(70,11,2040),
	vlc(79,11,2041),
	vlc(81,11,2042),
	vlc(82,11,2043),
	vlc(84,11,2044),
	vlc(45,12,4090),
	vlc(86,12,4091),
	vlc(107,12,4092),
	vlc(40,13,8186),
	vlc(41,13,8187),
	vlc(72,13,8188),
	vlc(85,13,8189),
	vlc(90,13,8190),
	vlc(75,14,16382),
	vlc(47,17,131064),
	vlc(119,18,262130),
	vlc(64,19,524262),
	vlc(74,19,524263),
	vlc(88,20,1048528),
	vlc(120,20,1048529),
	vlc(87,21,2097060),
	vlc(94,21,2097061),
	vlc(106,21,2097062),
	vlc(121,21,2097063),
	vlc(125,21,2097064),
	vlc(0,22,4194130),
	vlc(1,22,4194131),
	vlc(2,22,4194132),
	vlc(3,22,4194133),
	vlc(4,22,4194134),
	vlc(5,22,4194135),
	vlc(6,22,4194136),
	vlc(7,22,4194137),
	vlc(8,22,4194138),
	vlc(11,22,4194139),
	vlc(12,22,4194140),
	vlc(14,22,4194141),
	vlc(15,22,4194142),
	vlc(16,22,4194143),
	vlc(17,22,4194144),
	vlc(18,22,4194145),
	vlc(19,22,4194146),
	vlc(20,22,4194147),
	vlc(21,22,4194148),
	vlc(22,22,4194149),
	vlc(23,22,4194150),
	vlc(24,22,4194151),
	vlc(25,22,4194152),
	vlc(26,22,4194153),
	vlc(27,22,4194154),
	vlc(28,22,4194155),
	vlc(29,22,4194156),
	vlc(30,22,4194157),
	vlc(31,22,4194158),
	vlc(35,22,4194159),
	vlc(36,22,4194160),
	vlc(37,22,4194161),
	vlc(38,22,4194162),
	vlc(42,22,4194163),
	vlc(43,22,4194164),
	vlc(60,22,4194165),
	vlc(61,22,4194166),
	vlc(62,22,4194167),
	vlc(89,22,4194168),
	vlc(92,22,4194169),
	vlc(95,22,4194170),
	vlc(96,22,4194171),
	vlc(123,22,4194172),
	vlc(124,22,4194173),
	vlc(126,22,4194174),
	vlc(127,22,4194175),
	vlc(128,22,4194176),
	vlc(129,22,4194177),
	vlc(130,22,4194178),
	vlc(131,22,4194179),
	vlc(132,22,4194180),
	vlc(133,22,4194181),
	vlc(134,22,4194182),
	vlc(135,22,4194183),
	vlc(136,22,4194184),
	vlc(137,22,4194185),
	vlc(138,22,4194186),
	vlc(139,22,4194187),
	vlc(140,22,4194188),
	vlc(141,22,4194189),
	vlc(142,22,4194190),
	vlc(143,22,4194191),
	vlc(144,22,4194192),
	vlc(145,22,4194193),
	vlc(146,22,4194194),
	vlc(147,22,4194195),
	vlc(148,22,4194196),
	vlc(149,22,4194197),
	vlc(150,22,4194198),
	vlc(151,22,4194199),
	vlc(152,22,4194200),
	vlc(153,22,4194201),
	vlc(154,22,4194202),
	vlc(155,22,4194203),
	vlc(156,22,4194204),
	vlc(157,22,4194205),
	vlc(158,22,4194206),
	vlc(159,22,4194207),
	vlc(160,22,4194208),
	vlc(161,22,4194209),
	vlc(162,22,4194210),
	vlc(163,22,4194211),
	vlc(164,22,4194212),
	vlc(165,22,4194213),
	vlc(166,22,4194214),
	vlc(167,22,4194215),
	vlc(168,22,4194216),
	vlc(169,22,4194217),
	vlc(170,22,4194218),
	vlc(171,22,4194219),
	vlc(172,22,4194220),
	vlc(173,22,4194221),
	vlc(174,22,4194222),
	vlc(175,22,4194223),
	vlc(176,22,4194224),
	vlc(177,22,4194225),
	vlc(178,22,4194226),
	vlc(179,22,4194227),
	vlc(180,22,4194228),
	vlc(181,22,4194229),
	vlc(182,22,4194230),
	vlc(183,22,4194231),
	vlc(184,22,4194232),
	vlc(185,22,4194233),
	vlc(186,22,4194234),
	vlc(187,22,4194235),
	vlc(188,22,4194236),
	vlc(189,22,4194237),
	vlc(190,22,4194238),
	vlc(191,22,4194239),
	vlc(192,22,4194240),
	vlc(193,22,4194241),
	vlc(194,22,4194242),
	vlc(195,22,4194243),
	vlc(196,22,4194244),
	vlc(197,22,4194245),
	vlc(198,22,4194246),
	vlc(199,22,4194247),
	vlc(200,22,4194248),
	vlc(201,22,4194249),
	vlc(202,22,4194250),
	vlc(203,22,4194251),
	vlc(204,22,4194252),
	vlc(205,22,4194253),
	vlc(206,22,4194254),
	vlc(207,22,4194255),
	vlc(208,22,4194256),
	vlc(209,22,4194257),
	vlc(210,22,4194258),
	vlc(211,22,4194259),
	vlc(212,22,4194260),
	vlc(213,22,4194261),
	vlc(214,22,4194262),
	vlc(215,22,4194263),
	vlc(216,22,4194264),
	vlc(217,22,4194265),
	vlc(218,22,4194266),
	vlc(219,22,4194267),
	vlc(220,22,4194268),
	vlc(221,22,4194269),
	vlc(222,22,4194270),
	vlc(223,22,4194271),
	vlc(224,22,4194272),
	vlc(225,22,4194273),
	vlc(226,22,4194274),
	vlc(227,22,4194275),
	vlc(228,22,4194276),
	vlc(229,22,4194277),
	vlc(230,22,4194278),
	vlc(231,22,4194279),
	vlc(232,22,4194280),
	vlc(233,22,4194281),
	vlc(234,22,4194282),
	vlc(235,22,4194283),
	vlc(236,22,4194284),
	vlc(237,22,4194285),
	vlc(238,22,4194286),
	vlc(239,22,4194287),
	vlc(240,22,4194288),
	vlc(241,22,4194289),
	vlc(242,22,4194290),
	vlc(243,22,4194291),
	vlc(244,22,4194292),
	vlc(245,22,4194293),
	vlc(246,22,4194294),
	vlc(247,22,4194295),
	vlc(248,22,4194296),
	vlc(249,22,4194297),
	vlc(250,22,4194298),
	vlc(251,22,4194299),
	vlc(252,22,4194300),
	vlc(253,22,4194301),
	vlc(254,22,4194302),
	vlc(255,22,4194303)
};

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

	ofstream ost("prova.txt");
	for (const auto& x : huff.table()) {
		ost << uint32_t(x._sym) << " ";
		ost << uint32_t(x._len) << " ";
		ost << uint32_t(x._code) << "\n";
	}

	ofstream myos("huffman.dat", ios::binary);
	for (const auto& x : huff.table()) {
		myos.put(x._sym);
		myos.put(x._len);
		myos.write(reinterpret_cast<const char*>(&(x._code)), 4);
	}
	myos.close();
	
	
	string filename = "huffman.dat";

	hufstr h(filename);
	string compressione = "aeio";
	cout << "Original string: " << compressione << "\n";
	cout << "String compression: ";
	vector<uint8_t> _v=h.compress(compressione);
	cout << " Done\n";
	cout << "String compress: ";
	for (auto x : _v) {
		cout << (x);
	}
	ofstream otf("compress.dat", ios::binary);
	for (auto x : _v) {
		otf.put(x);
	}
	cout << "\n";
	string dec = h.decompress(_v);
	cout << "String decompress: " << dec << "\n";

	cout << "Static use \n";
	hufstr test2(vlc_table);
	string compres = "pippo fa la pippa";
	cout << "Original string: " << compres << "\n";
	cout << "String compression: ";
	vector<uint8_t> _v2 = h.compress(compres);
	cout << " Done\n";
	cout << "String compress: ";
	for (auto x : _v2) {
		cout << (x);
	}
	cout << "\n";
	string dec2 = h.decompress(_v2);
	cout << "String decompress: " << dec2 << "\n";
}