//Joe silveira

void printString(char*);
void printChar(char*);
void readString(char*);
void readSector(char*,int);
void handleInterrupt21(int ax,int bx,int cx,int dx);
void readFile(char* fileName, char* buffer);
void executeProgram(char*);
int stringCompare(char*,char*);

//Main Function
void main(){

	char buffer[13312];   /*this is the maximum size of a file*/
	int sectorsRead;
	makeInterrupt21(); 
	interrupt(0x21, 3, "messag", buffer, &sectorsRead);   /*read the file into buffer*/ 
	if (sectorsRead>0)
		interrupt(0x21, 0, buffer, 0, 0);   /*print out the file*/ 
	else
		interrupt(0x21, 0, "messag not found\r\n", 0, 0);  /*no sectors read? then print an error*/


	while(1);
}

//Print string function
void printString(char* chars){
    int i = 0;
    while(chars[i] != '\0'){
            int ch = chars[i];
		    interrupt(0x10, 0xe*256+ch, 0, 0, 0);
            i++;
    }
}

//Print char function
void printChar(char c){
	char* chars[2];
	chars[0] = c;
	//Must add \0 so that printstring knows end of "string"
	chars[1] = '\0';
	printString(chars);
}

//Read String function
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

//Read sector function
void readSector(char* buffer, int sector){

	int ah = 2; //BIOS CALL
	int al = 1; //number of sectors
	int ch = 0; //track number
	int cl = sector + 1; // relative sector number
	int dh = 0; //head number
	int dl = 0x80; //device number

    //Combine them all
	int ax = (ah*256) + al;
	int cx = (ch*256) + cl;
	int dx = (dh*256) + dl;


	//Call interrupt
	interrupt(0x13,ax,buffer,cx,dx);

}


void handleInterrupt21(int ax, int bx, int cx, int dx){
		//printString("hello world");

		//AX determines what function to run

		//BUNCH OF IFS!

		//Print String
		//ax = 0; bx = address of string to print
		if(ax == 0){
			printString(bx);
		}

		//Read String
		//ax = 1; bx = address of character array of keys
		if(ax == 1){
			readString(bx);
		}

		//Read sector
		//ax = 2; bx = character array; cx = sector num
		if(ax == 2){
			readSector(bx,cx);
		}

		//Error print
		//ax >= 3
		if(ax >= 3){
			readFile(bx,cx,dx);
		}
}


//function to read file
void readFile(char* fileName,char* buffer){ 
    
    int fileLocation;
    int bufferPos;
    int filePresent;
    int sectors[];
    int i;
    int j;

   	//Read the current directory
    readSector(buffer, 2);  

  
	filePresent = stringCompare(buffer,fileName);

	//If filePresent is greater than 0 then the file is found
	if (filePresent > 0){


		fileLocation = filePresent * 32 + 6;


		for (i = 0;i < 26; i++){
			sectors[i] = buffer[fileLocation + i];
			
		}


		//While loop to loop through the file and call readSector over and over until it is 0
		j = 0;
		bufferPos = 0;

		while(sectors[j] !=0x0 ){

			readSector(buffer + bufferPos,sectors[j]);

			//Add 512 to buffer position
			bufferPos += 512;
			j++;
		}
		
	}
	
}

int stringCompare(char* buffer, char* fileName){ 

	int i = 0;
	int j = 0;

	int fileFound = 0;

	//For the 16 bits
     for (i = 0; i < 16; i++){

     	//if a file exists at the location in memory that we give it
		if (buffer[32*i] != 0x0){

			//for the first 6 chars
			for (j=0; j < 6; j++){
				
				//if the buffer position(char) is equal to the same char at the filename position it is correct
				if (buffer[j+32*i] == fileName[j]){
					fileFound = 1;	
				}

				//if not we have the wrong file
				else {
					fileFound = 0;
				}
			}

		//If a file doesnt exist at the location that we give it then return 0	
		}else{
			fileFound = 0;
		}
	
	}
	return fileFound;

}

