//Alex Stalter
//also Jack Nichols
//#include <stdio.h>

void printString(char*);
void printChar(char);
void readString(char*);
void readSector(char*,int);
void handleInterrupt21(int,int,int,int,int);
void readFile(char*,char*,int*);
void executeProgram(char*);
void terminate();
void main()
{
	//char buffer[13312];
	//int sectorsRead;
	makeInterrupt21();
	interrupt(0x21,5,"tstpr2",0,0);
	//interrupt(0x21,4,"tstpr1",0,0);
	//makeInterrupt21();
	//interrupt(0x21,3,"messag",buffer,&sectorsRead);
	//if(sectorsRead>0){

	//	interrupt(0x21,0,buffer,0,0);

	//}else{

	//interrupt(0x21,0,"messag not found\r\n",0,0);

	//}

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
	
	}else if(ax==4){

	executeProgram(bx);

	}else if(ax==5){
	
	terminate();

	}else{

	printString("ERROR no such function exists\n");

	}


}

void readFile(char* address,char* buffer,int* sectorsRead){
	int count=0;
	int i=0;
	int matches = 0;
	char dir[512];

	readSector(dir,2);
	for(count=0;count<512;count=count+32)
	{


	*sectorsRead=*sectorsRead+1;

		for(i=0;i<6;i++)
		{

		if(address[i]==dir[i+count]){

		matches++;
//		printChar(dir[i+count]);

		}else{

		matches==0;
		break;
		}

		}


	if(matches==6){
	readSector(buffer,*sectorsRead+4);
	break;

}

	}
	if(matches!=6){
	*sectorsRead=0;
	printString("/nError file not found.");
}

}
void executeProgram(char* name)
{
	int i;
	char buffer[13312];
	int segment = 0x2000;

	readFile(name, buffer);

	for(i = 0; i < 13312; i++)
	{
		putInMemory(segment, i, buffer[i]);
	}

	launchProgram(segment);


}

void terminate()
{
	while(1);
}


