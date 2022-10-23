#include <iostream>
#include <sndfile.hh>
#include <vector>
#include <SFML/Main.hpp>
#define SHRTMAX 32767
#define USHRTMAX 65535
#define INTMAX 2147483647
#define BUFMAX 44100 * 60 * 10 * 2

#define FILELEN 111

inline float map(float s, float a1, float a2, float b1, float b2){
    return b1 + (s-a1)*(b2-b1)/(a2-a1);
}

struct chunk_t{
	unsigned long length;
	float* buffer;
	
	chunk_t(int llength){
		length = llength;
		buffer = (float*) malloc(sizeof(float) * length);	
	}
	~chunk_t(){
		free(buffer);
	}
	void fade_in(int num){
		if(num > length) num = length;
		float divisor = 1.0 / num;
		for(int i = 0;i<num;++i){
			buffer[i] *= divisor * ((float)i);		
	}}
	
	void fade_out(int num){
		if(length-num <= 0) num -= abs(length - num);
		float divisor = 1.0 / num;
		float counter = 1.0;
		for(int i=length;i>length-num;--i){
			counter -= divisor;
			buffer[i] *= float(counter);
	}}
};

static float* read_wav(const char* fname, std::vector<float>* iinto){
	static short buffer [BUFMAX];
	static float outbuf [BUFMAX];

	SndfileHandle file;

	file = SndfileHandle(fname);

	printf("i have opened: '%s' \n", fname);
	printf("samplerate: %d\n", file.samplerate() );
	printf("channels: %d\n", file.channels() );

	file.read(buffer, BUFMAX);

	puts("\n");

	float convert = 1.0 / (float) SHRTMAX;
	printf("converter - %f\n", convert);

	for(int i = 0; i < BUFMAX; ++i){
		outbuf[i] = map((float)buffer[i], -SHRTMAX, SHRTMAX, -1.0, 1.0);
	}

	for(int i=0;i<BUFMAX;++i){
		if(outbuf[i] != NULL) iinto->push_back(outbuf[i]);
	}
	
	return outbuf;
}

void write_wav(const char* fname, float* buffer, int length, int ssrate){
	float* out_buffer = (float*) malloc(sizeof(float) * length);

	for(int i = 0; i < length; ++i){
		out_buffer[i] = buffer[i]; 
	}

	//test...
	for(int i = 2000; i < 6100; ++i){
		printf("shrt - %d : float %f\n", out_buffer[i], buffer[i]);
	}

	SndfileHandle file;
	int channels = 2;
	int srate = ssrate;
	file = SndfileHandle(
		fname,
		SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_FLOAT,
		channels,
		srate);
	file.write(out_buffer, length);
	puts(" ");
}

