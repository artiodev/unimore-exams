#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include "image.h"
#include "bit.h"

using namespace std;


void syntax() {
	cout << "syntax: <inputfile> <prefissoouput>";
	EXIT_FAILURE;
}

struct huffstr {
	uint8_t red;
	uint8_t gb;
	uint8_t len;
	uint32_t code;

	huffstr(uint8_t r, uint8_t b, uint8_t l) :red(r), gb(b), len(l) {}
	huffstr() {}
};

struct rgb2 {
	uint8_t red;
	uint8_t blue;
	uint8_t green;

	rgb2(uint8_t r, uint8_t b, uint8_t g) :red(r), blue(b), green(g) {}
};


void writePPM(vector<image<rgb>>& v,string prefisso) {
	stringstream ss;
	for (size_t i= 0; i < v.size(); i++) {
		if (i <= 9) {
			ss << "00" << i+1;
		}
		else if (i > 9 && i <= 99) {
			ss << "0" << i+1;
		}
		else if (i > 99) {
			ss << i+1;
		}
		string code = ss.str();
		ss.str("");
		string filename = prefisso + code;
		ofstream of(filename+".ppm", ios::binary);
		

		of << "P6" << "\n";
		of << v[i].width() << " " << v[i].height() << "\n";
		of << "255" << "\n";

		for (size_t x = 0; x < v[i].height(); ++x) {
			for (size_t y = 0; y < v[i].width(); ++y) {
				for (size_t k = 0; k < 3; ++k) {
					of.put(v[i](y, x)[k]);
				}
			}
		}
	}


}



vector<image<rgb>> readimage(ifstream& it,vector<huffstr>& v) {
	bitreader br(it);
	size_t w, h, numimages,numelement;
	uint8_t r, b, len;
	vector<image<rgb>> images;
	vector<huffstr>::iterator its;
	//leggo magic number;
	br(48);

	//leggo larghezza e altezza;
	w = br(32);
	h = br(32);
	numimages = br(10);
	numelement = br(16);
	for (size_t c = 0; c < numelement; ++c) {
		r = br(8);
		b = br(8);
		len = br(5);
		v.push_back(huffstr(r, b, len));
	}
	//Calcolo i codici canonici
	size_t lung = 0;
	uint32_t code=0;
	for (auto &x : v) {
		while (x.len > lung) {
			lung++;
			code <<= 1;
		}
		x.code = code;
		code++;
	}
	size_t t = 0;
	size_t cont = 0;
	uint32_t buf = 0;
	int shift = 0;
	bool trovato = false;
	vector<rgb2> val;
	//Vettore RGB
	while (true) {
			if (it.eof() && buf==0) {
				break;
			}
			buf = (buf << 1) | br(1);
			shift++;
			auto its = find_if(v.begin(), v.end(),
				[&buf,&shift](huffstr v) {
				return (v.code == buf && v.len == shift);
			});
			if (its != v.end()) {
				buf = 0; shift = 0;
				val.push_back(rgb2((*its).red,(*its).gb,(*its).gb));
			}
			/*
			for (auto &x : v) {
				if (buf == x.code && shift==x.len) {
					buf = 0; shift = 0;
					val.push_back(rgb2(x.red, x.gb, x.gb));
				}
			}
			*/
		}
	int k = 0;
	while (t < numimages) {
		image<rgb> img(w, h);
	
		for (size_t y = 0; y < h; ++y) {
			for (size_t x = 0; x < w; ++x) {
				img(x, y)[0] = val[k].red;
				img(x, y)[1] = val[k].blue;
				img(x, y)[2] = val[k].green;
				k++;
			}
		}
		images.push_back(img);
		t++;
	}
	return images;
}



int main(int argc, char** argv) {
	if (argc != 3) {
		syntax();
	}
	ifstream it(argv[1], ios::binary);
	if (!it) syntax();
	string prefisso = argv[2];
	vector<huffstr> v;


	auto img = readimage(it,v);
	writePPM(img,prefisso);

}