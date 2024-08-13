//Esame del 25 agosto 2015

// Artioli Gianluca

#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cstdint>
#include <istream>
#include <iostream>
#include <vector>

#include "image.h"
#include "bit.h"
using namespace std;

uint16_t len = 0;
string valueF;
uint16_t var;
uint32_t val;

void syntax() {
	cout << "syntax: ppm2dicm *.ppm *.dcm \n";
	EXIT_FAILURE;
}

template<typename T>
istream& read(istream& is, T& val) {
	return is.read(reinterpret_cast<char*>(&val), sizeof val);
}
template<typename T>
ostream& write(ostream& os, const T& val) {
	return os.write(reinterpret_cast<const char*>(&val), sizeof val);
}



image<rgb> readPPM(ifstream& ifs) {
	string magic;
	getline(ifs, magic);
	bool plain;

	if (magic == "P6") {
		plain = false;
	}
	else {
		plain = true;
	}
	string comment;
	while ( ifs.peek() == '#') {
		getline(ifs, comment);
	}
	size_t w, h, n;

	ifs >> w >> h >> n;

	ifs.get(); 
	image<rgb> img(w, h);

	for (size_t y = 0; y < h; ++y) {
		for (size_t x = 0; x < w; ++x) {
			for (uint8_t k = 0; k < 3; ++k) {
				img(x, y)[k] = ifs.get();
			}
		}
	}
	return img;
}




void check(uint16_t len,ofstream& ofs,string& valueF,bitwriter& bw) {
	if (len % 2 != 0) {
		len++;
		write(ofs, len);
		for (const auto &x : valueF) {
			ofs.put(x);
		}
		bw(0, 8);
	}
	else {
		var = len;
		write(ofs, len);
		for (const auto &x : valueF) {
			ofs.put(x);
		}
	}
}



void checkCS(uint16_t len, ofstream& ofs, string& valueF, bitwriter& bw) {
	if (len % 2 != 0) {
		len++;
		write(ofs, len);
		valueF.push_back(' ');
		for (const auto &x : valueF) {
			ofs.put(x);
		}
	}
	else {
		write(ofs, len);
		for (const auto &x : valueF) {
			ofs.put(x);
		}
	}

}

void PPMtoDCM(ofstream& ofs,image<rgb> img) {
	uint8_t byte = 0;
	int c = 0;
	while (c < 128) {
		write(ofs, byte);
		c++;
	}
	ofs.put('D'); ofs.put('I'); ofs.put('C'); ofs.put('M');

	bitwriter bw(ofs);

	//GRUPPPO2
	//File Meta Information
	cout << ".";
	var = 2;
	write(ofs, var);
	var = 0;
	write(ofs, var);
	ofs.put('U'); ofs.put('L');
	var = 4;
	write(ofs, var);
	val = 164;
	write(ofs, val);
	//File Meta Version
	cout << ".";
	var = 2;
	write(ofs, var);
	var = 1;
	write(ofs, var);
	ofs.put('O'); ofs.put('B');
	bw(0, 16); 
	val = 2;
	write(ofs, val);
	bw(0, 8); 
	bw(1, 8);  
	//MediaStorage SOP Class
	cout << ".";
	var = 2;
	write(ofs, var);
	write(ofs, var);
	ofs.put('U'); ofs.put('I');
	valueF = "1.2.840.10008.5.1.4.1.1.77.1.4";
	var = uint16_t(valueF.size());
	check(var, ofs, valueF, bw);
	//MediaStorage SOP Instance
	cout << ".";
	var = 2;
	write(ofs, var);
	var = 3;
	write(ofs, var);
	ofs.put('U'); ofs.put('I');
	valueF = "1.2.392.200036.9125.0.19950720112207";
	len = uint16_t(valueF.size());
	check(len, ofs, valueF, bw);
	//Transfer Syntax UID
	cout << ".";
	var = 2;
	write(ofs, var);
	var = 16;
 	write(ofs, var);
	ofs.put('U'); ofs.put('I');
	valueF = "1.2.840.10008.1.2.1";
	len = uint16_t(valueF.size());
	check(len, ofs, valueF, bw);
	//Implementation Class
	var = 2;
	write(ofs, var);
	var = 18;
	write(ofs, var);
	ofs.put('U'); ofs.put('I');
	valueF = "1.2.392.200036.9125.0.1234567890";
	len = uint16_t(valueF.size());
	check(len, ofs, valueF, bw);
	//GRUPPO8
	//Image Type
	cout << ".";
	var = 8;
	write(ofs, var);
	var = 8;
	write(ofs, var);
	ofs.put('C'); ofs.put('S');
	valueF = "ORIGINAL\\PRIMARY";
	len = uint16_t(valueF.size());
	checkCS(len, ofs, valueF, bw);
	//SOP class UID
	cout << ".";
	var = 8;
	write(ofs, var);
	var = 22;
	write(ofs, var);
	ofs.put('U'); ofs.put('I');
	valueF = "1.2.840.10008.5.1.4.1.1.77.1.4";
	len = uint16_t(valueF.size());
	check(len, ofs, valueF, bw);
	//SOP Instance UID
	var = 8;
	write(ofs, var);
	var = 24;
	write(ofs, var);
	ofs.put('U'); ofs.put('I');
	valueF = "1.2.392.200036.9125.0.19950720112207";
	len = uint16_t(valueF.size());
	check(len, ofs, valueF, bw);
	//GRUPPO 28
	//Sample per Pixel
	cout << ".";
	var = 40;
	write(ofs, var);
	var = 2;
	write(ofs, var);
	ofs.put('U'); ofs.put('S');
	var = 2;
	write(ofs, var);
	var = 3;
	write(ofs, var);
	//Photometric Interpetation
	cout << ".";
	var = 40;
	write(ofs, var);
	var = 4;
	write(ofs, var);
	ofs.put('C'); ofs.put('S');
	valueF = "RGB";
	len = uint16_t(valueF.size());
	checkCS(len, ofs, valueF, bw);
	//Planar Configuration
	cout << ".";
	var = 40;
	write(ofs, var);
	var = 6;
	write(ofs, var);
	ofs.put('U'); ofs.put('S');
	var = 2;
	write(ofs, var);
	var = 0;
	write(ofs, var);
	//Rows
	cout << ".";
	var = 40;
	write(ofs, var);
	var = 16;
	write(ofs, var);
	ofs.put('U'); ofs.put('S');
	var = 2;
	write(ofs, var);
	var = uint16_t(img.height());
	write(ofs, var);
	//Colimns
	cout << ".";
	var = 40;
	write(ofs, var);
	var = 17;
	write(ofs, var);
	ofs.put('U'); ofs.put('S');
	var = 2;
	write(ofs, var);
	var = uint16_t(img.width());
	write(ofs, var);
	//bit alloc
	var = 40;
	write(ofs, var);
	var = 256;
	write(ofs, var);
	ofs.put('U'); ofs.put('S');
	var = 2;
	write(ofs, var);
	var = 8;
	write(ofs, var);
	//Bits Allocated
	cout << ".";
	var = 40;
	write(ofs, var);
	var = 257;
	write(ofs, var);
	ofs.put('U'); ofs.put('S');
	var = 2;
	write(ofs, var);
	var = 8;
	write(ofs, var);
	//Hight Bit
	cout << ".";
	var = 40;
	write(ofs, var);
	var = 258;
	write(ofs, var);
	ofs.put('U'); ofs.put('S');
	var = 2;
	write(ofs, var);
	var = 7;
	write(ofs, var);
	//Pizel Rapresentation
	cout << ".";
	var = 40;
	write(ofs, var);
	var = 259;
	write(ofs, var);
	ofs.put('U'); ofs.put('S');
	var = 2;
	write(ofs, var);
	var = 0;
	write(ofs, var);
	//Lossy image Compression
	cout << ".";
	var = 40;
	write(ofs, var);
	var = 8464;
	write(ofs, var);
	ofs.put('C'); ofs.put('S');
	valueF = "00";
	len = uint16_t(valueF.size());
	checkCS(len, ofs, valueF, bw);
	//Pixel Data
	cout << ".";
	var = 32736;
	write(ofs, var);
	var = 16;
	write(ofs, var);
	ofs.put('O'); ofs.put('B');
	var = 0;
	write(ofs, var);
	val = img.height()*img.width() * 3;
	write(ofs, val);
	ofs.write(img.dataptr(), img.datasize());
}




int main(int argc, char **argv) {
	if (argc != 4) {
		syntax();
	}
	ifstream ifs(argv[2], ios::binary);
	if (!ifs) {
		cout << "Errore nel caricamento del file \n";
		EXIT_FAILURE;
	}
	auto img = readPPM(ifs);
	cout << "Start Conversion: ";
	ofstream ofs(argv[3], ios::binary);
	PPMtoDCM(ofs,img);
	cout << " Done\n";

}