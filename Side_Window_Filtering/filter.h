#include "used_headers.h"

using namespace cv;
using namespace std;

#ifndef _FILTER_H_
#define _FILTER_H_

struct filtering_information {
	int depth;
	Size kernal_size;
	Point anchor;
	int border_type;
};

#endif // _FILTER_H_


/*
class filtering_information {
	Size kernal_size;
	Point anchor;
	int border_type;

	public:
		filtering_information(Size kernal_size , Point anchor , int border_type) {
			this->kernal_size = kernal_size;
			this->anchor = anchor;
			this->border_type = border_type;
		}
};
*/