//Alex Stalter
//Joe Silveira

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
		char filename[6];
		char programName[6];
		char fileBuff[512];
		int sectorsRead;
		int i;
		int pos;
		int pos2;

	//syscall(0,"\n");
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

				syscall(3,filename,fileBuff,&sectorsRead);

				if(sectorsRead>0){
					syscall(0,fileBuff);
					syscall(0,"\r\n");
					break;
				}else{

					syscall(0,"\r\nNo File found with the specified name\n\r");
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


		}else {

			syscall(0,"Invalid Input\n\r");
			syscall(5);

		}

	}
}
