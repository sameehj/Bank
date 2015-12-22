#ifndef list_
#define list_
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;
using std::ostream;
extern ofstream to;

class BankAccount{

	public:
		int id;
		int amount;
		int password;
		int readers_count;
		
		pthread_mutex_t chang_reader;
		pthread_mutex_t chang_writer;
		BankAccount* Next;
		
		BankAccount(int id,int amount,int password):id(id),amount(amount),password(password),readers_count(0){ // constructor
			if (pthread_mutex_init(&chang_reader, NULL) != 0)
			{	
				perror(NULL);
				exit(0);
			}
			if (pthread_mutex_init(&chang_writer, NULL) != 0)
			{
				perror(NULL);
				exit(0);
			}
		}
		~BankAccount(); // destructor
};

class BankAccountsList{

	pthread_mutex_t all_readers;   // Mutexes
	pthread_mutex_t all_writers;
	pthread_mutex_t add_reader;
	pthread_mutex_t add_writer;
	pthread_mutex_t all_tax_readers;
	pthread_mutex_t all_tax_writers;
	pthread_mutex_t print_mutex;
	
	
	BankAccount* head;
	BankAccount* tail;
	int taxsum;
	int numaccount;
	int readers_count;
	int add_readers_count;
	int all_tax_readers_count;

	BankAccount* getAccount(int id);

	public:
	BankAccountsList():head(NULL),tail(NULL),taxsum(0),numaccount(0),readers_count(0),add_readers_count(0),all_tax_readers_count(0){

		if (pthread_mutex_init(&all_readers, NULL) != 0)  // mutex init
		{	
			perror(NULL);
			exit(0);
		}
		if (pthread_mutex_init(&all_writers, NULL) != 0)
		{
			perror(NULL);
			exit(0);
		}
		if (pthread_mutex_init(&add_reader, NULL) != 0)
		{	
			perror(NULL);
			exit(0);
		}
		if (pthread_mutex_init(&add_writer, NULL) != 0)
		{
			perror(NULL);
			exit(0);
		}
		if (pthread_mutex_init(&all_tax_readers, NULL) != 0)
		{	
			perror(NULL);
			exit(0);
		}
		if (pthread_mutex_init(&all_tax_writers, NULL) != 0)
		{
			perror(NULL);
			exit(0);
		}
		if (pthread_mutex_init(&print_mutex, NULL) != 0)
		{	
			perror(NULL);
			exit(0);
		}

	}
	
	// functions for List
	int open_account(int id,int password,int amount,int atmID);
	int deposit(int id,int password,int amount,int atmID);
	int withdrawal(int id,int password,int amount,int atmID);
	int balance(int id,int password,int atmID);
	int transfer(int id,int password,int targetID,int amount,int atmID);
	int tax();
	void print();
	~BankAccountsList();
};




#endif

