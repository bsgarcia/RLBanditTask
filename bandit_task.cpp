#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>

// Uncomment this to use std functions without the std:: prefix
//using namespace std;

// -------------------------------------------------------------------------- // 

namespace utils {
    // generate random doubles
    double randd(double x0, double x1) {
        return x0 + (x1 - x0) * rand() / ((double) RAND_MAX);
    }
    
    // generate random int between bounds
    int randint(int x0, int x1) {
        return rand() % x1 + x0;
    }

    // get index for the max value
    int argmax(std::vector<double> array) {
        int n = 0;
        for (int i = 0; i < array.size(); i++) {
            if (array[i] > array[n]) {
                n = i;
            }
        }
        return n;
    }

    // print array
    void print_array(std::vector<double> array) {
        for (int i = 0; i < array.size();  i++) {
            std::cout << i << " = " << array[i] << "\n";
        }
    }
}

// -------------------------------------------------------------------------- // 

class Agent {

    public:

        int nb;
        float e;
        float a;

        // Value associated to each machine
        std::vector<double> values = std::vector<double>(nb);

        Agent(int nb, float e, float a) : nb(nb), e(e), a(a) {}
        
        // "this" prefixes are not mandatory but
        // help finding actual class' members
        int make_choice() {
            int n;
            if (utils::randd(0, 1) < this->e) {
                n = utils::randint(0, this->nb);
            }
            else {
                n = utils::argmax(this->values);
            }
            return n;
        }

        int play(int n, double p) {
            int reward = 0;
            if (utils::randd(0, 1) < p) {
                reward = 1;
            }
            return reward;
        }

        void learn(int n, int reward) {
            this->values[n] += this->a * (reward - this->values[n]);
        }

};

// -------------------------------------------------------------------------- // 

class Casino {
    
    public:
        // n machine
        int nb;
        
        // probability associated to each machine
        std::vector<double> prob = std::vector<double>(nb);

        Casino(int nb) : nb(nb) {
            init_prob_machines(); 
        }
        
    private:

        void init_prob_machines() {
            for (int i = 0; i < nb; i++) {
                prob[i] = utils::randd(0, 1);
            }
        }

};

// -------------------------------------------------------------------------- // 

int main () {

    //set random seed
    srand(time(NULL));
    
    // n machine in the casino
    int n_machine = 8;
    
    // exploration rate
    float e = 0.3;

    // learning rate
    float a = 0.4;

    // trials
    int tmax = 9999999;
    
    // Instantiation
    Agent agent {n_machine, e, a}; 
    Casino casino {n_machine};
    
    // Train agent to guess the machine 
    // with highest probability reward
    int n, reward;

    std::cout << "Training agent...\n";

    for (int i = 0; i < tmax; i++) {
        n = agent.make_choice();
        reward = agent.play(n, casino.prob[n]);
        agent.learn(n, reward);
    }
    
    std::cout << "------------------\n";
    std::cout << "VALUES            \n";
    std::cout << "------------------\n";
    utils::print_array(agent.values); 
    std::cout << "------------------\n";
    std::cout << "PROBABILITIES     \n";
    std::cout << "------------------\n";
    utils::print_array(casino.prob);
    std::cout << "------------------\n";
    std::cout << "RESULTS           \n";
    std::cout << "------------------\n";
    std::cout << "max prob: " << utils::argmax(casino.prob) << "\nmax value: " << utils::argmax(agent.values) << "\n";

    return 0;
}
