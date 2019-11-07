//Alex Stalter

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
	
	makeInterrupt21();
	interrupt(0x21, 4, "shell", 0, 0); 


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

void readString(char* string){

	//local variables
	int i = 0;
	int pos = 0;
	int stringLength = 80;
	char currentChar = interrupt(0x16, pos, 0, 0, 0);
	char backPressed = 0x8;
	char enterPressed = 0xd;

	//Start string - 2 and make sure enter isnt pressed
	while(i < string-2 && currentChar != enterPressed){

		//if backspace isnt pressed
		if(currentChar != backPressed){

			printChar(currentChar);
			string[i] = currentChar;
			i++;
		}else{
			//If backspace is pressed
			i--;

			//if the string length is greater than 0 at interrupt
			if(i >= 0){

				//Print out the char
				printChar(currentChar);

				//Change the current position value on interrupt
				interrupt(0x10, 0xe*256+'\0', 0, 0, 0);

				//Call the bacspace interrupt
				interrupt(0x10, 0xe*256+backPressed, 0, 0, 0);

			}else{
				//if the string is not then set iteration back to start
				i = 0;
			}
		}
		//Now if enter is pressed position to interrupt 
		//at current and allow no more entry
		currentChar = interrupt(0x16,0,0, 0, 0);
	}

	//Add new line feed with carriage return and new line
	printString("\r\n");
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
	char shell[6];
	shell[0] = 's';
	shell[1] = 'h';
	shell[2] = 'e';
	shell[3] = 'l';
	shell[4] = 'l';
	shell[5] = '\0';
	executeProgram(shell);
}


