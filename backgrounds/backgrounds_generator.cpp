#include <iostream>
#include <fstream>

#define length 205
#define height 55

bool is_mountain(int x, int y){
	return y >= 5 and y <= 15 and x > 10 and x < 50;
}

bool is_tree(int x, int y){
	return x%5 == 0 and y%5 == 3 and x > 40;
}

bool is_water(int x, int y){
	return y > 50 or (x > 160 and y < 20);
}

int main(int argc, char* argv[]){
	if(argc != 2){
		throw std::invalid_argument("Need one parameter for output name");
	}
	std::ofstream output;
	output.open(argv[1]);
	for(unsigned y = 0; y != height; ++y){
		for(unsigned x = 0; x != length; ++x){
			if(is_water(x, y)) {
				output << 'w';
			}else if(is_tree(x, y)){
				output << 'Y';
			}else if(is_mountain(x, y)){
				output << 'M';
			}else{
				output << 'g';
			}
		}
		output << '\n';
	}
	output.close();
	return 0;
}
