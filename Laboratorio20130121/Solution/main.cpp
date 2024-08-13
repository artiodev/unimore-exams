#include <fstream>
#include <vector>
#include <iostream>
#include <string>

#include "image.h"

using namespace std;

struct packbit {
	enum class status { empty, run, copy } s;
	vector<uint8_t> buffer;

	void output(ostream& os) {
		int L = buffer.size();
		switch (s)
		{
		case status::empty:
		case status::run:
			os.put(257 - L);
			os.put(buffer.back());
			break;
		case status::copy:
			os.put(L - 1);
			copy(begin(buffer), begin(buffer) + L, ostream_iterator<uint8_t>(os));
			break;
		default:
			break;
		}
		buffer.clear();
	}

	void compress(image<uint8_t>& img, ofstream &of) {
		s = status::empty;
		int j = 0;
		int i = 0;

		while (true) {
			
			if (j == img.width()) {
				j = 0;
				i++;
			}
			if (i == img.height()) break;
			uint8_t next = img(j, i);
			
			switch (s)
			{
			case packbit::status::empty:
				if (!buffer.empty()) {
					if (next == buffer.back()) {
						s = status::run;
					}
					else
						s = status::copy;

				}
				buffer.push_back(next);
				break;
			case packbit::status::run:
				if (next == buffer.back()) {
					buffer.push_back(next);
				}
				else {
					output(of);
					buffer.push_back(next); // Aggiungto alla normale codifica con fstream perche peek non raccoglie il carattere
					s = status::empty;
				}
				break;
			case packbit::status::copy:
				if (next == buffer.back()) {
					buffer.pop_back();
					output(of);
					buffer.push_back(next);
					s = status::run;
				}
				buffer.push_back(next);
				break;
			default:
				break;
			}
			if (buffer.size() == 128) {
				output(of);
				s = status::empty;
			}
			j++;
		}
		output(of);
		of.put(uint8_t(128));
	}
};



image<rgb> readPPM(ifstream& it) {

	string magic;
	size_t w;
	size_t h;
	size_t max_data;

	getline(it, magic);
	bool plain;
	if (magic == "P6") {
		plain = false;
	}
	else if (magic == "P3") {
		plain = true;
	}

	while (it.peek() == '#') {
		string comment;
		getline(it, comment);
	}
	
	it >> w >> h >> max_data;
	it.get();
	image<rgb> img(w, h);
	if (plain) {
		for (auto&x : img) {
			it >> x[0] >> x[1] >> x[2];
		}
	}
	else {
		it.read(img.dataptr(), img.datasize());
	}
	return img;
}



bool writePPMColor(image<uint8_t>& img, string filename) {
	ofstream os(filename+".bin", ios::binary);
	if (!os)return false;

	for (size_t i = 0; i < img.height(); ++i) {
		for (size_t j = 0; j < img.width(); ++j) {
			os.put(img(j, i));
		}
	}

}



void compute64(uint8_t a, uint8_t b, uint8_t c, ofstream& of, int& cont,bool two,vector<uint8_t>& Table,bool three) {

	uint8_t R1 = a >> 2;
	uint8_t R2 = a & 0x03;
	R2 = (R2 << 4) | (b >> 4 & 0x0F);
	uint8_t R3 = (b & 0x0F);
	R3 = (R3 << 2) | (c >> 6 & 0x03);
	uint8_t R4 = c & 0x3F;


	for (int i = 0; i < Table.size(); ++i) {
		if (i == R1) {
			of.put(Table[i]);
			cont++;
			break;
		}
		if (cont == 76) {
			of.put('\n');
			cont = 0;
		}
	}
	for (int i = 0; i < Table.size(); ++i) {
		if (i == R2) {
			of.put(Table[i]);
			cont++;
			break;
		}
		if (cont == 76) {
			of.put('\n');
			cont = 0;
		}
	}
	if (two == false) {
		for (int i = 0; i < Table.size(); ++i) {
			if (i == R3) {
				of.put(Table[i]);
				cont++;
				break;
			}
			if (cont == 76) {
				of.put('\n');
				cont = 0;
			}
		}
	if (three == false) {
			for (int i = 0; i < Table.size(); ++i) {
				if (i == R4) {
					of.put(Table[i]);
					cont++;
					break;
				}
				if (cont == 76) {
					of.put('\n');
					cont = 0;
				}
			}
		}
	}

}


void encode64(ifstream& iq,ofstream& ol) {
	vector<uint8_t> Table;
	for (int i = 'A'; i <= 'Z'; ++i) {
		Table.push_back(i);
	}
	for (int i = 'a'; i <= 'z'; ++i) {
		Table.push_back(i);
	}
	for (int i = '0'; i <= '9'; ++i) {
		Table.push_back(i);
	}
	Table.push_back('+');
	Table.push_back('/');

	int cont = 0;
	bool two = false;
	bool three = false;
	while (true) {

		uint8_t a = iq.get();
		if (iq.eof()) break;
		uint8_t b = iq.get();
		if (iq.eof()) {
			two = true;
			compute64(a, 0, 0, ol, cont, two, Table,three);
			two = false;
			ol.put('=');
			ol.put('=');
			break;
		}
		uint8_t c = iq.get();
		if (iq.eof()) {
			three = true;
			compute64(a, b, 0, ol,cont, two, Table, three);
			three = false;
			ol.put('=');
			break;
		}
		compute64(a, b, c, ol, cont, two, Table,three);

	}
}


void divideColor(image<rgb>& img) {
	image<uint8_t> imgR(img.width(),img.height());
	image<uint8_t> imgG(img.width(), img.height());
	image<uint8_t> imgB(img.width(), img.height());
	for (int i = 0; i < img.height(); ++i) {
		for (int j = 0; j < img.width(); ++j) {
			imgR(j, i) = img(j, i)[0];
		}
	}
	for (int i = 0; i < img.height(); ++i) {
		for (int j = 0; j < img.width(); j++) {
			imgG(j, i) = img(j, i)[1];
		}
	}
	for (int i = 0; i < img.height(); ++i) {
		for (int j = 0; j < img.width(); j++) {
			imgB(j, i) = img(j, i)[2];
		}
	}
	writePPMColor(imgR, "rosso");
	writePPMColor(imgG, "verde");
	writePPMColor(imgB, "blu");
	packbit p;
	ofstream ored("r.pkb", ios::binary);
	ofstream ogreen("g.pkb", ios::binary);
	ofstream oblue("b.pkb", ios::binary);
	//COMPRESS PACKBITS
	p.compress(imgG, ogreen );
	p.compress(imgR, ored);
	p.compress(imgB, oblue);

	ored.close();
	ogreen.close();
	oblue.close();


	//BASE 64
	ifstream ig("g.pkb", ios::binary);
	ofstream og("g.txt", ios::binary);
	encode64(ig,og);
	ifstream ir("r.pkb", ios::binary);
	ofstream or ("r.txt", ios::binary);
	encode64(ir, or );
	ifstream ib("b.pkb", ios::binary);
	ofstream ob("b.txt", ios::binary);
	encode64(ib, ob);

	ifstream prova("prova64.txt", ios::binary);
	ofstream oprova("prova.txt", ios::binary);
	encode64(prova, oprova);
}




int main(int argc, char **argv) {
	ifstream it(argv[1], ios::binary);
	auto img= readPPM(it);
	vector<image<uint8_t>> v;
	divideColor(img);



}