//Alex Stalter

void main(){

	
	while(1){


	char line[512];
	char filename[6];
	char programName[6];
	char fileBuff[512];
	int sectorsRead;
	int i;
	int pos;
	int pos2;
	
	syscall(0,"\r\n");
	syscall(0,"SHELL>");
	syscall(1,line);



	if(line[0]=='e'&&line[1]=='x'&&line[2]=='i'&&line[3]=='t'){
		syscall(5);

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
				break;
			}else{

				syscall(0,"No File found with the specified name");
				break;
			}
		}else{
			break;
		}

		break;

	}else if(line[0]=='e'&&line[1]=='x'&&line[2]=='e'&&line[3]=='c'){

		syscall(0,"exec");

		// pos2 = 5;

		// if(line[5] != 0x0){

		// 	//Loop through the user entry
		// 	for(i = 0; i < 6; i++){
		// 		programName[i] = line[pos + i];	
		// 	}

		// 	programName[6] = 0x0;

		// 	syscall(0,programName);

		// }else{
		// 	break;
		// }



	}else {

	syscall(0,"Invalid Input/n");

	}
}
}


