#ifndef thread_func_
#define thread_func_
#include <iostream>
#include <fstream>
#include <string.h>
#include <pthread.h>
#include "list.h"
extern list Bank;
extern ofstream to;
extern int stillATMs;

#define MAXLEN 100

struct pthread_data {
	public:
		int threadID;
		const char* input_file;
};


using namespace std;
using std::ostream;


void* ATM(void* args);
void* printbank(void* args);
void* taxbank(void* args);




#endif
