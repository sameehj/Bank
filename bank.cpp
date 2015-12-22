#include "list.h"
#include "thread.h"


list Bank;
ofstream to("log.txt");
int stillATMs;

int main(int argc, char** argv) {

	if (argc == 1 ){
		cerr << "illegal arguments" << endl;
		exit(0);
	}

	int atms = atoi(argv[1]);

	if (argc-2 != atms ){
		cerr << "illegal arguments" << endl;
		exit(0);
	}


	stillATMs = 1;
	srand(time(NULL));
	pthread_t threads[atms];
	pthread_t taxThread;
	pthread_t printStatusThread;
	struct pthread_data data[atms];
	if (!to) {
		cerr << "cannot open the log file" << endl;
		exit(0);
	}



	pthread_create(&taxThread, NULL, taxbank , NULL );

	pthread_create(&printStatusThread, NULL, printbank , NULL );


	for (int i = 0; i < atms; i++) {
		data[i].threadID = i+1;
		data[i].input_file = argv[i + 2];
		pthread_create(&threads[i], NULL, ATM , &data[i]);
	}

	for (int i = 0; i < atms; i++) {
		pthread_join(threads[i], NULL);
	}
	stillATMs = 0;

	return 0;
}
