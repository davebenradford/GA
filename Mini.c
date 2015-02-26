#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// GLOBAL CONSTANTS
#define NUMBER_ROUTES 10
#define NUMBER_CITIES 10
#define MAXIMUM_GENERATIONS 1000
#define MINIMUM_FITNESS 10
#define MUTATION_RATE 0.025
#define TRUE 1
#define FALSE 0

// GLOBAL VARIABLES
int generations = 1, a, b, eq, mutateTotal, elitism;
char currentRoutes[NUMBER_ROUTES][NUMBER_CITIES + 1];
char nextRoutes[NUMBER_ROUTES][NUMBER_CITIES + 1];
double routesFunctionFitnesses[NUMBER_ROUTES], routesEvalFitnesses[NUMBER_ROUTES];
double averageFitness, maxFuncFitness, maxEvalFitness, minFuncFitness, minEvalFitness;
double nMaximum = pow(2.0, NUMBER_CITIES), nRange = ;

// Parabola Function
//
// f(x) = x^2
double setFitness(int n) {
	return (double) pow(n, 2.0);
}

void initializeRoutes() {
	int x, y, z;
	double n;
	char candidate[NUMBER_CITIES + 1];
	for(x = 0; x < NUMBER_ROUTES; x++) {
		n = rand() % (int) nMaximum;
		if(n >= pow(2.0, NUMBER_CITIES)) {
			n -= 128;
			printf("%0.0f\n", n);
			getchar();
		}
		routesFunctionFitnesses[x] = setFitness(n);
		z = 0;
		for(y = NUMBER_CITIES - 1; y >= 0; y--) {
			if(n >= pow(2, y)) {
				n -= pow(2, y);
				candidate[z] = '1';
			}
			else {
				candidate[z] = '0';
			}
			z++;
		}
		candidate[z] = '\0';
		for(y = 0; y < (NUMBER_CITIES + 1); y++ ) {
			currentRoutes[x][y] = candidate[y];
		}
	}
	//printf("Initialized");
	//printf("\n");
}

int evaluateRoutes() {
	int route, city, currentRouteFitness;
	//long double sum;
	// FIND FITNESS METRICS: AVG, MIN (Best), MAX (Worst)
	for(route = 0; route < NUMBER_ROUTES; route++) {
		currentRouteFitness = 0;
		for(city = 0; city < NUMBER_CITIES; city++) {
			if(currentRoutes[route][city] == '0') {
				currentRouteFitness++;
				//printf("%d\n", currentRouteFitness);
			}
		}
		routesEvalFitnesses[route] = currentRouteFitness;
		//printf("%f\n", routesEvalFitnesses[route]);
		if(currentRouteFitness == MINIMUM_FITNESS) {
			//printf("MINIMUM FOUND\n");
			minEvalFitness = routesEvalFitnesses[route];
			minFuncFitness = routesFunctionFitnesses[route];
			return 1;
		}
		else {
			if(generations == 1 && route == 0) {
				minEvalFitness = routesEvalFitnesses[route];
				minFuncFitness = routesFunctionFitnesses[route];
				maxEvalFitness = routesEvalFitnesses[route];
				maxFuncFitness = routesFunctionFitnesses[route];
			}
			if(routesEvalFitnesses[route] > minEvalFitness) {
				minEvalFitness = routesEvalFitnesses[route];
				minFuncFitness = routesFunctionFitnesses[route];
			}
			if(routesEvalFitnesses[route] < maxEvalFitness) {
				maxEvalFitness = routesEvalFitnesses[route];
				maxFuncFitness = routesFunctionFitnesses[route];
			}
		}		
	}
	//if(count) {
	//printf("---\n");
	//printf("GENERATION %d\n", generations);
	//printf("---\n");
	//printf("MAXIMUM EVALUATION FITNESS: %0.4f\n", maxEvalFitness);
	//printf("MINIMUM EVALUATION FITNESS: %0.4f\n", minEvalFitness);
	//printf("---\n");
	//printf("MAXIMUM FUNCTION FITNESS: %0.4f\n", maxFuncFitness);
	//printf("MINIMUM FUNCTION FITNESS: %0.4f\n", minFuncFitness);
	//printf("---\n");
	//}
	//printf("Evaluation Unsuccessful\n");
    return 0;
}

int selectParent() {
	int candidateA, candidateB, /*x,*/ fitnessA, fitnessB;
	//if(elitism < 5) {
		//printf("ELITE\n");
	//	for(x = 0; x < NUMBER_ROUTES; x++) {
	//		if(minEvalFitness == routesEvalFitnesses[x]) {
	//			candidateA = x;
				//printf("CAND A%d\n", candidateA);
	//		}
	//	}
	//	elitism++;
	//}
	//else {
		//printf("NON-ELITE\n");
	candidateA = (int) rand() % NUMBER_ROUTES;
	//}
	candidateB = (int) rand() % NUMBER_ROUTES;
	//printf("CAND B%d\n", candidateB);
	fitnessA = routesFunctionFitnesses[candidateA];
	fitnessB = routesFunctionFitnesses[candidateB];
	//printf("FIT A%d\n", fitnessA);
	//printf("FIT B%d\n", fitnessB);
	if(fitnessB == fitnessA) {
		eq++;
		//printf("EQ TURN\n");
		return candidateB;
	}
	else if(fitnessB < fitnessA) {
		b++;
		//printf("BE TURN\n");
		return candidateB;
	}
	else {
		a++;
		//printf("A TURN\n");
		return candidateA;
	}
}

void generateNextRoutes() {
	int x, y, z, n, children, p1Candidate, p2Candidate, crossover;
	double mutation;
	for(children = 0; children < NUMBER_ROUTES; children++) {
		char parent1[NUMBER_CITIES + 1], parent2[NUMBER_CITIES + 1], child1[NUMBER_CITIES + 1], child2[NUMBER_CITIES + 1], temp;
		// Select and Populate Parent1 and Initialize Child1
		p1Candidate = selectParent();
		for(x = 0; x < NUMBER_CITIES; x++) {
			parent1[x] = currentRoutes[p1Candidate][x];
			child1[x] = parent1[x];
		}
		// Select and Populate Parent2 and Initialize Child2
		p2Candidate = selectParent();
		for(x = 0; x < NUMBER_CITIES; x++) {
			parent2[x] = currentRoutes[p2Candidate][x];
			child2[x] = parent2[x];
		}
		crossover = rand() %  (int) NUMBER_CITIES;
		for(x = 0; x < NUMBER_CITIES; x++) {
			if(x <= crossover) /* || x == (crossover + 1) || x == (crossover + 2))*/ {
				temp = child1[x];
				child1[x] = child2[x];
				child2[x] = temp;
			}
		}
		// MUTATE CHILDREN IF SELECTED
		mutation = rand() % (int) (1.0 / MUTATION_RATE);
		if(mutation == 0) {
			crossover = rand() % (int) NUMBER_CITIES;
			if(child1[crossover] == '1') {
				child1[crossover] = '0';
			}
			else if(child1[crossover] == '0'){
				child1[crossover] = '1';
			}
			mutateTotal++;
		}
		mutation = rand() % (int) (1.0 / MUTATION_RATE);
		if(mutation == 0) {
			crossover = rand() % (NUMBER_CITIES - 1);
			if(child2[crossover] == '1') {
				child2[crossover] = '0';
			}
			else if(child2[crossover] == '0'){
				child2[crossover] = '0';	
			}
			mutateTotal++;
		}
		for(x = 0; x < NUMBER_CITIES; x++) {
			nextRoutes[children * 4][x] = child1[x];
			nextRoutes[children * 4 + 1][x] = child2[x];
		}
		n = rand() % (int) pow(2.0, NUMBER_CITIES);
		for(y = NUMBER_CITIES - 1; y >= 0; y--) {
			if(n >= pow(2, y)) {
				n -= pow(2, y);
				nextRoutes[children * 4 + 2][z] = '1';
			}
			else {
				nextRoutes[children * 4 + 2][z] =  '0';
			}
			z++;
		}
		nextRoutes[children * 4 + 2][z] = '\0';
		z = 0;
		n = rand() % (int) pow(2.0, NUMBER_CITIES);
		for(y = NUMBER_CITIES - 1; y >= 0; y--) {
			if(n >= pow(2, y)) {
				n -= pow(2, y);
				nextRoutes[children * 4 + 3][z] = '1';
			}
			else {
				nextRoutes[children * 4 + 3][z] =  '0';
			}
			z++;
		}
		nextRoutes[children * 4 + 3][z] = '\0';
		z = 0;
	}
	for(x = 0; x < NUMBER_ROUTES; x++) {
		n = 0;
		for(y = 0; y < NUMBER_CITIES; y++) {
			if(nextRoutes[x][y] == '1') {
				n += pow (2, (NUMBER_CITIES - 1) - y);
				//printf("%d\n", n);
			}
			currentRoutes[x][y] = nextRoutes[x][y];
		}
		routesFunctionFitnesses[x] = setFitness(n);
	}
}

int main() {
    clock_t begin, end;
    double time_spent, mutateRate;
    int seed, stop = 0, candidateTotal;
    begin = clock();
    seed = time(NULL);
	srand(seed);
	initializeRoutes();
	while(generations <= MAXIMUM_GENERATIONS && stop == 0) {
		stop = evaluateRoutes();
		if(stop == 1) {
			printf("MINIMUM FOUND!\n");
		}
		else {
			//printf("NEXT GENS\n");
			generateNextRoutes();
			if(generations < MAXIMUM_GENERATIONS && (generations % 20 == 0 || generations == 1)) {
				printf("Generation running: %d\n", generations);
				printf("%0.4f\n", minEvalFitness);
			}
			//	generations++;
			//	elitism = 0;
			//}
			if(generations == MAXIMUM_GENERATIONS){
				printf("MAXIMUM GENERATIONS REACHED\n");
				generations++;
				//elitism = 0;
			}
			else {
				generations++;
				//elitism = 0;
			}
		}
	}
	printf("---\n");
	printf("MAXIMUM EVALUATION FITNESS: %0.4f\n", maxEvalFitness);
	printf("MINIMUM EVALUATION FITNESS: %0.4f\n", minEvalFitness);
	printf("---\n");
	printf("MAXIMUM FUNCTION FITNESS: %0.4f\n", maxFuncFitness);
	printf("MINIMUM FUNCTION FITNESS: %0.4f\n", minFuncFitness);
	printf("---\n");
	printf("A COUNT: %d\n", a);
	printf("B COUNT: %d\n", b);
	printf("EQUAL COUNT: %d\n", eq);
	printf("---\n");
	candidateTotal = a + b + eq;
	printf("CANDIDATES SELECTED: %d\n", candidateTotal);
	printf("---\n");
	printf("MUTATION COUNT: %d\n", mutateTotal);
	mutateRate = (double) mutateTotal / candidateTotal;
	printf("MUTATION RATE: %0.4f\n", mutateRate);
	printf("---\n");
	printf("Generations Evaluated: %d\n", generations - 1);
    end = clock();
    time_spent =  (double) (end - begin) / CLOCKS_PER_SEC;
    printf("Runtime: %0.4fs\n", time_spent);
    printf("---\n");
	return 0;
}
