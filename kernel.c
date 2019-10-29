//Alex Stalter

#include <stdio.h>

void printString(char*);
void printChar(char);
void readString(char*);
void readSector(char*,int);
void handleInterrupt21(int,int,int,int,int);
void readFile(int,char*,int);
void main()
{
	char* buffer[512];
	printString("Testing printString \n\r\0 if you see this it has not worked");
	makeInterrupt21();
	interrupt(0x21,1,buffer,0,0);
	interrupt(0x21,0,buffer,0,0);
	interrupt(0x21,2,buffer,5,0);
	interrupt(0x21,0,buffer,0,0);
	interrupt(0x21,5,0,0,0);

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
	printString("Enter a line: \0");
	while(exit==0){

	char input = interrupt(0x16,0,0,0,0);
	if(input==0xd){
	printChar(0xd);
	printChar(0xa);
	line[i]=0xd;
	i++;
	line[i]=0xa;
	i++;
	line[i]=0x0;

	exit=1;

	}else if(input==0x8 && i!=0){
	printChar(0x8);
	printChar(' ');
	printChar(0x8);
	i--;
	}else if(input!=0x8){
	line[i]=input;
	printChar(input);
	i++;
	}
	}


}
void readSector(char* buffer,int sector){


	int ax = 2*256+1;
	int cx = 0*256+(sector+1);
	int dx = 0*256+0x80;
	interrupt(0x13,ax,buffer,cx,dx);


}
void handleInterrupt21(int ax,int bx,int cx,int dx){

	if(ax==0){
	printString(bx);
	}else if(ax==1){

	readString(bx);

	}else if(ax==2){

	readSector(bx,cx);

	}else if(ax==3){

	readFile(bx,cx,dx);

	}else{

	printString("ERROR no such function exists\n");

	}


}

void readFile(int address,char* buffer,int sectorsRead){



}
