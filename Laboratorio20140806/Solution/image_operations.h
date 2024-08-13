#ifndef IMAGE_OPERATIONS_H
#define IMAGE_OPERATIONS_H

#include <algorithm>
using namespace std;

#include "image.h"

template<typename T>
void flip(image<T>& img) {
	for (unsigned y = 0; y < img.height()/2; ++y) {
		for (unsigned x = 0; x < img.width(); ++x) {
			swap(img(x, y), img(x, img.height() - 1 - y));
		}
	}
}

#endif // IMAGE_OPERATIONS_H