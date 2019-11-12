#ifndef __ANT_COLONY_SYSTEM_H__
#define __ANT_COLONY_SYSTEM_H__

#define MAX_IT 100
#define NUM_ANTS 10
#define CITY_SIZE 52
#define DECAY 0.1
#define C_HEUR 2.5
#define C_LOCAL_PHERO 0.1
#define C_GREED 0.9

typedef struct
{
    int vector[CITY_SIZE], cost;
} Solution;

typedef struct
{
    int city, distance;
    float history, heuristic, prob;
} Probability;

void search(int[][2], int, int, float, float, float, float);
int euc_2d(int*, int*);
int cost(int*, int[][2]);
void random_permutation(int*);
void initialise_pheromone_matrix(float[][CITY_SIZE], float);
int calculate_choices(int, Probability*, int[][2], int, int*, float[][CITY_SIZE], float, float);
int prob_select(Probability*, int);
int greedy_select(Probability*, int);
void stepwise_const(int*, int[][2], float[][CITY_SIZE], float, float);
void global_update_pheromone(float[][CITY_SIZE], Solution, float);
void local_update_pheromone(float[][CITY_SIZE], Solution, float, float);

#endif
