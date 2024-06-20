// Δήλωση βιβλιοθηκών 
#include "stats.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Συνάρτηση για την εμφάνιση μηνύματος λάθους
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

// Δομή για την παράδοση παραμέτρων στο thread
struct thread_args {
    int newsockfd;
    char* host;
};

// Συνάρτηση για την κλήση των συναρτήσεων του RPC
void
add_prog_1(int epil , int newsockfd, int n, int *Y, double a,int done,char * host)
{
	// Δήλωση μεταβλητών
	CLIENT *clnt;
	double  *result_1;
	input1  average_1_arg;
	output1  *result_2;
	input1  min_max1_1_arg;
	output2  *result_3;
	input2  prod_ay_1_arg;

// Ανοίγουμε σύνδεση με τον server
#ifndef	DEBUG
	clnt = clnt_create (host, ADD_PROG, ADD_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	// Αρχικοποίηση των παραμέτρων με την τιμή n
	average_1_arg.y.y_len = n;
	min_max1_1_arg.y.y_len = n;
	prod_ay_1_arg.y.y_len = n;

	// Δευσμεύω δυναμικά μνήμη για τα διάφορα πεδία των παραμέτρων
	average_1_arg.y.y_val = (int *)malloc(n * sizeof(int));
	min_max1_1_arg.y.y_val = (int *)malloc(n * sizeof(int));
	prod_ay_1_arg.y.y_val = (int *)malloc(n * sizeof(int));

	// Ελέγχω αν η δέσμευση μνήμης έγινε επιτυχώς
	if(average_1_arg.y.y_val == NULL || min_max1_1_arg.y.y_val == NULL || prod_ay_1_arg.y.y_val == NULL) {
		error("ERROR on malloc");
	}

	// Αρχικοποίω τιν παραμέτρους με τις τιμές του διανύσματος Y
	for(int i = 0; i < n; i++) {
		average_1_arg.y.y_val[i] = Y[i];
		min_max1_1_arg.y.y_val[i] = Y[i];
		prod_ay_1_arg.y.y_val[i] = Y[i];
		// Εκτυπώνω τις τιμές του διανύσματος Y
		printf("%d ", Y[i]);
	}
	printf("\n");

	// Αρχικοποίηση των μεταβλητών
	int min;
	int max;
	double mean;
	double *aY=(double *)malloc(n * sizeof(double));
	if(aY == NULL) {
		error("ERROR on malloc");
	}
	// Κλήση των συναρτήσεων του RPC
	if(epil==1)
	{
		// Κλήση της συνάρτησης average_1
		result_1 = average_1(&average_1_arg, clnt);
		// Έλεγχος αν η κλήση έγινε επιτυχώς
		if (result_1 == (double *) NULL) {
			clnt_perror (clnt, "call failed");
		}
		else{
			// Αν έγινε επιτυχώς δέχομαι τον μέσο όρο του Y
			mean = *result_1;
			printf("Mean of Y: %f\n", mean);
			// Στέλνω τον μέσο όρο στον client
			send(newsockfd, &mean, sizeof(mean), 0);
		}
	}
	if(epil==2)
	{
		// Κλήση της συνάρτησης min_max1_1
		result_2 = min_max1_1(&min_max1_1_arg, clnt);
		// Έλεγχος αν η κλήση έγινε επιτυχώς
		if (result_2 == (output1 *) NULL) {
			clnt_perror (clnt, "call failed");
		}
		else{
			// Αν έγινε επιτυχώς δέχομαι το min και το max του Y
			min = result_2->min_max[0];
			max = result_2->min_max[1];
			printf("Min of Y: %d\n", min);
			printf("Max of Y: %d\n", max);
			// Στέλνω το min και το max στον client
			send(newsockfd, &min, sizeof(min), 0);
			send(newsockfd, &max, sizeof(max), 0);
		}
	}
	if(epil==3)
	{
		// Αρχικοποίηση της παραμέτρου a
		prod_ay_1_arg.a = a;
		// Κλήση της συνάρτησης prod_ay_1
		result_3 = prod_ay_1(&prod_ay_1_arg, clnt);
		if (result_3 == (output2 *) NULL) {
			clnt_perror (clnt, "call failed");
		}
		else{
			// Αν έγινε επιτυχώς δέχομαι το a*Y
			for(int i = 0; i < n; i++) {
				printf("a*Y[%d] = %f\n", i, result_3->pr_aY.pr_aY_val[i]);
				aY[i] = result_3->pr_aY.pr_aY_val[i];
				// Στέλνω το a*Y στον client
				send(newsockfd,&aY[i], sizeof(double), 0);
			}
		}
	}
	if(epil==4)
	{
		// Αν η επιλογή είναι 4 τότε τερματίζω το πρόγραμμα
		done=1;
	}
	// Αποδεσμεύω την μνήμη που δέσμευσα	
	free(aY);
	free(average_1_arg.y.y_val);
	free(min_max1_1_arg.y.y_val);
	free(prod_ay_1_arg.y.y_val);
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

// Συνάρτηση για την εξυπηρέτηση των clients
void* handle_client(void* arg) {
	// H δομή που περιέχει τις παραμέτρους	
    struct thread_args* args = (struct thread_args*) arg;
    int newsockfd = args->newsockfd;
    char* host = args->host;
    free(arg);

	// Δηλώνω μεταβλητές
	int done = 0;
	int epil;
	double a;
	do
	{
		// Δέχομαι την επιλογή του client
		recv(newsockfd, &epil, sizeof(epil), 0);
		if(epil==4)
		{	
			// Αν η επιλογή είναι 4 τότε τερματίζω το πρόγραμμα
			done=1;
			break;
		}
		printf("epil = %d\n", epil);
		if(epil == 3)
		{
			// Δέχομαι τον αριθμό a αν η επιλογή είναι 3
			recv(newsockfd, &a, sizeof(a), 0);
		}
		int n;
		// Δέχομαι το μήκος του διανύσματος
		recv(newsockfd, &n, sizeof(n), 0);
		// Δηλώνω το διάνυσμα Y
		int* Y = (int * )malloc(n * sizeof(int));
		// Έλεγχος αν η δέσμευση μνήμης έγινε επιτυχώς
		if(Y == NULL) {
			error("ERROR on malloc");
		}
		// Δέχομαι τα στοιχεία του διανύσματος
		for(int i = 0; i < n; i++) {
			recv(newsockfd, &Y[i], sizeof(int), 0);
		}

		printf("n = %d\n", n);
		// Κλήση της συνάρτησης add_prog_1
		add_prog_1(epil,newsockfd,n,Y,a,done,host);
		free(Y);
	}while(!done);
	// Κλείνω την σύνδεση
	close(newsockfd);
	return NULL;
}


int 
main (int argc, char *argv[])
{
	// Δήλωση μεταβλητών
	char *host;
	char *port;
	// Έλεγχος αν ο χρήστης έδωσε τις σωστές παραμέτρους
	if (argc < 3) {
		printf ("usage: %s server_host, %s server_port\n", argv[0], argv[1]);
		exit (1);
	}
	// Αρχικοποίηση των μεταβλητών
	host = argv[1];
	port = argv[2];
	int sockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

	// Δημιουργία socket AF_INET
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// Έλεγχος αν το socket έχει δημιουργηθεί επιτυχώς
    if (sockfd < 0) 
       error("ERROR opening socket");

	// Αρχικοποίηση της διεύθυνσης του server
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(port);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

	// Έλεγχος αν η σύνδεση έγινε επιτυχώς
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

	// Αρχικοποίηση του socket για να δέχεται 5 συνδέσεις
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

	// Επαναληπτική διαδικασία για την εξυπηρέτηση των clients
    while (1) {
        int* newsockfd = malloc(sizeof(int));
		// Δέχομαι την σύνδεση
        *newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (*newsockfd < 0) 
            error("ERROR on accept");
		
		// Δημιουργία thread για την εξυπηρέτηση του client
		struct thread_args *args = malloc(sizeof(struct thread_args));
		args->newsockfd = *newsockfd;
		args->host = host;
        pthread_t thread_id;
		// Έλεγχος αν η δημιουργία του thread έγινε επιτυχώς
        if (pthread_create(&thread_id, NULL, handle_client, args) != 0) {
			error("ERROR creating thread");
        }
    }
	// Κλείνω την σύνδεση
    close(sockfd);
exit (0);
}
