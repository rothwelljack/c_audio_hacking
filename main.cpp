#include <iostream>
#include <sndfile.hh>
#include <vector>
#include <SFML/Main.hpp>
#include	<cstdio>
#include	<cstring>
#include "file_rw.hpp"
#include "lsf_wav.hpp"
#define SHRTMAX 32767
#define USHRTMAX 65535
#define INTMAX 2147483647
#define BUFMAX 44100 * 60 * 10 * 2

#define FILELEN 111

std::vector<float> sample;


int main(){
	std::cout << "hitleer \n";
	float* list = read_wav("portraits.wav", &sample);

	chunk_t chunk = chunk_t(44100);

	for(int i=0;i<44100;++i){
		chunk.buffer[i] = sample[i];
	}
	
	chunk.fade_in(2000);
	chunk.fade_out(4000);
	

	write_wav("lerbens.wav", chunk.buffer, chunk.length, 48000);

	printf("bufmax - %d\nlength - %d\n", BUFMAX, sample.size());
	return 0;
}
