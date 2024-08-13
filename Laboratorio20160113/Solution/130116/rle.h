#if !defined RLE_h
#define RLE_H


#include <fstream>

bool rlecomp(std::istream& is, std::ostream& os);
bool rledecomp(std::istream& is, std::ostream& os);


#endif