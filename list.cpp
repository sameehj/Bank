#include "list.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//READERS WRITERS 2015S
int BankAccountsList::open_account(int id, int password, int amount, int atmID){

	pthread_mutex_lock(&all_readers);
	readers_count++;
	
	if(readers_count==1){
		pthread_mutex_lock(&all_writers);
	}
	pthread_mutex_unlock(&all_readers);
	pthread_mutex_lock(&add_writer);
	if(getAccount(id)){
		pthread_mutex_lock(&print_mutex);
		to << "Error " << atmID <<": Your transaction failed - account with the same id exists" << endl;
		sleep(1);
		pthread_mutex_unlock(&print_mutex);
	} else{
		BankAccount* temp = new BankAccount(id,amount,password);
		if(!numaccount){
			head=temp;
			tail=temp;
			numaccount++;
		}else{
			tail->Next=temp;
			tail=temp;
			numaccount++;
		}
		pthread_mutex_lock(&print_mutex);
		to <<  atmID << ": New account id is " << id << " with password " << password << " and initial balance " << amount << endl; 
		sleep(1);
		pthread_mutex_unlock(&print_mutex);
	}


	pthread_mutex_unlock(&add_writer);
	pthread_mutex_lock(&all_readers);
	readers_count--;
	if(readers_count==0){
		pthread_mutex_unlock(&all_writers);
	}
	pthread_mutex_unlock(&all_readers);
}

//READERS WRITERS 2015S
int BankAccountsList::deposit(int id,int password,int amount,int atmID){
	pthread_mutex_lock(&all_readers);
	readers_count++;
	if(readers_count==1){
		pthread_mutex_lock(&all_writers);
	}
	pthread_mutex_unlock(&all_readers);

	pthread_mutex_lock(&add_reader);
	add_readers_count++;
	if(add_readers_count==1){
		pthread_mutex_lock(&add_writer);
	}
	pthread_mutex_unlock(&add_reader);

	BankAccount* temp=getAccount(id);
	if(!temp){
		pthread_mutex_lock(&print_mutex);
		to << "Error " << atmID <<": Your transaction failed - account id " << id << " does not exist" << endl;
		sleep(1);
		pthread_mutex_unlock(&print_mutex);
	}else if(temp->password!=password){
		pthread_mutex_lock(&print_mutex);
		to << "Error " << atmID <<": Your transaction failed - password for account id " << id << " is incorrect" << endl;
		sleep(1);
		pthread_mutex_unlock(&print_mutex);
	}else {
		pthread_mutex_lock(&(temp->chang_writer));
		(temp->amount)+=amount;
		pthread_mutex_lock(&print_mutex);
		to <<  atmID << ": Account "<< id <<" new balance is "<< temp->amount <<" after " << amount << " $ was deposited" << endl;
		sleep(1);
		pthread_mutex_unlock(&print_mutex);
		pthread_mutex_unlock(&(temp->chang_writer));
	}

	pthread_mutex_lock(&add_reader);
	add_readers_count--;
	if(add_readers_count==0){
		pthread_mutex_unlock(&add_writer);
	}
	pthread_mutex_unlock(&add_reader);

	pthread_mutex_lock(&all_readers);
	readers_count--;
	if(readers_count==0){
		pthread_mutex_unlock(&all_writers);
	}
	pthread_mutex_unlock(&all_readers);
}

//READERS WRITERS 2015S
int BankAccountsList::withdrawal(int id,int password,int amount,int atmID){
	pthread_mutex_lock(&all_readers);
	readers_count++;
	if(readers_count==1){
		pthread_mutex_lock(&all_writers);
	}
	pthread_mutex_unlock(&all_readers);

	pthread_mutex_lock(&add_reader);
	add_readers_count++;
	if(add_readers_count==1){
		pthread_mutex_lock(&add_writer);
	}
	pthread_mutex_unlock(&add_reader);

	BankAccount* temp=getAccount(id);
	
	if(!temp){
		pthread_mutex_lock(&print_mutex);
		to << "Error " << atmID <<": Your transaction failed - account id " << id << " does not exist" << endl;
		sleep(1);
		pthread_mutex_unlock(&print_mutex);
	}else if(temp->password!=password){
		pthread_mutex_lock(&print_mutex);
		to << "Error " << atmID <<": Your transaction failed - password for account id " << id << " is incorrect" << endl;
		sleep(1);
		pthread_mutex_unlock(&print_mutex);
	}else if(temp->amount<amount){
		pthread_mutex_lock(&print_mutex);
		to << "Error " << atmID <<": Your transaction failed - account id "<< id << " balance is lower than " << amount << endl;
		sleep(1);
		pthread_mutex_unlock(&print_mutex);
	} else{
		pthread_mutex_lock(&(temp->chang_writer));
		temp->amount-=amount;
		pthread_mutex_lock(&print_mutex);
		to <<  atmID << ": Account "<< id <<" new balance is "<< temp->amount <<" after " << amount << " $ was withdrew" << endl;
		sleep(1);
		pthread_mutex_unlock(&print_mutex);
		pthread_mutex_unlock(&(temp->chang_writer));
	}
	
	pthread_mutex_lock(&add_reader);
	add_readers_count--;
	if(add_readers_count==0){
		pthread_mutex_unlock(&add_writer);
	}
	pthread_mutex_unlock(&add_reader);

	pthread_mutex_lock(&all_readers);
	readers_count--;
	if(readers_count==0){
		pthread_mutex_unlock(&all_writers);
	}
	pthread_mutex_unlock(&all_readers);
}

//READERS WRITERS 2015S
int BankAccountsList::balance(int id,int password,int atmID){

	pthread_mutex_lock(&all_tax_readers);	
	all_tax_readers_count++;
	if(all_tax_readers_count==1){
		pthread_mutex_lock(&all_tax_writers);
	}
	pthread_mutex_unlock(&all_tax_readers);

	pthread_mutex_lock(&add_reader);	
	add_readers_count++;
	if(add_readers_count==1){
		pthread_mutex_lock(&add_writer);
	}
	pthread_mutex_unlock(&add_reader);

	BankAccount* temp=getAccount(id);


	if(!temp){
		pthread_mutex_lock(&print_mutex);
		to << "Error " << atmID <<": Your transaction failed - account id " << id << " does not exist" << endl;
		sleep(1);
		pthread_mutex_unlock(&print_mutex);
	}else if(temp->password!=password){
		pthread_mutex_lock(&print_mutex);
		to << "Error " << atmID <<": Your transaction failed - password for account id " << id << " is incorrect" << endl;
		sleep(1);
		pthread_mutex_unlock(&print_mutex);
	}else{
		pthread_mutex_lock(&(temp->chang_reader));
		temp->readers_count++;
		if(temp->readers_count==1){
			pthread_mutex_lock(&(temp->chang_writer));
		}
		pthread_mutex_unlock(&(temp->chang_reader));

		pthread_mutex_lock(&print_mutex);
		to <<  atmID << ": Account "<< id <<" balance is "<< temp->amount << endl;
		sleep(1);
		pthread_mutex_unlock(&print_mutex);

		pthread_mutex_lock(&(temp->chang_reader));
		temp->readers_count--;
		if(temp->readers_count==0){
			pthread_mutex_unlock(&(temp->chang_writer));
		}
		pthread_mutex_unlock(&(temp->chang_reader));
	}


	pthread_mutex_lock(&add_reader);
	add_readers_count--;
	if(add_readers_count==0){
		pthread_mutex_unlock(&add_writer);
	}
	pthread_mutex_unlock(&add_reader);

	pthread_mutex_lock(&all_tax_readers);	
	all_tax_readers_count--;
	if(all_tax_readers_count==0){
		pthread_mutex_unlock(&all_tax_writers);
	}
	pthread_mutex_unlock(&all_tax_readers);
}

//READERS WRITERS 2015S
int BankAccountsList::transfer(int id,int password,int targetID,int amount,int atmID){

	pthread_mutex_lock(&all_readers);
	readers_count++;
	if(readers_count==1){
		pthread_mutex_lock(&all_writers);
	}
	pthread_mutex_unlock(&all_readers);

	pthread_mutex_lock(&add_reader);
	add_readers_count++;
	if(add_readers_count==1){
		pthread_mutex_lock(&add_writer);
	}
	pthread_mutex_unlock(&add_reader);


	BankAccount* temp=getAccount(id);
	BankAccount* tempT=getAccount(targetID);


	if(!temp){
		pthread_mutex_lock(&print_mutex);
		to << "Error " << atmID <<": Your transaction failed - account id " << id << " does not exist" << endl;
		sleep(1);
		pthread_mutex_unlock(&print_mutex);
	}else if(temp->password!=password){
		pthread_mutex_lock(&print_mutex);
		to << "Error " << atmID <<": Your transaction failed - password for account id " << id << " is incorrect" << endl;
		sleep(1);
		pthread_mutex_unlock(&print_mutex);
	}else if(temp->amount < amount){
		pthread_mutex_lock(&print_mutex);
		to << "Error " << atmID <<": Your transaction failed - account id "<< id << " balance is lower than " << amount << endl;
		sleep(1);
		pthread_mutex_unlock(&print_mutex);
	}else if(!tempT){
		pthread_mutex_lock(&print_mutex);
		to << "Error " << atmID <<": Your transaction failed - account id " << targetID << " does not exist" << endl;
		sleep(1);
		pthread_mutex_unlock(&print_mutex);
	}else{
		if(temp->id > tempT->id){
			pthread_mutex_lock(&(temp->chang_writer));
			pthread_mutex_lock(&(tempT->chang_writer));
		} else{
			pthread_mutex_lock(&(tempT->chang_writer));
			pthread_mutex_lock(&(temp->chang_writer));
		}

		temp->amount-=amount;
		tempT->amount+=amount;
		pthread_mutex_lock(&print_mutex);
		to <<  atmID << ": Transfer " << amount << " from account " << temp->id << " to account " << tempT->id << " new account balance is " << temp->amount << " new target account balance is " << tempT->amount << endl;
		sleep(1);
		pthread_mutex_unlock(&print_mutex);

		if(temp->id > tempT->id){
			pthread_mutex_unlock(&(tempT->chang_writer));
			pthread_mutex_unlock(&(temp->chang_writer));
		} else{
			pthread_mutex_unlock(&(temp->chang_writer));
			pthread_mutex_unlock(&(tempT->chang_writer));
		}
	}


	pthread_mutex_lock(&add_reader);
	add_readers_count--;
	if(add_readers_count==0){
		pthread_mutex_unlock(&add_writer);
	}
	pthread_mutex_unlock(&add_reader);

	pthread_mutex_lock(&all_readers);
	readers_count--;
	if(readers_count==0){
		pthread_mutex_unlock(&all_writers);
	}
	pthread_mutex_unlock(&all_readers);
}

//READERS WRITERS 2015S
int BankAccountsList::tax(){
	int i;
	int curtax;
	int randnum = ((rand() % 3) + 2);

	pthread_mutex_lock(&all_writers);
	pthread_mutex_lock(&all_tax_writers);

	pthread_mutex_lock(&print_mutex);

	BankAccount* temp=head;
	for (i=0 ; i < numaccount ; i++){	
		curtax = (temp->amount)*randnum / 100;
		taxsum+=curtax;
		temp->amount -= curtax;
		to << " Bank: commissions of " << randnum << " % were charged, the bank gained " << curtax << "$ from account " << temp->id << endl;
		temp=temp->Next;
	}

	pthread_mutex_unlock(&print_mutex);

	pthread_mutex_unlock(&all_tax_writers);
	pthread_mutex_unlock(&all_writers);
}

//READERS WRITERS 2015S
void BankAccountsList::print(){


	int i;

	pthread_mutex_lock(&all_writers);

	BankAccount* arr[numaccount];
	int	tempTax =  taxsum;
	BankAccount* itr = head;
	int k=numaccount;
	for(i=0 ; i< k ; i++){
		BankAccount* temp = new BankAccount(itr->id,itr->amount,itr->password);
		arr[i]=temp;
		itr=itr->Next;
	}
	pthread_mutex_unlock(&all_writers);
	int j;
	for(i=0 ; i< k ; i++){

		for(j=0 ; j< k ; j++){

			if(j!=k-1){
				if(arr[j]->id > arr[j+1]->id){
					BankAccount* tmp=arr[j+1];
					arr[j+1]=arr[j];
					arr[j]=tmp;

				}
			} 
		}

	}
	printf("\033[2J");
	printf("\033[1;1H");
	printf("Current Bank Status\n");
	for(i=0 ; i<k ; i++){
		printf("Account %d: Balance  %d $ , Account Password  %d\n",arr[i]->id,arr[i]->amount,arr[i]->password);
	}
	printf("The Bank has %d $\n",tempTax);
	for(i=0 ; i<k ; i++){
		delete arr[i];
	}
}
//READERS WRITERS 2015S
BankAccount::~BankAccount(){
	pthread_mutex_destroy(&chang_reader);
	pthread_mutex_destroy(&chang_writer);
}
//READERS WRITERS 2015S
BankAccountsList::~BankAccountsList(){

	pthread_mutex_destroy(&all_readers);
	pthread_mutex_destroy(&all_writers);
	pthread_mutex_destroy(&add_reader);
	pthread_mutex_destroy(&add_writer);
	pthread_mutex_destroy(&print_mutex);

	BankAccount* itr = head;
	BankAccount* tempAccount = itr;
	while(itr){
		tempAccount = itr;
		itr = itr->Next;
		delete tempAccount;
	}

}
BankAccount* BankAccountsList::getAccount(int id){
	int i;
	BankAccount* itr=head;
	for(i=0;i<numaccount;i++){
		if(itr->id==id){
			return itr;
		}
		itr = itr->Next;
	}

	return NULL;
}
