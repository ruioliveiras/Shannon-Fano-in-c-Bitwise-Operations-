/*
 * Developer : Rui Oliveira (rui96pedro@hotmail.com)
 * Date : 05/01/2014
 * You can use this for your own purpose
 */


#include <iostream>
#include <cstdlib>
#include "shan_table.class.h"




ShanTable::ShanTable(){
	mShanTable = new ShanRow[TABLE_SIZE];
	mShanGraph = new ShanGraph[2*TABLE_SIZE];
	mFreq = new Frequency[TABLE_SIZE];
	mTotalFreq = 0;

	int size = TABLE_SIZE;
	for (int i=0;i<size;i++){
		mFreq[i].index = i;
		mFreq[i].freq = 1;
		mTotalFreq += mFreq[i].freq; 
		mShanTable[i] = ShanRow();
	}
}


/*################ Set/get Properties ################*/
Frequency ShanTable::getFrequency(int i){
	return mFreq[i];
}
void ShanTable::setFrequency(int index,Frequency value){
	mTotalFreq -= mFreq[index].freq;
	mFreq[index] = value;
	mTotalFreq +=value.freq;
}

void ShanTable::addFrequency(int index){
	(mFreq[index].freq)++;
	mTotalFreq++;
	if (mTotalFreq==0) std::cout << "shit\n";
}

int ShanTable::size(){
	return TABLE_SIZE;
}

unsigned long ShanTable::tFreq(){
	return mTotalFreq;
}

ShanRow ShanTable::getRow(unsigned int index){
	//return mShanTable[mFreq[index].index];
	return mShanTable[index];
}



/*################ create Shannon Fanno Table ################*/


void ShanTable::createShanTable(){
	sort();
	int free=1;
	shanTableAux(0,size(),mTotalFreq,0,&free);
	//std::cout<< "Table created, size: " << size() << ", total of frequency: " << mTotalFreq <<" \n";
}
void ShanTable::shanTableAux(int startIndex,int stopIndex,unsigned long freqSize,int grapthWrite,int* graphFreeIndex){
	unsigned long halfSum;	
	int i,midleIndex;

	//stop case
	if( not (stopIndex-startIndex>1)){		
		mShanGraph[grapthWrite].one = mShanGraph[grapthWrite].zero = -1;
		mShanGraph[grapthWrite].value = mFreq[startIndex].index;
		return;
	}	

	// add 1
	halfSum = 0;
	i = startIndex;		
	while (halfSum < (freqSize/2) && (i < (stopIndex - 1))){
		halfSum += mFreq[i].freq;
		mShanTable[mFreq[i].index].addBit(1);
		i++;
	}
	// add 0
	midleIndex = i;
	while(i<stopIndex){
		mShanTable[mFreq[i].index].addBit(0);
		i++;
	}
	
	mShanGraph[grapthWrite].one = (*graphFreeIndex)++;
	mShanGraph[grapthWrite].zero = (*graphFreeIndex)++;
	mShanGraph[grapthWrite].value = -1;

	//call:
	shanTableAux(startIndex,midleIndex,halfSum,mShanGraph[grapthWrite].one,graphFreeIndex);
	shanTableAux(midleIndex,stopIndex,freqSize-halfSum,mShanGraph[grapthWrite].zero,graphFreeIndex);
}


/**
	Function used in ShanTable::sort(), to use qsort
*/
int compare_ints(const void* a, const void* b) 
{
    unsigned long arg1 = (*reinterpret_cast<const Frequency*>(a)).freq;
    unsigned long arg2 = (*reinterpret_cast<const Frequency*>(b)).freq;
    if(arg1 < arg2) return -1;
    if(arg1 > arg2) return 1;
    return 0;
}

void ShanTable::sort(){
	 std::qsort(mFreq, size(), sizeof(Frequency), compare_ints);
}

/*################ Other ################*/

bool ShanTable::getCharByBitsGraph(char a,int bitIndex,unsigned int *index){
	if (bitIndex == 0)
		*index =0;	
	
	if (a == 1){
		*index = mShanGraph[*index].one;
	}else{
		*index = mShanGraph[*index].zero;
	}


	if (mShanGraph[*index].value != -1){
	//	std::cout << "value: " << std::hex << 	(unsigned int) mShanGraph[*index].value << "\n";
		*index = (unsigned int) mShanGraph[*index].value;
		return true;
	}

	
	return false;
}


bool ShanTable::getCharByBitsTable(char a,int bitIndex,unsigned int *index){
	int i;
	char bit;
	if (bitIndex == 0)
		*index = size() -1;

	i = *index;
	while(1){
		bit = mShanTable[mFreq[i].index].bit(bitIndex);
		if (bit == a){
			*index = i;
			if ((mShanTable[mFreq[i].index].size() -1	)  == bitIndex){
				*index = mFreq[i].index;
				return true;
			}else{				
				return false;
			}
		}
		
		i--;
	}
}


	
void ShanTable::printTable(){
	int sum=0;	
	for (int i=0;i<size();i++){
		mShanTable[mFreq[i].index].printRow();	
		sum+=mFreq[i].freq;	
		std::cout  << " \t freq " << mFreq[i].freq << " sum "<<  sum << "| original: " << std::hex << mFreq[i].index << std::dec << " {" << i << "/" <<size() << "}\n" ;
	}
	std::cout << "end\n\n\n";
	printGraph(0,0);
}

void ShanTable::printGraph(int index,int layer){
	for (int i = 0; i<layer;i++)
		std::cout << " ";
	std::cout << index << "(0)" << mShanGraph[index].zero << ",1)"<< mShanGraph[index].one << ","<<  mShanGraph[index].value << ")\n";

	if (mShanGraph[index].zero != -1){
		printGraph(mShanGraph[index].zero,layer+1);
		printGraph(mShanGraph[index].one,layer+1);
	}
}











