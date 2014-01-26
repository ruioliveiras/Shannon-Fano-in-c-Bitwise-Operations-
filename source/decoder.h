/*
 * Developer : Rui Oliveira (rui96pedro@hotmail.com)
 * Date : 05/01/2014
 * You can use this for your own purpose
 */



#ifndef DECODER_H
#define DECODER_H
#include <fstream>
#include <string.h>
#include "struct/shan_table.class.h"

class Decoder
{
	private:
		ShanTable mdShan;
		void readTable();
	public:
		Decoder();
		void fileName(std::string fileName,std::string fileOutName);
		void startReading();
		void startReadingRLE();
		void startWriting();
		void startWritingRLE();
		void close();
		void printTable();
		
};



#endif
