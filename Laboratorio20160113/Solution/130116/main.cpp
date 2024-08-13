#include <iostream>
#include <string>

#include "bit.h"
#include "rle.h"

using namespace std;


void syntax() {
	cout << "syntax inputfile outpuitfile";
	EXIT_FAILURE;
}




bool rledecomp(ifstream& is, ofstream& os) {
	bitreader br(is);
	uint8_t valid;
	uint32_t occ;
	while (true) {
		if (is.eof()) {
			break;
		}
		valid = br(1);
		switch (valid)
		{
		case 0: {
			occ = br(6);
			for (int i = 0; i < (occ + 1); i++) {
				uint8_t val = br(8);
				os.put(val);
			}
			break;
		}
		case 1: {
			occ = br(6);
			if (occ == 0) {
				while (true) {
					br(1);
					if (is.eof()) {
						break;
					}
				}
				break;
			}
			uint8_t val = br(8);
			for (int i = 0; i < (occ + 1); i++) {
				os.put(val);
			}
			break;
		}
	}
	}
	return 1;
}



bool rlecomp(ifstream& is,ofstream& os) {
	string stringa;
	uint8_t corrente;
	uint8_t successivo;
	uint8_t corr;
	uint32_t cont=0;
	corrente = is.get();
	successivo = is.get();
	bitwriter bw(os);
	while (true) {
		if (is.eof()) {
			break;
		}
		if (corrente == successivo) {
			cont = cont +2 ;
			corr = is.get();
			while (corr == successivo) {
				cont++;
				corr = is.get();
			}
			cont--;
			bw(1, 1);
			bw(cont, 6);
			bw(corrente, 8);
			cont = 0;
			corrente = corr;
			successivo = is.get();
			if (is.eof()) {
				bw(0, 1);
				bw(0, 6);
				bw(corrente, 8);
			}
		}
		else {
			stringa.push_back(corrente);
			stringa.push_back(successivo);
			cont++;
			corr = is.get();
			while (corr != successivo) {
				cont++;
				stringa.push_back(corr);
				successivo = corr;
				corr = is.get();
				if (is.eof()) {
					cont++;
					break;
				}
			}
			cont--;
			if (corr == successivo)
				stringa.pop_back();

			bw(0, 1);
			bw(cont, 6);
			for (uint8_t x : stringa) {
				bw(x, 8);
			}
			cont = 0;
			stringa.clear();
			corrente = successivo;
			successivo = corr;
		}
	}
	bw(1, 1);
	bw(0, 6);
	bw.~bitwriter();
	return 1;
}


int main(int argc, char **argv) {
	if (argc != 3) {
		syntax();
	}
	/*
	ifstream is(argv[1], ios::binary);
	if (!is) {
		cout << "Errore apertura file";
		EXIT_FAILURE;
	}
	ofstream of(argv[2], ios::binary);
	if (rlecomp(is, of)) {
		cout << "Compressione avvenuta con successo \n";
	}
	
	of.close();
	is.close();
	*/

	ifstream af(argv[1], ios::binary);
	if (!af) {
		cout << "Errore apertura file";
		EXIT_FAILURE;
	}
	ofstream cf(argv[2], ios::binary);
	rledecomp(af, cf);
	




}