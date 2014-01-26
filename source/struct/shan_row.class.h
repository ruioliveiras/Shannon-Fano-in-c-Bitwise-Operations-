/*
 * Developer : Rui Oliveira (rui96pedro@hotmail.com)
 * Date : 05/01/2014
 * You can use this for your own purpose
 */




#ifndef SHAN_ROW_H
#define SHAN_ROW_H

class ShanRow
{
	private:
		int mBitIndex;
		unsigned char* mBits;
		int mArraySize;
		int mSize;
		void allocMore();
	public:
		ShanRow();		
		/**
			Add one more bit, allocMore if it need.
			@argument char bit - should be 1 or 0 number, not char like '1'. */
		void addBit(char bit);
		unsigned char* bits();
		unsigned char block(int i);
		unsigned char getNBits(int nbits,int* nget,int from);
		void getNBitsClose();
		char debugBit(int i);
		char bit(int i);
		void printRow();
		int size();
		
};

#endif
