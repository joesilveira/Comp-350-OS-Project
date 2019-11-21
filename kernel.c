//Alex Stalter
//also Jack Nichols
//also joe silveira
//#include <stdio.h>


//String stuff
void printString(char*);
void printChar(char);
void readString(char*);
void printInteger(int*);

//File stuff
void readSector(char*,int);
void readFile(char*,char*,int*);
void writeSector(char*, int);
void listDir();

//Program stuff
void handleInterrupt21(int,int,int,int,int);
void executeProgram(char*);
void terminate();


void main()
{
	
	makeInterrupt21();
	interrupt(0x21, 4, "shell", 0, 0); 


	while(1);

}



/*
	************************String Stuff**************************

*/
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

void printInteger(int i){
	int* integer[2];
	integer[0] = i;
	integer[1] = '\0';
	printString(integer);
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



/*
	**************************File Stuff*****************

*/

void readSector(char* buffer,int sector){


	int ax = 2*256+1;
	int cx = 0*256+(sector+1);
	int dx = 0*256+0x80;
	interrupt(0x13,ax,buffer,cx,dx);


}


void writeSector(char* buffer, int sector){
	int ax = 3*256+1;
	int cx = 0*256+(sector + 1);
	int dx = 0*256+0x80;
	interrupt(0x13,ax,buffer,cx,dx);
}

/*
*Renewed by Joe Silviera
*
*
//Per Professor Black
*
*Parameter 'sectorsRead' may or may not be used
*
*Need to add for loop to loop through each sector not just 1 sector
*
//Original req's (comments for own use)
*
*1. Takes fileName and reads that in to buffer (A.K.A parameters filename and buffer)
*
*2. Calls interrupt 21 under the following parameters:
*
*	interrupt 21 (ax,bx,cx) formally interrupt 21 (ax,bx,cx,dx)
*
*	ax = 3 - interrupt call
*	bx = file name or address or char array
*	cx = address of buffer to hold file
*
*	//dx = #number of sector to read//
*
*	The use of dx is nullified because we now have a for loop that will
*	loop through each sector until the sector reads '\0' rather than
*	giving the function and exact number of sectors to read it will determine that on its own
*
*
//Basic operation
*
*1. Load file directory in to 512 char array using readSector (Variables: fileDirectory and buffer)
*
*2. Loop through the directory 
*
*	2.1 Try to match the file name
*
*	2.2 If the file name is not found set sectors read to 0 and return
*
*		**We will not be using sectors read therefore we will loop**
*
*3. Using the sector numbers in the directory:
*
*	a: Load the file sector by sector in to the buffer array
*
*	b. add 512 to buffer address everytime readSector is called
*
*	c. Increment # of sector we read everytime we loop
*
*		**Again we will not need to increment as we will loop until '\0'**
*
*/
void readFile(char* fileName , char* fileBuffer) {

//Variables
	char fileDirectory[512];
	char buffer[512];

	//for loop variables 

	int i = 0;	//Directory iteration
	int k = 0;	//File name iteration
	int j = 0;	//sector iteration
	int l = 0;	//Inner sector iteration (# sectors)

	int matches = 0;	

	/*
		*Function begin
	*/

	//1. Load file in to 512 char array
	readSector(buffer,2);

	//2. Loop through the directory 
	for(i = 0; i < 512; i++){

		//2.1 try to match the file name
		if(buffer[i] != fileName[matches]){

			//2.2 if it doesnt match go to the next sector
			i += 31;

		}else if (matches == 5){
			//printString("Matches \n");
			

			//Loop through the remaining 26 bytes (sectors)
			for(j = 0; j < 26; j++){

				//If the next sector is empty (== '\0') set the filebuffer to \0 and break
				if(buffer[j + i + 1] == '\0'){

					fileBuffer[j * 512] = '\0';

					//printString("setting to 0\n");
					break;

				//If not read the sector 
				}else{

				//printString("Reading Sectors\n");

				//Read the sectors and set them equal to the given sector
					readSector(fileDirectory,buffer[j + i + 1]);

				//3C: This is where we eliminate sectors read and have the for loop perform 
					for(l = 0; l < 512; l++){

					//Set the passed sector equal to the directory sector
						fileBuffer[(j * 512) + l] = fileDirectory[l];
					}
				}
			}
			break;

		//buffer[i] does equal fileBuffer[macthes]	so ++;
		}else{
			matches++;
		}

	}
	return;
}






void listDir(){



	//local vars
	char directoryBuffer[512];
	char buffer[512];
	int i;
	int j;
	int k = 0;
	int l = 0;
	int size = 0;


	//Set both buffers at i to 0 to start
	for(i = 0; i < 512; i++){
		directoryBuffer[i] = 0x0;
		buffer[i] = 0x0;
	}

	//Read the given sector of the directory
	readSector(directoryBuffer,2);

	//loop through the sector 
	for(i = 0; i < 16; i++){

		//if file exists with in the given sector
		if(directoryBuffer[32 * i] != '\0'){

			//Read the file name 
			for(j = 0; j < 6; j++){

				buffer[k] = directoryBuffer[i * 32 + j];
				k++;		
				

			}

			//Add a new line and return for every file 
			//print nicely on its own line
			buffer[k] = '\r';
			k++;
			buffer[k] = '\n';
			k++;
			
		}
	}
	printString("\n\r");
	for(i = 0; i < 512; i++){

		if(buffer[i] != 0x0){
			printChar(buffer[i]);
		}	
	}

}




/*
	***********************Program Stuff********************

*/

void handleInterrupt21(int ax,int bx,int cx,int dx){

	if(ax==0){
		printString(bx);
	}else if(ax==1){

		readString(bx);

	}else if(ax==2){

		readSector(bx,cx);

	}else if(ax==3){

		readFile(bx,cx);

	}else if(ax==4){

		executeProgram(bx);

	}else if(ax==5){

		terminate();

	}else if(ax==6){

		listDir(bx);

	}

	else{

		printString("ERROR no such function exists\n");

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



