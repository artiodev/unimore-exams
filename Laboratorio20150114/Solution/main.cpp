#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <functional>
#include <exception>
#include <sstream>

using namespace std;

#include "ppm.h"
#include "bitstreams.h"
size_t width;
size_t height;

class ObjectJSON {
	vector<string> value;
	string name;
	vector<uint8_t> v;
	vector<int> dati;

public:
	ObjectJSON(){}
	ObjectJSON(string name) : name(name) {}
	void addValue(string v) {
		value.push_back(v);
	}
	void addArray(uint8_t t) {
		v.push_back(t);
	}
	void addDati(int d) {
		dati.push_back(d);
	}
	vector<string> getValue() {
		return value;
	}
	vector<uint8_t> getArray() {
		return v;
	}
	vector<int> getDati() {
		return dati;
	}
	string getName() {
		return name;
	}
};



void decodeArray(bitreader& br, ObjectJSON& o) {
	int val;
	br(8);
	br(8); // tyoe array nsotro caso sempre uint8_t
	br(8);
	br(8);
	if (o.getValue()[o.getValue().size() - 1] == "data") {
		int bit = (width*height * 3);
		if (bit > 255 && bit < 65535) val = (br(16));
		else if (bit < 256) val = br(8);
		else val = br(32);
	}
	else {
		val = br(8);
	}
	while (val-- > 0) o.addArray(br(8));
}

void decodeInt(bitreader& br, ObjectJSON& o) {
	uint8_t dato;
	dato = br(8);
	if (o.getValue()[o.getValue().size() - 1] == "width") {
		width = dato;
	}
	if (o.getValue()[o.getValue().size() - 1] == "height") {
		height = dato;
	}
	o.addDati(dato);
}


void decodeInt16(bitreader& br, ObjectJSON& o) {
	uint16_t dato;
	dato = br(16);
	if (o.getValue()[o.getValue().size() - 1] == "width") {
		width = dato;
	}
	if (o.getValue()[o.getValue().size() - 1] == "height") {
		height = dato;
	}
	o.addDati(dato);
}

void decodeInt32(bitreader& br, ObjectJSON& o) {
	uint32_t dato;
	dato = br(32);
	if (o.getValue()[o.getValue().size() - 1] == "width") {
		width = dato;
	}
	if (o.getValue()[o.getValue().size() - 1] == "height") {
		height = dato;
	}
	o.addDati(dato);
}

void decodeU(bitreader& br, ObjectJSON& o) {
	uint8_t dato;
	dato = br(8);
	if (o.getValue()[o.getValue().size() - 1] == "width") {
		width = dato;
	}
	if (o.getValue()[o.getValue().size() - 1] == "height") {
		height = dato;
	}
	o.addDati(dato);
}
void decodef32(bitreader& br, ObjectJSON& o) {
	float dato;
	dato = br(32);
	if (o.getValue()[o.getValue().size() - 1] == "width") {
		width = dato;
	}
	if (o.getValue()[o.getValue().size() - 1] == "height") {
		height = dato;
	}
	o.addDati(dato);
}
void decodef64(bitreader& br, ObjectJSON& o) {
	double dato;
	dato = br(64);
	if (o.getValue()[o.getValue().size() - 1] == "width") {
		width = dato;
	}
	if (o.getValue()[o.getValue().size() - 1] == "height") {
		height = dato;
	}
	o.addDati(dato);
}
int convert(const string& sInput, const string& sOutput) {


	ifstream it(sInput, ios::binary);
	vector<image<vec3b>> vI;

	bitreader br(it);
	char type;
	uint32_t val;
	string name;
	string value;
	stringstream ss;
	int parentesi = 1;


	vector<uint8_t> v;

	unsigned w ; // TODO : modificare
	unsigned h ; // TODO : modificare

	br(8);
	br(8);
	val=br(8);
	while (val-- > 0) ss << char(br(8));
	name = ss.str();
	ObjectJSON o(name);
	br(8);

	//Leggo Canvas
	while (true) {
		if (parentesi == 2) break;
		char a=br(8);
		if (a != '}') {
			val=br(8); //i
			ss.str("");
			while (val-- > 0)
			ss << char(br(8));
			o.addValue(ss.str());
			type=br(8); // type catergory
			if (type == 'i') 
			decodeInt(br, o);
			else if (type == '[') 
			decodeArray(br, o);
			else if(type=='I')
			decodeInt16(br, o);
			else if (type == 'l')
			decodeInt32(br, o);
			else if (type = 'U')
			decodeU(br, o);
			else if(type='d')
			decodef32(br, o);
			else if(type='D')
			decodef64(br, o);
		}
		else {
			parentesi++;
		}

	}
	
	vector<int> v1 = o.getDati();
	w = v1[0];
	h = v1[1];
	vector<uint8_t> ar = o.getArray();

	image<vec3b> img(w, h);

	for (int i = 0; i < img.height(); ++i) {
		for (int j= 0; j < img.width(); ++j) {
			for (int k = 0; k < 3; ++k) {
				img(j, i)[k] = ar[k];
			}
		}
	}
	image<vec3b> SFONDO(w, h);
	SFONDO = img;
	// Output in formato PPM
	if (!writeP6(sOutput, img))
		return EXIT_FAILURE;

	// Decodifico oggetti dentro element dopo aver decodificato element stesso
	br(8);
	val = br(8);
	ss.str("");
	while (val-- > 0) ss << char(br(8));
	int parantesiElement = 1;
	parentesi = 1;
	vector<ObjectJSON> vO;
	br(8);
	char p;

	bool finito = false;
	while (true) {
		
		ss.str("");
		char p=br(8);
		if (p == '}') 
			parantesiElement++;
		if (parantesiElement == 2) {
			break;
		}
		val = br(8);
		while (val-- > 0) ss << char(br(8));
		name = ss.str();
		ObjectJSON o(name);
		br(8);
		parentesi = 1;
		while (true) {
			if (parentesi == 2) {
				vO.push_back(o);
				break;
			}
			char a = br(8);
			if (a != '}') {
				finito = false;
				val = br(8); //i
				ss.str("");
				while (val-- > 0)
					ss << char(br(8));
				o.addValue(ss.str());
				type = br(8); // type catergory
				if (type == 'i')
					decodeInt(br, o);
				else if (type == '[')
					decodeArray(br, o);
				else if (type == 'I')
					decodeInt16(br, o);
				else if (type == 'l')
					decodeInt32(br, o);
				else if (type = 'U')
					decodeU(br, o);
				else if (type = 'd')
					decodef32(br, o);
				else if (type = 'D')
					decodef64(br, o);
			}
			else {
				parentesi++;
			}
		}
	}
	br(8); //Ultima parentesi

	size_t x;
	size_t y;
	size_t wit;
	size_t heig;
	int r = 0;
	vector<size_t> X;
	vector<size_t> Y;

	for (int i = 0; i < vO.size();++i) {
		int cont = 0;
		ss.str("");
		if (vO[i].getName() == "image") {
			for (int k = 0; k < vO[i].getValue().size(); ++k) {
				if (vO[i].getValue()[k] == "x") x = vO[i].getDati()[k];
				if (vO[i].getValue()[k] == "y") y = vO[i].getDati()[k];
				if (vO[i].getValue()[k] == "width") wit = vO[i].getDati()[k];
				if (vO[i].getValue()[k] == "height") heig = vO[i].getDati()[k];
			}
			X.push_back(x);
			Y.push_back(y);
			image<vec3b> img(wit, heig);
			for (int t = 0; t < img.height(); ++t) {
				for (int j = 0; j < img.width(); ++j) {
					for (int k = 0; k < 3; ++k) {
						img(j, t)[k] = vO[i].getArray()[cont];
						cont++;
					}
				}
			}
			vI.push_back(img);
			ss << i + 1;
			string filename = "image" + ss.str() + ".ppm";
			if (!writeP6(filename, img))
				return EXIT_FAILURE;
		}
	}
	
	image<vec3b> img2(w, h);
	int cont = 0;
	int c = 0;
	//Copio Sfondo
	for (int i = 0; i < img2.height(); ++i) {
		for (int j = 0; j < img2.width(); ++j) {
			for (int k = 0; k < 3; ++k) {
				img2(j, i)[k] = SFONDO(j,i)[k];
			}
		}
	}
	//Sovrapongo immagini
	for (int i = 0; i < img.height(); ++i) {
		for (int j = 0; j < img.width(); ++j) {
			if (c < X.size()) {
				x = X[c];
				y = Y[c];
			}
			if (j == x && i == y) {
				for (int t = 0; t < vI[cont].height(); ++t) {
					for (int f = 0; f < vI[cont].width(); ++f) {
						for (int k = 0; k < 3; ++k) {
							img2(j+f,i+t)[k] = vI[cont](f, t)[k];
						}
					}
				}
				cont++;
				c++;
			}
		}
	}

	if (!writeP6(sOutput, img2))
		return EXIT_FAILURE;


	//Print console
	for (int i = 0; i < vO.size();++i) {
		cout << vO[i].getName() << ": ";
		for (int k = 0; k < vO[i].getValue().size(); ++k) {
			cout << vO[i].getValue()[k] << ", ";
		}
		cout << "\n";
	}




	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {

	if (argc!= 3) {
		cout << "ERRORE POCH PARAMETRI IN INPUT \n";
		exit(1);
	}

		
	string sInput = argv[1];
	string sOutput = argv[2];

	return convert(sInput, sOutput);
}