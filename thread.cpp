#include "thread.h"


void* ATM(void* args){
	ifstream from(((struct pthread_data*)args)->input_file);
	if (!from){
		cerr << "connot open file " << ((struct pthread_data*)args)->input_file << endl;
		exit(0);
	}
	int atmID = ((struct pthread_data*)args)->threadID;
	const char* delimiters = " \t\n";
	char command[MAXLEN];
	char* commandptr;
	char* now;
	char arg1[MAXLEN], arg2[MAXLEN], arg3[MAXLEN],
	     arg4[MAXLEN], arg5[MAXLEN];
	while (from.getline(command, MAXLEN))
	{
		usleep(100000);

		now = strtok_r(command, delimiters, &commandptr);
		if (now)
			strcpy(arg1, now);

		now = strtok_r(NULL, delimiters, &commandptr);
		if (now)
			strcpy(arg2, now);

		now = strtok_r(NULL, delimiters, &commandptr);
		if (now)
			strcpy(arg3, now);

		now = strtok_r(NULL, delimiters, &commandptr);
		if (now)
			strcpy(arg4, now);

		now = strtok_r(NULL, delimiters, &commandptr);
		if (now)
			strcpy(arg5, now);
		switch (*arg1) {
			case 'O':
				Bank.open_account(atoi(arg2), atoi(arg3), atoi(arg4),atmID);
				break;
			case 'D':
				Bank.deposit(atoi(arg2), atoi(arg3), atoi(arg4),atmID);
				break;
			case 'W':
				Bank.withdrawal(atoi(arg2), atoi(arg3), atoi(arg4),atmID);
				break;
			case 'B':
				Bank.balance(atoi(arg2), atoi(arg3),atmID);
				break;
			case 'T':
				Bank.transfer(atoi(arg2), atoi(arg3), atoi(arg4), atoi(arg5),atmID);
				break;
			default:
				break;
		}
	}
	pthread_exit(NULL);

}

void* printbank(void* args){

	while(stillATMs){
		Bank.print();
		usleep(500000);
	}
	pthread_exit(NULL);
}



void* taxbank(void* args){

	while(stillATMs){
		Bank.tax();	
		usleep(3000000);

	}
	pthread_exit(NULL);
}

