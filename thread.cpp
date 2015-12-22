#include "thread.h"


void* ATM(void* args){
	ifstream from(((struct pthread_data*)args)->input_file);
	if (!from){
		cerr << "connot open file " << ((struct pthread_data*)args)->input_file << endl;
		exit(0);
	}
	int atmID = ((struct pthread_data*)args)->threadId;
	const char* delimiters = " \t\n";
	char command[MAX_LENGTH];
    char arg1[MAX_LENGTH]; 
	char arg2[MAX_LENGTH]; 
	char arg3[MAX_LENGTH];
	char arg4[MAX_LENGTH]; 
	char arg5[MAX_LENGTH];
	char* commandptr;
	char* now;

	while (from.getline(command, MAX_LENGTH))
	{
		usleep(ONE_SECONDS);

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
			case 'W':
				Bank.withdrawal(atoi(arg2), atoi(arg3), atoi(arg4),atmID);
				break;
			case 'O':
				Bank.open_account(atoi(arg2), atoi(arg3), atoi(arg4),atmID);
				break;
			case 'B':
				Bank.balance(atoi(arg2), atoi(arg3),atmID);
				break;
			case 'T':
				Bank.transfer(atoi(arg2), atoi(arg3), atoi(arg4), atoi(arg5),atmID);
				break;

			case 'D':
				Bank.deposit(atoi(arg2), atoi(arg3), atoi(arg4),atmID);
				break;

			default:
				break;
		}
	}
	pthread_exit(NULL);

}

void* printBankAccounts(void* args){

	while(stillATMs){
		Bank.print();
		usleep(FIVE_SECONDS);
	}
	pthread_exit(NULL);
}



void* commisionTaxBank(void* args){

	while(stillATMs){
		Bank.tax();	
		usleep(THREE_SECONDS);

	}
	pthread_exit(NULL);
}

