#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//--------------------- Global variables ----------------------------//
//nb of machines
#define nb 5
//explore
static float e = 0.4;
//alpha (learning rate)
static float a = 0.5;
//value for each machine
static double values[nb] = {0.};
//prob for each machine
static double prob[nb] = {0.};

//---------------------- Side functions -----------------------------//
//generate bounded random doubles
double randd(double x0, double x1) {
    return x0 + (x1 - x0) * rand() / ((double) RAND_MAX);
}

//generate bounded random ints
int randint(int x0, int x1) {
    return rand() % x1 + x0;
}

void init_prob_array() {
    for (int i = 0; i < nb; i++) {
        prob[i] = randd(0, 1);
    }
}

//get index for the max value
int argmax(double array[]) {
    int n = 0;
    for (int i = 0; i < nb; i++) {
        if (array[i] > array[n]) {
             n = i;
        }
    }
    return n;
}

void print_array(double array[]) {
    for (int i = 0; i < nb; i++) {
        printf("%d = %f \n", i , array[i]);
    }
}

void print_score(int maxprob, int maxvalue){
    printf("------------------\n");
    printf("VALUES            \n");
    printf("------------------\n");
    print_array(values);
    printf("------------------\n");
    printf("PROBABILITIES     \n");
    printf("------------------\n");
    print_array(prob);
    printf("------------------\n");
    printf("RESULTS           \n");
    printf("------------------\n");
    printf("max prob: %d \nmax value: %d\n", maxprob, maxvalue);

}

//------------------------ Main functions -----------------------------//
int make_choice() {
    int n;
    if (randd(0, 1) < e) {
        n = randint(0, nb);
    }
    else {
        n = argmax(values);
    }
    return n;
}

int play(int n) {
    int reward = 0;
    if (randd(0, 1) < prob[n]) {
        reward = 1;
    }
    return reward;
}

void learn(int n, int reward) {
    values[n] += a * (reward - values[n]);
}

int main() {

    int tmax = 100000;
    int n, reward;

    // set random seed
    srand(time(NULL));

    //init arrays containing
    //probs for each machines
    init_prob_array();

    for (int i = 0; i < tmax; i++) {
        n = make_choice();
        reward = play(n);
        learn(n, reward);
    }

    print_score(argmax(prob), argmax(values));
    return 0;
}
