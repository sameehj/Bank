#ifndef thread_func_
#define thread_func_
#include <iostream>
#include <fstream>
#include <string.h>
#include <pthread.h>
#include "list.h"
extern BankAccountsList Bank;
extern ofstream to;
extern int stillATMs;

#define MAX_LENGTH 100
#define ONE_SECONDS 100000
#define THREE_SECONDS 300000
#define FIVE_SECONDS 500000


using namespace std;
using std::ostream;


struct pthread_data {
	public:
		int threadId;
		const char* input_file;
};

void* ATM(void* args);
void* printBankAccounts(void* args);
void* commisionTaxBank(void* args);




#endif
