#include "used_headers.h"

using namespace cv;
using namespace std;

struct filtering_information {
	int depth;
	Size kernal_size;
	Point anchor;
	int border_type;
};