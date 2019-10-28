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

//	printString("Hello World! how are you today?\0");
	char* line[80];
	printString("Enter a line: ");
	readString(line);
	printString(line);
	while(1);

}

void printString(char* chars){
	int i = 0;
	while(chars[i]!=0x0){

	interrupt(0x10,0xe*256+chars[i],0,0,0);
	i++;
	}

}

void printChar(char c){
	interrupt(0x10,0xe*256+c,0,0,0);
}
void readString(char* line){

	int i =0;
	int exit =0;
	while(exit==0){

	char input = interrupt(0x16,0,0,0,0);

	if(input==0xd){
	printChar(0xd);
	line[i]=0xd;
	i++;
	line[i]=0xa;
	i++;
	printChar(0xa);
	line[i]=0x0;
//	printString("Enter has been pressed\0");
	exit=1;

	}else if(input==0x8){
	printChar(0x8);
	printChar(' ');
	printChar(0x8);
		if(i>0){

		i--;
		}
	}else{
	line[i]=input;
	printChar(input);
	i++;
	}
	}


}
