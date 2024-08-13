#include "basic_types.h"
#include "pgm.h"

int main (int argc, char *argv[]) {
	unsigned w;
	unsigned h;
	char *output_filename;

	// Parsing della command line

	// Lettura del TIFF

	// Creazione dell'immagine
	image<uint8_t> img(w,h);

	// Inserimento dei dati nell'immagine

	// Salvataggio in PGM
	SavePGM_bin (output_filename,img);
}