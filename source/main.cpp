/*
 * Developer : Rui Oliveira (rui96pedro@hotmail.com)
 * Date : 05/01/2014
 * You can use this for your own purpose
 */

#include <iostream>
#include <ctime>
#include "main.h"
#include "encoder.h"
#include "decoder.h"
#include <iomanip>


int main(int argc, char* argv[])
{
	char function;
	std::string input;
	std::string output;
	//Input:
	std::cout << "Mode [c/d] (compress/deconpress):\n";
	std::cin >> function;
	std::cout << "intput path:\n";
	std::cin >> input;
	std::cout << "output path:\n";
	std::cin >> output;
	
		

	//what to do?
	if (function == 'c'){
        encode(input,output);
	}else if (function == 'd'){
		decode(input,output);
	}else{
		std::cout <<"ERROR: use mode 'c' to compress or 'd' to decompress\n";
	}


//	test();

	return 0;

}

void test(){
	//encode("testes2/201061200010_1.PDF","3.jpg");
	//decode("3.jpg","4.xml");
}

void encode(std::string input, std::string output){
	Encoder ec = Encoder();
	ec.fileName(input,output);
//	std::cout << "Start Sampling the file\n";	
	ec.startSample();
//	std::cout << "Start Writing final file\n";
	ec.startWriting();
//	std::cout << "End write, lets close\n";
	ec.close();
}



void decode(std::string input, std::string output){
	Decoder ec = Decoder();
	ec.fileName(input,output);
//	std::cout << "Start reading the file\n";	
	ec.startReading();
//	std::cout << "Start Writing final2 file\n";
	//ec.printTable();
	ec.startWriting();
//	std::cout << "End write, lets close\n";
	ec.close();
}

void encodeRLE(std::string input, std::string output){
	Encoder ec = Encoder();
	ec.fileName(input,output);
//	std::cout << "Start Sampling the file\n";	
	ec.startSampleRLE();
//	std::cout << "Start Writing final file\n";
	ec.startWritingRLE();
//	std::cout << "End write, lets close\n";
	ec.close();
}



void decodeRLE(std::string input, std::string output){
	Decoder ec = Decoder();
	ec.fileName(input,output);
//	std::cout << "Start reading the file\n";	
	ec.startReadingRLE();
//	std::cout << "Start Writing final2 file\n";
	//ec.printTable();
	ec.startWritingRLE();
//	std::cout << "End write, lets close\n";
	ec.close();
}
