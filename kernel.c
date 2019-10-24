#include <stdio.h>

void printString(char*);
void printChar(char);
void readString(char*);


void main()
{
//	int startVidMem = 0xb800;
//	int vidMemOffset = 0x0;
//	int white = 0x7;
//	char* letters = "Hello World!\0";
//	while(*letters !=0x0){
//		putInMemory(startVidMem,vidMemOffset,*letters);
//		vidMemOffset++;
//		putInMemory(startVidMem,vidMemOffset,white);
//		vidMemOffset++;
//		letters++;
//	}

	printString("Hello World! how are you today?\0");
	while(1);

}

void printString(char* chars){
	int i = 0;
	while(chars[i]!=0x0){

	interrupt(0x10,0xe*256+chars[i],0,0,0);
	i++;
	}

}
