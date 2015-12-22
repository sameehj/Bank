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
class node{
	public:
		int ID;
		int amount;
		int Pass;
		int chng_rd_cnt;
		pthread_mutex_t lock_chng_rd;
		pthread_mutex_t lock_chng_wrt;
		node* next;
		node(int ID,int amount,int Pass):ID(ID),amount(amount),Pass(Pass),chng_rd_cnt(0){
			if (pthread_mutex_init(&lock_chng_rd, NULL) != 0)
			{	
				perror(NULL);
				exit(0);
			}
			if (pthread_mutex_init(&lock_chng_wrt, NULL) != 0)
			{
				perror(NULL);
				exit(0);
			}
		}
		~node();
};
class list{
	node* head;
	node* tail;
	int taxsum;
	int numaccount;
	int all_rd_cnt;
	int add_rd_cnt;
	int all_taxrd_cnt;
	pthread_mutex_t lock_all_rd;
	pthread_mutex_t lock_all_wrt;
	pthread_mutex_t lock_add_rd;
	pthread_mutex_t lock_add_wrt;
	pthread_mutex_t lock_all_taxrd;
	pthread_mutex_t lock_all_taxwrt;
	pthread_mutex_t lock_print;

	node* getAccount(int ID);

	public:
	list():head(NULL),tail(NULL),taxsum(0),numaccount(0),all_rd_cnt(0),add_rd_cnt(0),all_taxrd_cnt(0){

		if (pthread_mutex_init(&lock_all_rd, NULL) != 0)
		{	
			perror(NULL);
			exit(0);
		}
		if (pthread_mutex_init(&lock_all_wrt, NULL) != 0)
		{
			perror(NULL);
			exit(0);
		}
		if (pthread_mutex_init(&lock_add_rd, NULL) != 0)
		{	
			perror(NULL);
			exit(0);
		}
		if (pthread_mutex_init(&lock_add_wrt, NULL) != 0)
		{
			perror(NULL);
			exit(0);
		}
		if (pthread_mutex_init(&lock_all_taxrd, NULL) != 0)
		{	
			perror(NULL);
			exit(0);
		}
		if (pthread_mutex_init(&lock_all_taxwrt, NULL) != 0)
		{
			perror(NULL);
			exit(0);
		}
		if (pthread_mutex_init(&lock_print, NULL) != 0)
		{	
			perror(NULL);
			exit(0);
		}

	}
	int open_account(int ID,int Pass,int amount,int atmID);
	int deposit(int ID,int Pass,int amount,int atmID);
	int withdrawal(int ID,int Pass,int amount,int atmID);
	int balance(int ID,int Pass,int atmID);
	int transfer(int ID,int Pass,int targetID,int amount,int atmID);
	int tax();
	void print();
	~list();
};




#endif

