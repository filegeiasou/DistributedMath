#include "stats.h"
#include <stdio.h>
#include <stdlib.h>

// Συνάρτηση για τον υπολογισμό του μέσου όρου
double *
average_1_svc(input1 *argp, struct svc_req *rqstp)
{
	// Δήλωση μεταβλητών
	static double  result;
	double sum=0;
	// Υπολογίζω το άθροισμα των στοιχείων του διανύσματος
	for(int i = 0; i < argp->y.y_len; i++) {
		sum += argp->y.y_val[i];
	}
	// Υπολογίζω το μέσο όρο
	result = sum / argp->y.y_len;
	// Επιστρέφω το αποτέλεσμα
	return &result;
}

// Συνάρτηση για τον υπολογισμό του ελάχιστου και μέγιστου
output1 *
min_max1_1_svc(input1 *argp, struct svc_req *rqstp)
{
	// Δήλωση μεταβλητών
	static output1  result;
	// Αρχικοποίηση των μεταβλητών
	result.min_max[0] = argp->y.y_val[0];
	result.min_max[1] = argp->y.y_val[0];
	// Υπολογιζω το ελάχιστο και το μέγιστο
	for(int i=0 ;i<argp->y.y_len; i++)
	{
		if(argp->y.y_val[i] < result.min_max[0])
		{
			result.min_max[0] = argp->y.y_val[i];
		}
		if(argp->y.y_val[i] > result.min_max[1])
		{
			result.min_max[1]= argp->y.y_val[i];
		}
	}

	// Επιστρέφω το αποτέλεσμα
	return &result;
}

output2 *
prod_ay_1_svc(input2 *argp, struct svc_req *rqstp)
{
	// Δήλωση μεταβλητών
	static output2  result;
	// Δεσμεύω δυναμικά μνήμη για το αποτέλεσμα
	result.pr_aY.pr_aY_len = argp->y.y_len;
	result.pr_aY.pr_aY_val = (double *)malloc(argp->y.y_len * sizeof(double));
	// Έλεγχος αν η δέσμευση μνήμης έγινε επιτυχώς
	if (result.pr_aY.pr_aY_val == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	// Υπολογίζω το a*Y
	for(int i=0; i<argp->y.y_len; i++)
	{
		result.pr_aY.pr_aY_val[i] = argp->a * argp->y.y_val[i];
	}
	// Επιστρέφω το αποτέλεσμα
	return &result;
}
