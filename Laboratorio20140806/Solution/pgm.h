#ifndef PGM_H
#define PGM_H

#include <string>

#include "image.h"

typedef unsigned char byte;

bool writeP2(const std::string& sFileName, const image<byte>& img);
bool writeP5(const std::string& sFileName, const image<byte>& img);

bool readP2(const std::string& sFileName, image<byte>& img);
bool readP5(const std::string& sFileName, image<byte>& img);

#endif // PGM_H