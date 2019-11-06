//Alex Stalter

void main(){

	while(1){
	char line[512];


	syscall(0,"SHELL>");
	syscall(1,line);

	if(line[0]=='e'&&line[1]=='x'&&line[2]=='i'&&line[3]=='t'){

	break;

	}else if(line[0]=='t'&&line[1]=='y'&&line[2]=='p'&&line[3]=='e'){

	syscall(3,line+5);
	syscall(0,line);

	}else if(line[0]=='e'&&line[1]=='x'&&line[2]=='e'&&line[3]=='c'){

	syscall(4,line+5);

	}else{

	syscall(0,"Invalid Input/n");

	}
}

}
