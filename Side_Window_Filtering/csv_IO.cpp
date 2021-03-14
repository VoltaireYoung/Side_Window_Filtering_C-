#include "used_headers.h"

using namespace cv;
using namespace std;

void write_csv_with_float_Mat(string save_path , string filename , Size data_size , Mat data) {
	int current_row, current_column, current_channel;

	ofstream B;
	ofstream G;
	ofstream R;

	string temp_path;

	temp_path = save_path + "\\" + "B" + "\\" + filename + ".csv";
	B.open(temp_path.c_str());
	temp_path = save_path + "\\" + "G" + "\\" + filename + ".csv";
	G.open(temp_path.c_str());
	temp_path = save_path + "\\" + "R" + "\\" + filename + ".csv";
	R.open(temp_path.c_str());

	Vec3f* current_row_pointer;
	int index;

	for (current_row = 0; current_row < data_size.height; current_row++) {
		current_row_pointer = data.ptr<Vec3f>(current_row);

		for (current_column = 0, index = 0; current_column < data_size.width - 1; current_column++) {
			B << current_row_pointer[current_column][0] << ",";
			G << current_row_pointer[current_column][1] << ",";
			R << current_row_pointer[current_column][2] << ",";
		}

		B << current_row_pointer[current_column][0] << "\n";
		G << current_row_pointer[current_column][1] << "\n";
		R << current_row_pointer[current_column][2] << "\n";


		//B << current_row_pointer << "\n";
		//G << current_row_pointer << "\n";
		//R << current_row_pointer << "\n";
	}
	
	B.close();
	G.close();
	R.close();
}