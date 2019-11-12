#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int best_cost = 0, best_vector[52];

int search(int**, int, int, float, float, float, float);
void random_permutation(int*);
void initialise_pheromone_matrix(float**, float);
int cost(int*, int**);
int euc_2d(int*, int*);
void stepwise_const(int**, float, float, float, int*);

int main()
{
    int berlin52[52][2] = {
        {565, 575}, {25, 185}, {345, 750}, {945, 685}, {845, 655},
        {880, 660}, {25, 230}, {525, 1000}, {580, 1175}, {650, 1130}, {1605, 620},
        {1220, 580}, {1465, 200}, {1530, 5}, {845, 680}, {725, 370}, {145, 665},
        {415, 635}, {510, 875}, {560, 365}, {300, 465}, {520, 585}, {480, 415},
        {835, 625}, {975, 580}, {1215, 245}, {1320, 315}, {1250, 400}, {660, 180},
        {410, 250}, {420, 555}, {575, 665}, {1150, 1160}, {700, 580}, {685, 595},
        {685, 610}, {770, 610}, {795, 645}, {720, 635}, {760, 650}, {475, 960},
        {95, 260}, {875, 920}, {700, 500}, {555, 815}, {830, 485}, {1170, 65},
        {830, 610}, {605, 625}, {595, 360}, {1340, 725}, {1740, 245}
    };
    int max_it = 100, num_ants = 10;
    float decay = 0.1, c_heur = 2.5, c_local_phero = 0.1, c_greed = 0.9;

    srand(time(NULL));
    best_cost = seach(berlin52, max_it, num_ants, decay, c_heur, c_local_phero, c_greed);
    printf("Done. Best Solution: c = %d, v = ", best_cost);
    for (i = 0; i < 52, i++)
        printf("%d ", best_vector[i]);
}

int search(int **cities, int max_it, int num_ants, float decay, float c_heur, float c_local_phero, float c_greed)
{
    float init_pheromone, pheromone;
    int i, j, cand_cost, cand_vector[52];
    int pheromone[52][52];
    random_permutation(best_vector);
    best_cost = cost(best_vector, cities);
    init_pheromone = 1.0 / (52.0 * best_cost);
    initialise_pheromone_matrix(pheromone, init_pheromone);
    for (i = 0; i < max_it; i++)
    {
        for (j = 0; j < num_ants; j++)
        {
            stepwise_const(cities, pheromone, c_heur, c_greed, cand_vector);
            cand_cost = cost(cand_vector, cities);
            if (cand_cost < best_cost)
            {
                best_cost = cand_cost;
                for (k = 0; k < 52; k++)
                    best_vector[k] = cand_vector[a];
            }
            local_update_pheromone(pheromone, cand, c_local_phero, init_pheromone);
        }
        global_update_pheromone(pheromone, best, decay);
        printf(" > iteration %d, best = %d", i + 1, best_cost);
    }
}

void random_permutation(int *a)
{
    int i, j, temp;
    for (i = 0; i < 52; i++)
    {
        //generate a random number [0, n-1]
        j = rand() % (52 - i);
        j += i;

        //swap the last element with element at random index
        temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }
}

void initialise_pheromone_matrix(float **a, float init_pher)
{
    int i, j;
    for (i = 0; i < 52; i++)
        for (j = 0; j < 52; j++)
            a[i][j] = init_pher;
}

int cost(int *vector, int **cities)
{
    int distance = 0, i, city1, city2;
    for (i = 0; i < 52; i++)
    {
        city1 = vector[i];
        if (i == 51)
            city2 = vector[0];
        else
            city2 = vector[i + 1];
        distance += euc_2d(cities[city1], cities[city2]);
    }
    return distance;
}

int euc_2d(int *city1, int *city2)
{
    return (int) round(sqrt((city1[0] - city2[0]) * (city1[0] - city2[0]) + (city1[1] - city2[1]) * (city1[1] - city2[1])));
}

void stepwise_const(int**, float, float, float, int*)
{
    
}
