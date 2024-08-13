#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <iterator>
#include <fstream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <utility>

#include "bit.h"
using namespace std;

using element=pair<uint32_t,uint32_t>;
vector<element> v;
uint32_t len; // textLen
uint32_t rcount;

void syntax() {
	cout << "Syntax <input> <output>";
	exit(1);
}

void writeBOM(fstream& of) {
	uint8_t val = 0xEF;
	of.write(reinterpret_cast<char *>(&val), sizeof(val));
	uint8_t val2 = 0xBB;
	of.write(reinterpret_cast<char *>(&val2), sizeof(val2));
	uint8_t val3 = 0xBF;
	of.write(reinterpret_cast<char *>(&val3), sizeof(val3));
}



void readData(int rcount,bitreader& br,ifstream& it,fstream& of) {
	int c = 1;
	int cont = 0;
	uint8_t val = 0;
	uint8_t lunghezza = 0;
	vector<uint8_t> symbols;
	int distanza = 0;
	int shift = 0;
	int outputLen = 0;
	int totLen = 0;
	while (c < rcount+1) {
		outputLen = 0;
		it.seekg(v[c].first, it.beg);
		while (outputLen != 4096) {
			val = br(8);
			if (val == 0) {
				break;
			}
			else if (val >= 0x01 && val <= 0x08) {
				for (int t = 0; t < val; ++t) {
					uint8_t valore = br(8);
					of.put(valore);
					outputLen += 1;
					if (c == rcount - 1) totLen += 1;
					if (outputLen == 4096  || totLen==len) {
						break;
					}
					
				}
			}
			else if (val >= 0x09 && val <= 0x7F) {
				of.put(val);
				outputLen += 1;
				
				if (c == rcount - 1) totLen += 1;
				if (outputLen == 4096  || totLen == len) 
					break;
				
			}
			else if (val >= 0x80 && val <= 0xBF) {
				uint8_t primobyte = val;
				uint8_t secondobyte = br(8);
				uint16_t sequenza = 0;
				sequenza = (sequenza << 8) | primobyte;
				sequenza = (sequenza << 8) | secondobyte;
				lunghezza = sequenza & 0x7;
				lunghezza += 3;
				sequenza = sequenza >> 3;
				cont = 0;
				distanza = sequenza & 0x7FF;
				if (distanza == 0) distanza++;
				long pos = of.tellp();
				of.seekg(-distanza,of.cur);
				while (cont++ < distanza) {
					uint8_t a = of.get();
					symbols.push_back(a);
				}
				of.seekg(pos, of.beg); //torno alla posizione di prima
				int f = 0;
				for (int k = 0; k < lunghezza; ++k) {
					of.put(symbols[f]);
					outputLen += 1;
					if (c == rcount - 1) totLen += 1;
					if (outputLen == 4096  || totLen == len) {
						break;
					}
					f++;
					if (f == distanza) f = 0;
				}
				symbols.clear();
			}
			else if (val >= 0xC0 && val <= 0xFF) {
				of.put(' ');
				outputLen += 1;
				if (c == rcount - 1) totLen += 1;
				if (outputLen == 4096 || totLen == len) {
					break;
				}
				val = val & 0x7F;
				of.put(val);
				outputLen += 1;
				if (c == rcount - 1) totLen += 1;
				if (outputLen == 4096  || totLen == len) {
					break;
				}
			}
		}
		c++;
		totLen += 4096;
	}


}


void  readRecords(int n,bitreader& br,ifstream& it) {
	int c = 0;
	uint32_t id;
	while (c < n) {
		uint32_t offset = br(32);
		br(8);
		uint32_t id = br(24);
		cout << c << " - offset: " << offset << " - id: " << id << "\n";
		v.push_back(pair<uint32_t, uint32_t>(offset, id));
		c++;
	}
	it.seekg(v[0].first,it.beg);
	uint16_t compression=br(16);
	cout << "Compression: ";
	if (compression == 1) {
		cout << "No Compression \n";
	}
	if (compression == 2) {
		cout << "PalmDOC compression \n";
	}
	if (compression == 17480) {
		cout << "HUFF/CDIC compression \n";
	}
	//unsed
	br(16);
	//TextLenght
	len = br(32);
	cout << "Text Lenght: " << len << "\n";
	//RecordCount
	rcount = br(16);
	cout << "Record Count: " << rcount << "\n";
	//RecordSize
	uint32_t rsize = br(16);
	cout << "Record Size: " << rsize << "\n";
	//EncryptionType
	uint32_t etype = br(16);
	cout << "Encrpyption Type: ";
	if (etype == 0) {
		cout << "No Encryption \n";
	}
	if (etype == 1) {
		cout << "Old Mobipocket Encryption \n";
	}
	if (etype == 2) {
		cout << "Mobipocket Encryption \n";
	}
	//Unknow
	br(16);
}

uint32_t readPDM(ifstream& it,bitreader& br) {
	string nome;
	string type;
	string creator;
	string file_version;
	for (int c = 0; c < 32; ++c) {
		char t = br(8);
		nome.push_back(t);
	}
	cout << "PDB name: " << nome << "\n";
	//attributes	
	br(16);
	//version
	br(16);
	//creation date
	uint32_t sec = br(32);
	time_t now = sec;
	tm *ltm = localtime(&now);
	cout << "Creation Date: ";
	cout << ltm->tm_mday << "/"  << 1 + ltm->tm_mon << "/" << 1900 + (ltm->tm_year) - 4 << " ";
	cout << "Time: " << 1 + ltm->tm_hour << ":" << 1 + ltm->tm_min << ":" << 1 + ltm->tm_sec <<"\n";
	//modification date
	uint32_t sec2 = br(32);
	//last basckup date
	uint32_t sec3 = br(32);
	//Modification Number
	br(32);
	//App info id
	br(32);
	//Sort info id
	br(32);
	//type
	for (int c = 0; c < 4; ++c) {
		char t = br(8);
		type.push_back(t);
	}
	cout << "type: " << type<<"\n";
	//creator
	for (int c = 0; c < 4; ++c) {
		char t = br(8);
		creator.push_back(t);
	}
	cout << "creator: " << creator << "\n";
	//uniqueId
	br(32);
	//nextrecordlistId
	br(32);
	int number=br(16);
	cout << "Records: " << number <<"\n";
	return number;
}

int main(int argc, char **argv) {
	if (argc != 3) {
		syntax();
	}
	ifstream it(argv[1], ios::binary);
	if (!it) {
		syntax();
	}
	fstream of(argv[2], ios::in | ios::app | ios::out);
	writeBOM(of);
	bitreader br(it);
	int n=readPDM(it,br);
	readRecords(n,br,it);
	readData(rcount, br, it,of);
}