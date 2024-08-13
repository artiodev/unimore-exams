#include <string>
#include <iostream>
#include <fstream>
#include "image.h"
#include <iterator>
#include <algorithm>
#include <sstream>
using namespace std;

void syntax() {
	cout << "Input non sufficienti \n";
	exit(2);
}

template<typename T>
istream& read(istream& it, T& val) {
	return it.read(reinterpret_cast<char*>(&val), sizeof(val));
}
template<typename T>
ostream&  write(ostream& it, T& val) {
	return it.write(reinterpret_cast<const char*>(&val), sizeof(val));
}

image<uint8_t> readPGM(ifstream& it) {
	size_t w;
	size_t h;
	size_t max_number;

	string magic;
	getline(it, magic);
	bool plain = false;
	if (magic == "P5") {
		plain = false;
	}
	else if (magic == "P2") {
		plain = true;
	}

	while (it.peek()=='#') {
		string comment;
		getline(it, comment);
	}
	it >> w >> h >> max_number;
	it.get(); // "\n"
	image<uint8_t> img(w, h);

	it.read(img.dataptr(), img.datasize());
	return img;
}


void compress(ifstream& it,ofstream& of) {
	uint32_t h;
	uint32_t w;
	vector<uint8_t> primo;
	vector<uint8_t> secondo;
	vector<uint8_t> terzo;
	vector<uint8_t> quarto;
	vector<uint8_t> quinto;
	vector<uint8_t> sesto;
	vector<uint8_t> settimo;
	auto img=readPGM(it);
	of << "MULTIRES";
	h = img.height();
	w = img.width();
	write(of, w);
	write(of, h);

	for (int i = 0; i < img.height(); i+=8) {
		for (int j = 0; j < img.width(); j+=8) {
			primo.push_back(img(j, i));
		}
	}
	for (int i = 0; i < img.height(); i+=8) {
		for (int j = 4; j < img.width(); j +=8 ) {
			secondo.push_back(img(j, i));
		}
	}
	for (int i = 4; i < img.height(); i += 8) {
		for (int j = 0; j < img.width(); j += 4) {
			terzo.push_back(img(j, i));
		}
	}
	for (int i = 0; i < img.height(); i += 4) {
		for (int j = 2; j < img.width(); j += 4) {
			quarto.push_back(img(j, i));
		}
	}
	for (int i = 2; i < img.height(); i += 4) {
		for (int j = 0; j < img.width(); j += 2) {
			quinto.push_back(img(j, i));
		}
	}
	for (int i = 0; i < img.height(); i += 2) {
		for (int j = 1; j < img.width(); j += 2) {
			sesto.push_back(img(j, i));
		}
	}
	for (int i = 1; i < img.height(); i += 2) {
		for (int j = 0; j < img.width(); j++) {
			settimo.push_back(img(j, i));
		}
	}
	
	for (auto x : primo) {
		of << x;
	}
	for (auto x : secondo) {
		of << x;
	}
	for (auto x : terzo) {
		of << x;
	}
	for (auto x : quarto) {
		of << x;
	}
	for (auto x : quinto) {
		of << x;
	}
	for (auto x : sesto) {
		of << x;
	}
	for (auto x : settimo) {
		of << x;
	}
}
void writePGM(image<uint8_t>& img,string filename) {
	ofstream of(filename, ios::binary);
	of << "P5\n";
	of << img.width() << " " << img.height() << "\n";
	of << "255\n";
	of.write(img.dataptr(), img.datasize());
}


void decompress(ifstream& it, string prefisso) {
	stringstream ss;
	uint32_t w;
	uint32_t h;
	uint8_t pixel;
	ss << prefisso;
	ss << "_7.pgm";
	string filename = ss.str();
	string magic;
	while (magic != "MULTIRES")
		magic.push_back(it.get());

	cout << magic << "\n";
	read(it, w);
	cout << w << "\n";
	read(it, h);
	cout << h << "\n";
	image<uint8_t> img(w, h);
	//Interpolo per la rec_7
	for (int i = 0; i < img.height(); i += 8) {
		for (int j = 0; j < img.width(); j += 8) {
			pixel=it.get();
			img(j, i) = pixel;
		}
	}
	for (int i = 0; i < img.height(); i += 8) {
		for (int j = 4; j < img.width(); j += 8) {
			pixel = it.get();
			img(j, i) = pixel;
		}
	}
	for (int i = 4; i < img.height(); i += 8) {
		for (int j = 0; j < img.width(); j += 4) {
			pixel = it.get();
			img(j, i) = pixel;
		}
	}
	for (int i = 0; i < img.height(); i += 4) {
		for (int j = 2; j < img.width(); j += 4) {
			pixel = it.get();
			img(j, i) = pixel;
		}
	}
	for (int i = 2; i < img.height(); i += 4) {
		for (int j = 0; j < img.width(); j += 2) {
			pixel = it.get();
			img(j, i) = pixel;
		}
	}
	for (int i = 0; i < img.height(); i += 2) {
		for (int j = 1; j < img.width(); j += 2) {
			pixel = it.get();
			img(j, i) = pixel;
		}
	}
	for (int i = 1; i < img.height(); i += 2) {
		for (int j = 0; j < img.width(); j++) {
			pixel = it.get();
			img(j, i) = pixel;
		}
	}
	writePGM(img, filename);
	image<uint8_t> img2(w, h);
	for (int i = 0; i < img.height(); i+=2) {
		for (int j = 0; j < img.width(); j ++) {
			img2(j, i) = img(j, i);
		}
	}
	for (int i = 1; i < img.height(); i += 2) {
		for (int j = 0; j < img.width(); j++) {
			img2(j, i) = img2(j,i-1);
		}
	}
	ss.str("");
	ss << prefisso;
	ss << "_6.pgm";
	string filename2 = ss.str();
	writePGM(img2, filename2);

	image<uint8_t> img3(w, h);
	for (int i = 0; i < img.height(); i +=2) {
		for (int j = 0; j < img.width(); j+=2) {

			img3(j, i) = img2(j, i);
			img3(j, ((i + 1) <= img.height() - 1) ? i + 1 : i) = img2(j, i);
			img3(((j + 1 <= img.width() - 1) ? j + 1 : j), i) = img2(j, i);
			img3(((j + 1 <= img.width() - 1) ? j + 1 : j), ((i + 1) <= img.height() - 1) ? i + 1 : i) = img2(j, i);

		}
	}
	ss.str("");
	ss << prefisso;
	ss << "_5.pgm";
	string filename3 = ss.str();
	writePGM(img3, filename3);

	image<uint8_t> img5(w, h);

	for (int i = 0; i < img.height(); i+=4) {
		for (int j = 0; j < img.width(); j+=2) {

			img5(j, i) = img3(j, i);
			img5(j, ((i + 1) <= img.height()- 1) ? i +1 :i) = img3(j, i);
			img5(j , ((i + 2) <= img.height() - 1) ? i + 2 : i) = img3(j, i);
			img5(j , ((i + 3) <= img.height() - 1) ? i + 3 : i) = img3(j, i);

			img5(((j + 1 <= img.width() - 1) ? j + 1 : j), i) = img3(j, i);
			img5(((j + 1 <= img.width() - 1) ? j + 1 : j), ((i + 1) <= img.height() - 1) ? i + 1 : i) = img3(j, i);
			img5(((j + 1 <= img.width() - 1) ? j + 1 : j), ((i + 2) <= img.height() - 1) ? i + 2 : i) = img3(j, i);
			img5(((j + 1 <= img.width() - 1) ? j + 1 : j), ((i + 3) <= img.height() - 1) ? i + 3 : i) = img3(j, i);
		}
	}

	ss.str("");
	ss << prefisso;
	ss << "_4.pgm";
	string filename5 = ss.str();
	writePGM(img5, filename5);


	image<uint8_t> img6(w, h);

	for (int i = 0; i < img.height(); i += 4) {
		for (int j = 0; j < img.width(); j += 4) {
			img6(j, i) = img5(j, i);

			img6(j, ((i + 1) <= img.height() - 1) ? i + 1 : i) = img5(j, i);
			img6(j, ((i + 2) <= img.height() - 1) ? i + 2 : i) = img5(j, i);
			img6(j, ((i + 3) <= img.height() - 1) ? i + 3 : i) = img5(j, i);


			img6((((j + 1) <= img.width() - 1) ? j + 1 : j), i) = img5(j, i);
			img6((((j + 2) <= img.width() - 1) ? j + 2 : j), i) = img5(j, i);
			img6((((j + 3) <= img.width() - 1) ? j + 3 : j), i) = img5(j, i);

			img6((((j + 1) <= img.width() - 1) ? j + 1 : j), ((i + 1) <= img.height() - 1) ? i + 1 : i) = img5(j, i);
			img6((((j + 2) <= img.width() - 1) ? j + 2 : j), ((i + 1) <= img.height() - 1) ? i + 1 : i) = img5(j, i);
			img6((((j + 3) <= img.width() - 1) ? j + 3 : j), ((i + 1) <= img.height() - 1) ? i + 1 : i) = img5(j, i);


			img6((((j + 1) <= img.width() - 1) ? j + 1 : j), ((i + 2) <= img.height() - 1) ? i + 2 : i) = img5(j, i);
			img6((((j + 2) <= img.width() - 1) ? j + 2 : j), ((i + 2) <= img.height() - 1) ? i + 2 : i) = img5(j, i);
			img6((((j + 3) <= img.width() - 1) ? j + 3 : j), ((i + 2) <= img.height() - 1) ? i + 2 : i) = img5(j, i);
		
			img6((((j + 1) <= img.width() - 1) ? j + 1 : j), ((i + 3) <= img.height() - 1) ? i + 3 : i) = img5(j, i);
			img6((((j + 2) <= img.width() - 1) ? j + 2 : j), ((i + 3) <= img.height() - 1) ? i + 3 : i) = img5(j, i);
			img6((((j + 3) <= img.width() - 1) ? j + 3 : j), ((i + 3) <= img.height() - 1) ? i + 3 : i) = img5(j, i);
		}
	}

	ss.str("");
	ss << prefisso;
	ss << "_3.pgm";
	string filename6 = ss.str();
	writePGM(img6, filename6);

	image<uint8_t> img7(w, h);

	for (int i = 0; i < img.height(); i += 8) {
		for (int j = 0; j < img.width(); j += 4) {
			img7(j, i) = img6(j, i);

			img7((((j + 1) <= img.width() - 1) ? j + 1 : j), i) = img6(j, i);
			img7((((j + 2) <= img.width() - 1) ? j + 2 : j), i) = img6(j, i);
			img7((((j + 3) <= img.width() - 1) ? j + 3 : j), i) = img6(j, i);

			img7(j, ((i + 1) <= img.height() - 1) ? i + 1 : i) = img6(j, i);
			img7((((j + 1) <= img.width() - 1) ? j + 1 : j), ((i + 1) <= img.height() - 1) ? i + 1 : i) = img6(j, i);
			img7((((j + 2) <= img.width() - 1) ? j + 2 : j), ((i + 1) <= img.height() - 1) ? i + 1 : i) = img6(j, i);
			img7((((j + 3) <= img.width() - 1) ? j + 3 : j), ((i + 1) <= img.height() - 1) ? i + 1 : i) = img6(j, i);

			//
			img7(j, ((i + 2) <= img.height() - 1) ? i + 2 : i) = img6(j, i);
			img7((((j + 1) <= img.width() - 1) ? j + 1 : j), ((i + 2) <= img.height() - 1) ? i + 2 : i) = img6(j, i);
			img7((((j + 2) <= img.width() - 1) ? j + 2 : j), ((i + 2) <= img.height() - 1) ? i + 2 : i) = img6(j, i);
			img7((((j + 3) <= img.width() - 1) ? j + 3 : j), ((i + 2) <= img.height() - 1) ? i + 2 : i) = img6(j, i);
			//
			img7(j, ((i + 3) <= img.height() - 1) ? i + 3 : i) = img6(j, i);
			img7((((j + 1) <= img.width() - 1) ? j + 1 : j), ((i + 3) <= img.height() - 1) ? i + 3 : i) = img6(j, i);
			img7((((j + 2) <= img.width() - 1) ? j + 2 : j), ((i + 3) <= img.height() - 1) ? i + 3 : i) = img6(j, i);
			img7((((j + 3) <= img.width() - 1) ? j + 3 : j), ((i + 3) <= img.height() - 1) ? i + 3 : i) = img6(j, i);
			//
			img7(j, ((i + 4) <= img.height() - 1) ? i + 4 : i) = img6(j, i);
			img7((((j + 1) <= img.width() - 1) ? j + 1 : j), ((i + 4) <= img.height() - 1) ? i + 4 : i) = img6(j, i);
			img7((((j + 2) <= img.width() - 1) ? j + 2 : j), ((i + 4) <= img.height() - 1) ? i + 4 : i) = img6(j, i);
			img7((((j + 3) <= img.width() - 1) ? j + 3 : j), ((i + 4) <= img.height() - 1) ? i + 4 : i) = img6(j, i);

			img7(j, ((i + 5) <= img.height() - 1) ? i + 5 : i) = img6(j, i);
			img7((((j + 1) <= img.width() - 1) ? j + 1 : j), ((i + 5) <= img.height() - 1) ? i + 5 : i) = img6(j, i);
			img7((((j + 2) <= img.width() - 1) ? j + 2 : j), ((i + 5) <= img.height() - 1) ? i + 5 : i) = img6(j, i);
			img7((((j + 3) <= img.width() - 1) ? j + 3 : j), ((i + 5) <= img.height() - 1) ? i + 5 : i) = img6(j, i);
			//
			img7(j, ((i + 6) <= img.height() - 1) ? i + 6 : i) = img6(j, i);
			img7((((j + 1) <= img.width() - 1) ? j + 1 : j), ((i + 6) <= img.height() - 1) ? i + 6 : i) = img6(j, i);
			img7((((j + 2) <= img.width() - 1) ? j + 2 : j), ((i + 6) <= img.height() - 1) ? i + 6 : i) = img6(j, i);
			img7((((j + 3) <= img.width() - 1) ? j + 3 : j), ((i + 6) <= img.height() - 1) ? i + 6 : i) = img6(j, i);
			//
			img7(j, ((i + 7) <= img.height() - 1) ? i + 7 : i) = img6(j, i);
			img7((((j + 1) <= img.width() - 1) ? j + 1 : j), ((i + 7) <= img.height() - 1) ? i + 7 : i) = img6(j, i);
			img7((((j + 2) <= img.width() - 1) ? j + 2 : j), ((i + 7) <= img.height() - 1) ? i + 7 : i) = img6(j, i);
			img7((((j + 3) <= img.width() - 1) ? j + 3 : j), ((i + 7) <= img.height() - 1) ? i + 7 : i) = img6(j, i);

		}
	}

	ss.str("");
	ss << prefisso;
	ss << "_2.pgm";
	string filename7 = ss.str();
	writePGM(img7, filename7);

	
}


int main(int argc, char **argv) {
	
	if (argc != 4) {
		syntax();
	}

	if (argv[1][0] == 'c' ) {
		ifstream it(argv[2], ios::binary);
		ofstream ot(argv[3], ios::binary);
		compress(it, ot);
	}
	else if (argv[1][0] == 'd') {
		ifstream it(argv[2], ios::binary);
		string prefisso = argv[3];
		decompress(it, prefisso);
	}
	
}