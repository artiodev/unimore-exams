#ifndef PGM_H
#define PGM_H

#include <string>

#include "image.h"
#include "basic_types.h"

bool LoadPGM_bin (const std::string& sFileName, image<uint8_t>& img);
bool SavePGM_bin (const std::string& sFileName, const image<uint8_t>& img);

#endif /* PGM_H */
