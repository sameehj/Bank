#include "list.h"


int list::open_account(int ID,int Pass,int amount,int atmID){
	pthread_mutex_lock(&lock_all_rd);
	all_rd_cnt++;
	if(all_rd_cnt==1){
		pthread_mutex_lock(&lock_all_wrt);
	}
	pthread_mutex_unlock(&lock_all_rd);
	pthread_mutex_lock(&lock_add_wrt);
	if(getAccount(ID)){
		pthread_mutex_lock(&lock_print);
		to << "Error " << atmID <<": Your transaction failed - account with the same id exists" << endl;
		sleep(1);
		pthread_mutex_unlock(&lock_print);
	} else{
		node* temp = new node(ID,amount,Pass);
		if(!numaccount){
			head=temp;
			tail=temp;
			numaccount++;
		}else{
			tail->next=temp;
			tail=temp;
			numaccount++;
		}
		pthread_mutex_lock(&lock_print);
		to <<  atmID << ": New account id is " << ID << " with password " << Pass << " and initial balance " << amount << endl; 
		sleep(1);
		pthread_mutex_unlock(&lock_print);


	}


	pthread_mutex_unlock(&lock_add_wrt);
	pthread_mutex_lock(&lock_all_rd);
	all_rd_cnt--;
	if(all_rd_cnt==0){
		pthread_mutex_unlock(&lock_all_wrt);
	}
	pthread_mutex_unlock(&lock_all_rd);
	//printf("***open**\n");
}


int list::deposit(int ID,int Pass,int amount,int atmID){
	pthread_mutex_lock(&lock_all_rd);
	all_rd_cnt++;
	if(all_rd_cnt==1){
		pthread_mutex_lock(&lock_all_wrt);
	}
	pthread_mutex_unlock(&lock_all_rd);

	pthread_mutex_lock(&lock_add_rd);
	add_rd_cnt++;
	if(add_rd_cnt==1){
		pthread_mutex_lock(&lock_add_wrt);
	}
	pthread_mutex_unlock(&lock_add_rd);

	node* temp=getAccount(ID);



	if(!temp){
		pthread_mutex_lock(&lock_print);
		to << "Error " << atmID <<": Your transaction failed - account id " << ID << " does not exist" << endl;
		sleep(1);
		pthread_mutex_unlock(&lock_print);
	}else if(temp->Pass!=Pass){
		pthread_mutex_lock(&lock_print);
		to << "Error " << atmID <<": Your transaction failed - password for account id " << ID << " is incorrect" << endl;
		sleep(1);
		pthread_mutex_unlock(&lock_print);
	}else {
		pthread_mutex_lock(&(temp->lock_chng_wrt));
		(temp->amount)+=amount;
		pthread_mutex_lock(&lock_print);
		to <<  atmID << ": Account "<< ID <<" new balance is "<< temp->amount <<" after " << amount << " $ was deposited" << endl;
		sleep(1);
		pthread_mutex_unlock(&lock_print);
		pthread_mutex_unlock(&(temp->lock_chng_wrt));
	}




	pthread_mutex_lock(&lock_add_rd);
	add_rd_cnt--;
	if(add_rd_cnt==0){
		pthread_mutex_unlock(&lock_add_wrt);
	}
	pthread_mutex_unlock(&lock_add_rd);

	pthread_mutex_lock(&lock_all_rd);
	all_rd_cnt--;
	if(all_rd_cnt==0){
		pthread_mutex_unlock(&lock_all_wrt);
	}
	pthread_mutex_unlock(&lock_all_rd);
	//printf("***D****\n");
}


int list::withdrawal(int ID,int Pass,int amount,int atmID){
	pthread_mutex_lock(&lock_all_rd);
	all_rd_cnt++;
	if(all_rd_cnt==1){
		pthread_mutex_lock(&lock_all_wrt);
	}
	pthread_mutex_unlock(&lock_all_rd);

	pthread_mutex_lock(&lock_add_rd);
	add_rd_cnt++;
	if(add_rd_cnt==1){
		pthread_mutex_lock(&lock_add_wrt);
	}
	pthread_mutex_unlock(&lock_add_rd);

	node* temp=getAccount(ID);



	if(!temp){
		pthread_mutex_lock(&lock_print);
		to << "Error " << atmID <<": Your transaction failed - account id " << ID << " does not exist" << endl;
		sleep(1);
		pthread_mutex_unlock(&lock_print);
	}else if(temp->Pass!=Pass){
		pthread_mutex_lock(&lock_print);
		to << "Error " << atmID <<": Your transaction failed - password for account id " << ID << " is incorrect" << endl;
		sleep(1);
		pthread_mutex_unlock(&lock_print);
	}else if(temp->amount<amount){
		pthread_mutex_lock(&lock_print);
		to << "Error " << atmID <<": Your transaction failed - account id "<< ID << " balance is lower than " << amount << endl;
		sleep(1);
		pthread_mutex_unlock(&lock_print);
	} else{
		pthread_mutex_lock(&(temp->lock_chng_wrt));
		temp->amount-=amount;
		pthread_mutex_lock(&lock_print);
		to <<  atmID << ": Account "<< ID <<" new balance is "<< temp->amount <<" after " << amount << " $ was withdrew" << endl;
		sleep(1);
		pthread_mutex_unlock(&lock_print);
		pthread_mutex_unlock(&(temp->lock_chng_wrt));
	}




	pthread_mutex_lock(&lock_add_rd);
	add_rd_cnt--;
	if(add_rd_cnt==0){
		pthread_mutex_unlock(&lock_add_wrt);
	}
	pthread_mutex_unlock(&lock_add_rd);

	pthread_mutex_lock(&lock_all_rd);
	all_rd_cnt--;
	if(all_rd_cnt==0){
		pthread_mutex_unlock(&lock_all_wrt);
	}
	pthread_mutex_unlock(&lock_all_rd);
	//printf("***W****\n");
}
int list::balance(int ID,int Pass,int atmID){

	pthread_mutex_lock(&lock_all_taxrd);	
	all_taxrd_cnt++;
	if(all_taxrd_cnt==1){
		pthread_mutex_lock(&lock_all_taxwrt);
	}
	pthread_mutex_unlock(&lock_all_taxrd);

	pthread_mutex_lock(&lock_add_rd);	
	add_rd_cnt++;
	if(add_rd_cnt==1){
		pthread_mutex_lock(&lock_add_wrt);
	}
	pthread_mutex_unlock(&lock_add_rd);

	node* temp=getAccount(ID);



	if(!temp){
		pthread_mutex_lock(&lock_print);
		to << "Error " << atmID <<": Your transaction failed - account id " << ID << " does not exist" << endl;
		sleep(1);
		pthread_mutex_unlock(&lock_print);
	}else if(temp->Pass!=Pass){
		pthread_mutex_lock(&lock_print);
		to << "Error " << atmID <<": Your transaction failed - password for account id " << ID << " is incorrect" << endl;
		sleep(1);
		pthread_mutex_unlock(&lock_print);
	}else{
		pthread_mutex_lock(&(temp->lock_chng_rd));
		temp->chng_rd_cnt++;
		if(temp->chng_rd_cnt==1){
			pthread_mutex_lock(&(temp->lock_chng_wrt));
		}
		pthread_mutex_unlock(&(temp->lock_chng_rd));

		pthread_mutex_lock(&lock_print);
		to <<  atmID << ": Account "<< ID <<" balance is "<< temp->amount << endl;
		sleep(1);
		pthread_mutex_unlock(&lock_print);

		pthread_mutex_lock(&(temp->lock_chng_rd));
		temp->chng_rd_cnt--;
		if(temp->chng_rd_cnt==0){
			pthread_mutex_unlock(&(temp->lock_chng_wrt));
		}
		pthread_mutex_unlock(&(temp->lock_chng_rd));
	}




	pthread_mutex_lock(&lock_add_rd);
	add_rd_cnt--;
	if(add_rd_cnt==0){
		pthread_mutex_unlock(&lock_add_wrt);
	}
	pthread_mutex_unlock(&lock_add_rd);

	pthread_mutex_lock(&lock_all_taxrd);	
	all_taxrd_cnt--;
	if(all_taxrd_cnt==0){
		pthread_mutex_unlock(&lock_all_taxwrt);
	}
	pthread_mutex_unlock(&lock_all_taxrd);
	//printf("***B****\n");
}


int list::transfer(int ID,int Pass,int targetID,int amount,int atmID){

	pthread_mutex_lock(&lock_all_rd);
	all_rd_cnt++;
	if(all_rd_cnt==1){
		pthread_mutex_lock(&lock_all_wrt);
	}
	pthread_mutex_unlock(&lock_all_rd);

	pthread_mutex_lock(&lock_add_rd);
	add_rd_cnt++;
	if(add_rd_cnt==1){
		pthread_mutex_lock(&lock_add_wrt);
	}
	pthread_mutex_unlock(&lock_add_rd);


	node* temp=getAccount(ID);
	node* tempT=getAccount(targetID);





	if(!temp){
		pthread_mutex_lock(&lock_print);
		to << "Error " << atmID <<": Your transaction failed - account id " << ID << " does not exist" << endl;
		sleep(1);
		pthread_mutex_unlock(&lock_print);
	}else if(temp->Pass!=Pass){
		pthread_mutex_lock(&lock_print);
		to << "Error " << atmID <<": Your transaction failed - password for account id " << ID << " is incorrect" << endl;
		sleep(1);
		pthread_mutex_unlock(&lock_print);
	}else if(temp->amount < amount){
		pthread_mutex_lock(&lock_print);
		to << "Error " << atmID <<": Your transaction failed - account id "<< ID << " balance is lower than " << amount << endl;
		sleep(1);
		pthread_mutex_unlock(&lock_print);
	}else if(!tempT){
		pthread_mutex_lock(&lock_print);
		to << "Error " << atmID <<": Your transaction failed - account id " << targetID << " does not exist" << endl;
		sleep(1);
		pthread_mutex_unlock(&lock_print);
	}else{
		if(temp->ID > tempT->ID){
			pthread_mutex_lock(&(temp->lock_chng_wrt));
			pthread_mutex_lock(&(tempT->lock_chng_wrt));
		} else{
			pthread_mutex_lock(&(tempT->lock_chng_wrt));
			pthread_mutex_lock(&(temp->lock_chng_wrt));
		}

		temp->amount-=amount;
		tempT->amount+=amount;
		pthread_mutex_lock(&lock_print);
		to <<  atmID << ": Transfer " << amount << " from account " << temp->ID << " to account " << tempT->ID << " new account balance is " << temp->amount << " new target account balance is " << tempT->amount << endl;
		sleep(1);
		pthread_mutex_unlock(&lock_print);

		if(temp->ID > tempT->ID){
			pthread_mutex_unlock(&(tempT->lock_chng_wrt));
			pthread_mutex_unlock(&(temp->lock_chng_wrt));
		} else{
			pthread_mutex_unlock(&(temp->lock_chng_wrt));
			pthread_mutex_unlock(&(tempT->lock_chng_wrt));
		}
	}




	pthread_mutex_lock(&lock_add_rd);
	add_rd_cnt--;
	if(add_rd_cnt==0){
		pthread_mutex_unlock(&lock_add_wrt);
	}
	pthread_mutex_unlock(&lock_add_rd);

	pthread_mutex_lock(&lock_all_rd);
	all_rd_cnt--;
	if(all_rd_cnt==0){
		pthread_mutex_unlock(&lock_all_wrt);
	}
	pthread_mutex_unlock(&lock_all_rd);
	//printf("***T****\n");
}


int list::tax(){
	int i;
	int curtax;
	///////////////////////////////////
	int randnum = ((rand() % 3) + 2);
	//////////////////////////////////

	pthread_mutex_lock(&lock_all_wrt);
	pthread_mutex_lock(&lock_all_taxwrt);

	pthread_mutex_lock(&lock_print);

	node* temp=head;
	for (i=0 ; i < numaccount ; i++){	
		curtax = (temp->amount)*randnum / 100;
		taxsum+=curtax;
		temp->amount -= curtax;
		to << " Bank: commissions of " << randnum << " % were charged, the bank gained " << curtax << "$ from account " << temp->ID << endl;
		temp=temp->next;
	}

	pthread_mutex_unlock(&lock_print);

	pthread_mutex_unlock(&lock_all_taxwrt);
	pthread_mutex_unlock(&lock_all_wrt);
	//printf("***tax****\n");
}


void list::print(){


	int i;

	pthread_mutex_lock(&lock_all_wrt);

	node* arr[numaccount];
	int	temptaxsum =  taxsum;
	node* ptr = head;
	int k=numaccount;
	for(i=0 ; i< k ; i++){
		node* temp = new node(ptr->ID,ptr->amount,ptr->Pass);
		arr[i]=temp;
		ptr=ptr->next;
	}
	pthread_mutex_unlock(&lock_all_wrt);
	int j;
	for(i=0 ; i< k ; i++){

		for(j=0 ; j< k ; j++){

			if(j!=k-1){
				if(arr[j]->ID > arr[j+1]->ID){
					node* tmp=arr[j+1];
					arr[j+1]=arr[j];
					arr[j]=tmp;

				}
			} 
		}

	}
	//pthread_mutex_lock(&lock_print);
	printf("\033[2J");
	printf("\033[1;1H");
	printf("Current Bank Status\n");
	for(i=0 ; i<k ; i++){
		printf("Account %d: Balance  %d $ , Account Password  %d\n",arr[i]->ID,arr[i]->amount,arr[i]->Pass);
	}
	printf("The Bank has %d $\n",temptaxsum);
	for(i=0 ; i<k ; i++){
		delete arr[i];
	}
	//pthread_mutex_unlock(&lock_print);

}
node::~node(){
	pthread_mutex_destroy(&lock_chng_rd);
	pthread_mutex_destroy(&lock_chng_wrt);
}
list::~list(){

	pthread_mutex_destroy(&lock_all_rd);
	pthread_mutex_destroy(&lock_all_wrt);
	pthread_mutex_destroy(&lock_add_rd);
	pthread_mutex_destroy(&lock_add_wrt);
	pthread_mutex_destroy(&lock_print);

	node* ptr = head;
	node* temp = ptr;
	while(ptr){
		temp = ptr;
		ptr = ptr->next;
		delete temp;
	}

}
node* list::getAccount(int ID){
	int i;
	node* ptr=head;
	for(i=0;i<numaccount;i++){
		if(ptr->ID==ID){
			return ptr;
		}
		ptr = ptr->next;
	}

	return NULL;
}
