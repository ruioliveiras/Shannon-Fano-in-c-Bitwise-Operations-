/*
 * Developer : Rui Oliveira (rui96pedro@hotmail.com)
 * Date : 05/01/2014
 * You can use this for your own purpose
 */


#include <iostream>
#include <cstdlib>
#include "decoder.h"
#include <vector>

std::ifstream mDecodeFile;
std::ofstream mDecodeOutFile;


Decoder::Decoder(){ }

void Decoder::fileName(std::string fileName,std::string fileOutName){
	mDecodeFile.open(fileName.c_str(), std::ios::in|std::ios::binary);
		mDecodeOutFile.open(fileOutName.c_str(), std::ofstream::out | std::ofstream::binary);
}

void Decoder::startReading(){
	mdShan = ShanTable();
	//std::cout << "Start Read Frequency Table\n";
	readTable();
	//std::cout << "Start Creating the Shannon Fanno table\n";	
	mdShan.createShanTable();	
}

void Decoder::startReadingRLE(){
	startReading();
}

void Decoder::startWriting(){
	char *buffer = new char[BUFFER_SIZE];
	unsigned char *outBuffer = new unsigned char[BUFFER_SIZE];
	unsigned char cBuffer;
	int i,ibit,iSerch =0,howMany,iOut=0;
	unsigned int nBits= 0;
	bool res;


	while ((howMany = mDecodeFile.readsome(buffer,BUFFER_SIZE))){
	
		for (i=0;i<howMany;i++){
			cBuffer = buffer[i];	
	
			for(ibit=0;ibit < (int)(sizeof(char) * 8)  ;ibit++,iSerch++){
				res = mdShan.getCharByBitsGraph( (cBuffer >> ibit) & 1, iSerch, &nBits);
				//when res == true, nBits have the original Value
				if (res){
					if (nBits <= TABLE_MAX_DATA){
						outBuffer[iOut] =(unsigned char) nBits;
						iOut++;
					}else if (nBits == TABLE_SPECIAL_EOF){
						//goto used to skip all the cyles
						goto end_of_clycle;
					}
					nBits=0;	
					iSerch =-1; //becouse will do ++
					

					if (not (iOut < BUFFER_SIZE)){
						mDecodeOutFile.write ((char *)outBuffer, sizeof(unsigned char) *  BUFFER_SIZE);
						iOut = 0;
					}
					
				}
			}
		}
	}
	end_of_clycle:
	mDecodeOutFile.write ((char *)outBuffer, sizeof(unsigned char) * iOut);

	delete[] buffer;
	delete[] outBuffer;
}

void Decoder::startWritingRLE(){
	char *buffer = new char[BUFFER_SIZE];
	unsigned char *outBuffer = new unsigned char[BUFFER_SIZE];
	unsigned char cBuffer,length;
	int i,ibit,iSerch =0,howMany,iOut=0;
	unsigned int nBits =0;// mdShan.size() -1
	bool res,bLenght;

	bLenght = true; // first char are lenght
	while ((howMany = mDecodeFile.readsome(buffer,BUFFER_SIZE))){
	
		for (i=0;i<howMany;i++){	
			cBuffer = buffer[i];		

			for(ibit=0;ibit < (int)(sizeof(char) * 8)  ;ibit++,iSerch++){
				res = mdShan.getCharByBitsGraph( (cBuffer >> ibit) & 1, iSerch, &nBits);
				//when res == true, nBits have the original Value
				if (res){
					if (nBits <= TABLE_MAX_DATA){
						//bits are data:
						if (bLenght){
							length = (unsigned char) nBits;
							bLenght = false;
						}else{			
							//add lenght bits, (plus 1 becouse lenght 0 is 1 bit)		
							bLenght = true;
							for(int k=0;k<(length + 1);k++){
								outBuffer[iOut] =(unsigned char) nBits;
								iOut++;
								if (not (iOut < BUFFER_SIZE)){
									mDecodeOutFile.write ((char *)outBuffer, sizeof(unsigned char) *  BUFFER_SIZE);
									iOut = 0;
								}
							}
						}
					}else if (nBits == TABLE_SPECIAL_EOF){
						//goto used to skip all the cyles
						goto end_of_clycle;
					}
					nBits =0;	//nBits=mdShan.size() -1;	
					iSerch =-1; //becouse will do ++
					


				}
			}
		}
		
//return;
	}
	end_of_clycle:
	mDecodeOutFile.write ((char *)outBuffer, sizeof(unsigned char) * iOut);

	delete[] buffer;
	delete[] outBuffer;
}


void Decoder::readTable(){	
	int size,iFreq,iStack,howMany;
	Frequency frequency;	
	mDecodeFile.read((char*)(&size),sizeof(int));
	unsigned long* myBuffer = new unsigned long[BUFFER_SIZE];

	//std::cout << "123size): " << size << "\n";	
	iFreq=0;
	while (iFreq < size){
		//Read the Max possivel value
		howMany = (size - iFreq  < BUFFER_SIZE) ? size- iFreq : BUFFER_SIZE;
		howMany = mDecodeFile.readsome((char*) myBuffer,sizeof(unsigned long) * howMany);			
		iStack = 0;
		while (iStack <  (howMany / (int)sizeof(unsigned long))){						
			frequency.freq = myBuffer[iStack];
			frequency.index = iFreq;
			mdShan.setFrequency(iFreq,frequency);
			iFreq++;
			iStack++;
		}

	}

	 delete[] myBuffer;

}

void Decoder::printTable(){
	mdShan.printTable();
}

void Decoder::close(){
	mDecodeFile.close();
}



