/*
 * Developer : Rui Oliveira (rui96pedro@hotmail.com)
 * Date : 05/01/2014
 * You can use this for your own purpose
 */


#include <iostream>
#include <cstdlib>
#include "encoder.h"
#include <iomanip>


std::ifstream mFile;
std::ofstream mOutFile;





Encoder::Encoder(){ }

void Encoder::fileName(std::string fileName,std::string outfile){
	mFile.open(fileName.c_str(), std::ios::in|std::ios::binary);
	mOutFile.open(outfile.c_str(), std::ofstream::out| std::ofstream::binary);
}

void Encoder::startSample(){
	char *buffer = new char[BUFFER_SIZE];	
	int howMany,i;	
	mShan = ShanTable();
	
	while ((howMany = mFile.readsome(buffer,BUFFER_SIZE))){
		for (i=0;i<howMany;i++){
			mShan.addFrequency((unsigned char)buffer[i]);
		}
	}
	
	//pre-write file:
	writeTable();

	std::cout << "Start Creating the Shannon Fanno table\n";		
	mShan.createShanTable();

	delete[] buffer;
}

void Encoder::startSampleRLE(){
	char *buffer = new char[BUFFER_SIZE];	
	unsigned char actual;
	unsigned char last;
	unsigned char count;
	int howMany,i;	
	mShan = ShanTable();
	
	// counting 0 is count 1, otherwise zero was empty on file
	while ((howMany = mFile.readsome(buffer,BUFFER_SIZE))){
		last = buffer[0];
		count = 0;
		for (i=1;i<howMany;i++){
			actual = (unsigned char)buffer[i];
			// 0xFF to guard the max value of count
			if ((last == actual) && (count != 0xFF)){
				count++;
			}else{
				mShan.addFrequency(count);
				mShan.addFrequency(last);
				last=(unsigned char) buffer[i];
				count = 0;
			}
		}
	}
	
	//pre-write file:
	writeTable();

	// 2º cierto: passa, mas freq correctas? ...
	mShan.createShanTable();
	delete[] buffer;
}



void Encoder::startWriting(){
	char *buffer = new char[BUFFER_SIZE];
	char *outBuffer = new char[BUFFER_SIZE];
	int blockIndex = 0;
	int bitIndex = 0;
	int howMany,i;	

	
	mFile.clear();
	mFile.seekg(0, std::ios::beg);
	

	for(i=0;i<BUFFER_SIZE;i++)outBuffer[i] = 0;

	while ((howMany = mFile.readsome(buffer,BUFFER_SIZE))){
		
		for (i=0;i<howMany;i++){
			writeOnBuffer((unsigned char) buffer[i],outBuffer,&blockIndex,&bitIndex);
		}
	}
	//mark EOF
	writeOnBuffer(TABLE_SPECIAL_EOF,outBuffer,&blockIndex,&bitIndex);	
	//not (blockIndex +1) becouse this index is to write, ie current value of blockIndex is the Size
	mOutFile.write (outBuffer, blockIndex * sizeof(char));
	

	
	delete[] buffer;
	delete[] outBuffer;
}

void Encoder::writeOnBuffer(unsigned int a,char *outBuffer,int* _blockIndex, int* _bitIndex){
	int blockIndex = *_blockIndex;
	int bitIndex = *_bitIndex;
	int get=0;	
	ShanRow row = mShan.getRow(a);
	
	do{
		outBuffer[blockIndex] |= row.getNBits(8-bitIndex,&get,bitIndex);
		bitIndex += get;		
		if (bitIndex >= 8){
			blockIndex++;
			//check if out of bound			
			if (blockIndex == BUFFER_SIZE){
				mOutFile.write (outBuffer, BUFFER_SIZE * sizeof(char));
				blockIndex = 0;
			}
			outBuffer[blockIndex] = 0;
			bitIndex = 0;
		}


		
	}while(get>0);
	
	row.getNBitsClose();
	*_blockIndex = blockIndex;
	*_bitIndex = bitIndex;

}



void Encoder::startWritingRLE(){
	char *buffer = new char[BUFFER_SIZE];
	char *outBuffer = new char[BUFFER_SIZE];
	int blockIndex = 0;
	int bitIndex = 0;
	int howMany,i;
	unsigned char actual,last,count;
	int first;
	
	mFile.clear();
	mFile.seekg(0, std::ios::beg);
	
	for(i=0;i<BUFFER_SIZE;i++)outBuffer[i] = 0;
	

	
	howMany = mFile.readsome(buffer,BUFFER_SIZE);	
	last = (unsigned char)buffer[0];
	count = 0;
	first = 1;
	// counting 0 is count 1, otherwise zero was empty on file
	do{
		//start at first (1), becouse will compare with the last (2º with the 1º).
		for (i=first;i<howMany;i++){
			first = 0;			
			actual = (unsigned char)buffer[i];
			// 0xFF to guard the max value of count
			if ((last==actual) && (count != 0xFF)){
				count++;
			}else{
 				writeOnBuffer(count,outBuffer,&blockIndex,&bitIndex);
				writeOnBuffer(last,outBuffer,&blockIndex,&bitIndex);
				last=actual;
				count = 0;
			}
		}
	
	}while ((howMany = mFile.readsome(buffer,BUFFER_SIZE)));

	writeOnBuffer(count,outBuffer,&blockIndex,&bitIndex);
	writeOnBuffer(last,outBuffer,&blockIndex,&bitIndex);
		//mark EOF
	writeOnBuffer(TABLE_SPECIAL_EOF,outBuffer,&blockIndex,&bitIndex);	
	//not (blockIndex +1) becouse this index is to write, ie current value of blockIndex is the Size
	mOutFile.write (outBuffer, blockIndex * sizeof(char));	


	delete[] buffer;
	delete[] outBuffer;	
}


void Encoder::writeTable(){
	int iFreq;
	int shanSize = mShan.size();

	//Write Frequency table Size	
	mOutFile.write ((char *)(&shanSize), sizeof(int));
	unsigned long* myBuffer = new unsigned long[shanSize];

	//add Frequency to the buffer
	for (iFreq=0;iFreq < shanSize;iFreq++){
		myBuffer[iFreq] = mShan.getFrequency(iFreq).freq;
	}

	mOutFile.write((char*)myBuffer,sizeof(unsigned long) * shanSize);


	 delete[] myBuffer;		
}


void Encoder::printTable(){
	mShan.printTable();
}


void Encoder::close(){
	mFile.close();
	mOutFile.close();
}





