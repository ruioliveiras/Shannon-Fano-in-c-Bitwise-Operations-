/*
 * Developer : Rui Oliveira (rui96pedro@hotmail.com)
 * Date : 05/01/2014
 * You can use this for your own purpose
 */


#include <iostream>
#include <cstdlib>
#include "shan_row.class.h"


ShanRow::ShanRow(){
	mArraySize = 1;
	mSize = 0;
	mBits = new unsigned char[mArraySize];
	mBitIndex= 0;
}
	

void ShanRow::allocMore(){
	unsigned char* aux = new unsigned char[mArraySize*2];
	int i;
	for (i=0;i<mArraySize;i++){
		aux[i] = mBits[i];
	}
	while(i< mArraySize*2){
		aux[i] = 0;
		i++;
	}
	delete[] mBits;
	mBits = aux;
	mArraySize *=2;	
}
void ShanRow::addBit(char bit){
	int writeBlock = mSize/ (sizeof(char) * 8) ;
	int indexBlock = mSize % (sizeof(char) * 8);
	unsigned char print;	
	if (writeBlock >= mArraySize)
		allocMore();
//std::cout << "bit ("<< (int)bit <<") blockIndex " << writeBlock << " bitIndex "<< indexBlock << " size " << mSize;
		
	print = mBits[writeBlock];
//	std::cout << " before: " << std::hex << (unsigned int) print << " + " << (unsigned int)(bit << indexBlock ) <<"";
	print = print | (bit << indexBlock );
//	std::cout << " after: " << std::hex << (unsigned int) print << "\n";
	mBits[writeBlock] = print;

	mSize++;
}

unsigned char* ShanRow::bits(){
	return mBits;
}

unsigned char ShanRow::block(int i){
	return mBits[i];
}

unsigned char ShanRow::getNBits(int nbits,int* nget,int shift){	
	if (mBitIndex >= mSize){
		*nget = 0;
		return 0;
	}
	
	if (nbits + mBitIndex > mSize)
		return getNBits(mSize - mBitIndex,nget,shift);

	//down of this can get ngetBits
	unsigned char ret = 0;
	int blockIndex = mBitIndex/(sizeof(char) * 8);
	int bitIndex = mBitIndex % (sizeof(char) * 8);

	ret = mBits[blockIndex];
	ret = ret >> bitIndex; // simulate like bitIndex is on start index.	
	if (blockIndex +1 < mArraySize)  
		ret = ret  | (mBits[blockIndex + 1] << (8 - bitIndex));
	
	mBitIndex += nbits;
	//ret = ret << (8 - nbits);	

	ret = ret << shift;
	*nget = nbits;
	return ret;
}
void ShanRow::getNBitsClose(){
	mBitIndex = 0;
}

int max(int a,int b){
	return (a>b) ? a : b;
}

char ShanRow::debugBit(int index){
	int writeBlock = index/(sizeof(char) * 8);
	int indexBlock = index % (sizeof(char) * 8);
	char print;	
	if (index>mSize)
		return 'e';	//error
	print = mBits[writeBlock];
	print = (print >> indexBlock) & 1;
	if (print)
		return '1';
	else
		return '0';
}

char ShanRow::bit(int index){
	int writeBlock = index/(sizeof(char) * 8);
	int indexBlock = index % (sizeof(char) * 8);
	char print;	
	if (index >= mSize)
		return 'e';	//error
	print = mBits[writeBlock];
	print = (print >> indexBlock) & 1;
	return print;
}

void ShanRow::printRow(){
	for (int i=0;i<mSize/8;i++){
		std::cout << std::hex <<(unsigned int)mBits[i] << std::dec;
	}	
	std::cout << " \t";
	for (int i=0;i<mSize;i++){
		if (i%8 == 0) std::cout << ",";
		std::cout << debugBit(i);
	}
}

int ShanRow::size(){
	return mSize;
}


