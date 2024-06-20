// Δήλωση βιβλιοθηκών 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

// Συνάρτηση για την εμφάνιση μηνύματος λάθους
void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    // Δήλωση μεταβλητών
    int sockfd, portno, n, t, done;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char str[100];

    // Έλεγχος αν έχουν δοθεί τα απαραίτητα ορίσματα
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    // Δημιουργία socket AF_INET
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Έλεγχος αν το socket έχει δημιουργηθεί επιτυχώς
    if (sockfd < 0) 
        error("ERROR opening socket");

    // Εύρεση του server με βάση το hostname
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    // Αρχικοποίηση της διεύθυνσης του server
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    printf("Trying to connect...\n");
    // Σύνδεση με τον server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        error("ERROR connecting");
    }
    printf("Connected.\n");
    int epil;

    // Κώδικας για την επιλογή της λειτουργίας
    do {
        printf("Give selection\n1)average 2)min and max 3)product a*y 4)Exit: ");
        scanf("%d",&epil);
        // Κώδικας για την είσοδο του πραγματικού αριθμού a
        if(epil==4)
        {
            // Κλείσιμο της σύνδεσης στέλνοντας την επιλογή
            send(sockfd, &epil, sizeof(epil), 0);
            break;
        }
        // Αποστολή της επιλογής στον server
        send(sockfd, &epil, sizeof(epil), 0);
        if (epil ==3 )
        {
            double a;
            // Διαβάζω τον πραγματικό αριθμό και τον στέλνω στον server
            printf("Enter a real number: ");
            scanf("%lf", &a);
            send(sockfd, &a, sizeof(a), 0);
        }

        // Διαβάζω το μήκος του διανύσματος και τα στοιχεία του

        int n;
        printf("Enter the length of the vector: ");
        scanf("%d", &n);

        int* Y = (int *) malloc(n * sizeof(int));
        if(Y == NULL)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }
        printf("Enter the elements of the vector: ");
        for (int i = 0; i < n; i++) {
            scanf("%d", &Y[i]);
        }

        // Στέλνω n , και του Y στον server
        send(sockfd, &n, sizeof(n), 0);
        for(int i=0; i<n; i++)
        {
            send(sockfd, &Y[i], sizeof(int), 0);
        }

        // Κώδικας για την λήψη των αποτελεσμάτων από τον server

        if (epil ==1 )
        {
            double mean;
            // Λαμβάνω το μέσο όρο του Y
            recv(sockfd, &mean, sizeof(mean), 0);
            printf("Mean of Y: %f\n", mean);
        }
        if(epil == 2)
        {
            int min;
            int max;
            // Λαμβάνω το min και το max του Y
            recv(sockfd, &min, sizeof(min), 0);
            recv(sockfd, &max, sizeof(max), 0);
            printf("Min of Y: %d, Max of Y: %d\n", min, max);
        }
        if(epil == 3)
        {
            double* aY = (double *) malloc(n * sizeof(double));
            printf("a*Y: ");
            // Λαμβάνω το a*Y
            for (int i = 0; i < n; i++) {
                recv(sockfd, &aY[i], sizeof(double), 0);
                printf("%f ", aY[i]);
            }
            printf("\n");
            free(aY);
        }
        // Αποδεσμεύω την μνήμη που δέσμευσα	
        free(Y);

    } while (epil!=4);

    // Κλείνω το socket
    close(sockfd);

    return 0;
}