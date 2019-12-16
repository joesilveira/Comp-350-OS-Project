//Alex Stalter
//Joe Silveira

void printHelp();

void main(){



//	char line[512];
//        char filename[6];
//        char programName[6];
//        char fileBuff[512];
//        int sectorsRead;
//        int i;
//       int pos;
//        int pos2;

	while(1){


		char line[512];
		char newFileBuff[512];
		char filename[6];
		char text[512];
		int numLines = 0;
		int input = 1;
		char programName[6];
		char filename1[512];
		char filename2[512];
		char fileBuff[13312];
		int sectorsRead;
		int i,j,k;
		int pos;
		int pos2;
		int pos3 = 0;

		syscall(0,"\n\r");
		syscall(0,"Type 'help' to see list of commands\n\r");
		syscall(0,"SHELL>");
		syscall(1,line);



		if(line[0]=='e'&&line[1]=='x'&&line[2]=='i'&&line[3]=='t'){
			syscall(5);

		}else if(line[0]=='d'&&line[1]=='i'&&line[2]=='r'){

			syscall(0,"\r\n");
			syscall(0,"**Directory Files**\r\n");
			syscall(9); 
			syscall(0,"\r");
			break;
			

		}else if(line[0]=='t'&&line[1]=='y'&&line[2]=='p'&&line[3]=='e'){


		//If the char at line[5] exists 
			pos = 5;

			if(line[5] != 0x0){

			//Loop through the user entry
				for(i = 0; i < 6; i++){
					filename[i] = line[pos + i];	
				}

				filename[6] = 0x0;
				//syscall(0,filename);
				syscall(3,filename,fileBuff,&sectorsRead);
				syscall(0,fileBuff);

				if(sectorsRead>0){
					syscall(0,fileBuff);
					syscall(0,"\n\r");
					break;
				}else{

					//syscall(0,"\r\nNo File found with the specified name\n\r");
					break;
				}
			}else{
				syscall(0,"\r\nPlease enter a file name\n\r");
				break;
			}

		//break;

		}else if(line[0]=='e'&&line[1]=='x'&&line[2]=='e'&&line[3]=='c'){

//		syscall(0,"exec");

//		 pos2 = 5;

			if(line[5] != 0x0){

		 	//Loop through the user entry
//		 	for(i = 0; i < 6; i++){
//		 		programName[i] = line[pos + i];	
//		 	}

//		 	programName[6] = 0x0;

				syscall(4,line+5);

			}else{
				syscall(0,"\r\nPlease enter a program name\n\r");
				break;
			}


		}else if(line[0]=='d'&&line[1]=='e'&&line[2]=='l'){
			

			for(i = 0; i < 6; i++)
			{
				filename[i] = line[4 + i];    
			}

			filename[6] = 0x0;

			syscall(7,filename);
			syscall(0,"**File Deleted**\n\r");

		}else if(line[0]=='c'&&line[1]=='o'&&line[2]=='p'&&line[3]=='y'){

		int k = 0;
		pos = 5;
		i = 0;

		for(j = 0;j < 6; j++)
		{
			filename1[i] = line[pos + j];
			i++;
		}
		filename1[6] = '\0';
		syscall(0,filename1);

		i =0;
		for(j = 7; j < 13; j++)
		{
			filename2[i] = line[pos + j];
			i++;
		}
	
		filename2[6] = '\0';
		syscall(0,filename2);
	

		syscall(3,filename1, fileBuff,&sectorsRead);
		syscall(8,filename2,fileBuff,1);

		
 		break;

		}else if(line[0]=='c'&&line[1]=='r'&&line[2]=='e'&&line[3]=='a'&&line[4]=='t'&&line[5]=='e'){
			syscall(0,"\n\r");
			pos = 7;


			//Positioning
			for(i = 0; i < 6; i++){
				filename[i] = line[pos + i];
			}

			filename[6] = '\0';

			//some stuff for user
			syscall(0,"***New File Created***\n\r");
			syscall(0,"\n\r");
			syscall(0,"			");
			syscall(0,filename);
			syscall(0,"			");
			syscall(0,"\n\r");
		



			//Start creating
			syscall(0,"Please Enter the text for the file below\n\r");
			syscall(0,"\n\r");

			while(input == 1){
				syscall(0,"\t:");
				syscall(1,text,0,0);

				if(text[2] == '\0'){
					syscall(0,"END OF FILE");
					syscall(0,"\n\r");
					input = 0;
					break;
				}else{
					for(i = 0; i < 512; i++){
						newFileBuff[i+512*pos3] = text[i];
					}
					numLines++;
					pos3++;
				}

			}
			syscall(8,filename,newFileBuff,numLines);


			break;
	
		}else if(line[0]=='h' && line[1] == 'e' && line[2] == 'l' && line[3] == 'p'){
			printHelp();
		}

		else {

			syscall(0,"Invalid Input\n\r");


		}

	}
}

void printHelp(){
	syscall(0,"--help\n\r");
	syscall(0,"Enter Commands immeditaly following > EX: Shell>myCommand\n\r");
	syscall(0,"Shell commands are listed below\n\r");
	syscall(0,"\n\r");
	syscall(0,"exit -Terminates Shell; EX: Shell>exit\n\r");
	syscall(0,"dir -Lists all files in the directory; EX: Shell>dir\n\r");
	syscall(0,"type[filename] -Prints given file out to screen; EX: Shell>type file name\n\r");
	syscall(0,"exec[programname] -Executes given program; EX: Shell>exec program name\n\r");
	syscall(0,"copy[Source File Name] [New File Name] -Copy's a given file; EX: Shell>copy file1 file 2\n\r");
	syscall(0,"create[filename] - Creates new text file; EX: Shell>create newFile\n\r");
	syscall(0,"del[filename] -Deletes given file; EX: Shell> del filename\n\r");
}
