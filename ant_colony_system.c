#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "ant_colony_system.h"

Solution best;

int main()
{
    int berlin52[CITY_SIZE][2] =
    {
        {565, 575}, {25, 185},  {345, 750}, {945, 685},  {845, 655},
        {880, 660}, {25, 230},  {525, 1000},{580, 1175}, {650, 1130},{1605, 620},
        {1220, 580},{1465, 200},{1530, 5},  {845, 680},  {725, 370}, {145, 665},
        {415, 635}, {510, 875}, {560, 365}, {300, 465},  {520, 585}, {480, 415},
        {835, 625}, {975, 580}, {1215, 245},{1320, 315}, {1250, 400},{660, 180},
        {410, 250}, {420, 555}, {575, 665}, {1150, 1160},{700, 580}, {685, 595},
        {685, 610}, {770, 610}, {795, 645}, {720, 635},  {760, 650}, {475, 960},
        {95, 260},  {875, 920}, {700, 500}, {555, 815},  {830, 485}, {1170, 65},
        {830, 610}, {605, 625}, {595, 360}, {1340, 725}, {1740, 245}
    }, i, j;

    srand(time(NULL));
    search(berlin52, MAX_IT, NUM_ANTS, DECAY, C_HEUR, C_LOCAL_PHERO, C_GREED);
    printf("Done. Best Solution: c = %d, v = ", best.cost);
    for (i = 0; i < CITY_SIZE; i++)
        printf("%d ", best.vector[i]);
    printf("\n");
}

void search(int cities[CITY_SIZE][2], int max_it, int num_ants, float decay, float c_heur, float c_local_phero, float c_greed)
{
    int i, j;
    float init_pheromone, pheromone[CITY_SIZE][CITY_SIZE];
    random_permutation(best.vector);
    best.cost = cost(best.vector, cities);
    init_pheromone = 1.0 / ((float) CITY_SIZE * best.cost);
    initialise_pheromone_matrix(pheromone, init_pheromone);
    
    for (i = 0; i < MAX_IT; i++)
    {
        for (j = 0; j < NUM_ANTS; j++)
        {
            Solution cand;
            stepwise_const(cand.vector, cities, pheromone, c_heur, c_greed);
            cand.cost = cost(cand.vector, cities);
            if (cand.cost < best.cost)
            {
                best.cost = cand.cost;
                for (j = 0; j < CITY_SIZE; j++)
                    best.vector[j] = cand.vector[j];
            }
            local_update_pheromone(pheromone, cand, c_local_phero, init_pheromone);
        }
        global_update_pheromone(pheromone, best, decay);
        printf(" > iteration %d, best = %d\n", i + 1, best.cost);
    }
}

void random_permutation(int *r)
{
    int i, j, temp;
    for (i = 0; i < CITY_SIZE; i++)
        r[i] = i;
    
    
    for (i = CITY_SIZE - 1; i >= 0; --i)
    {
        //generate a random number [0, n - 1]
        j = rand() % (i + 1);

        //swap the last element with element at random index
        temp = r[i];
        r[i] = r[j];
        r[j] = temp;
    }
}

int cost(int *vector, int cities[CITY_SIZE][2])
{
    int distance = 0, i, city1, city2;
    for (i = 0; i < CITY_SIZE; i++)
    {
        city1 = vector[i];
        if (i == CITY_SIZE - 1)
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

void initialise_pheromone_matrix(float pheromone_matrix[CITY_SIZE][CITY_SIZE], float init_pheromone)
{
    int i, j;
    for (i = 0; i < CITY_SIZE; i++)
        for (j = 0; j < CITY_SIZE; j++)
            pheromone_matrix[i][j] = init_pheromone;
}

void stepwise_const(int *vector, int cities[CITY_SIZE][2], float pheromone[CITY_SIZE][CITY_SIZE], float c_heur, float c_greed)
{
    int i, greedy, next_city, size_of_choices;
    Probability choices[CITY_SIZE - 1];
    vector[0] = rand() % (CITY_SIZE - 1);
    
    for (i = 1; i < CITY_SIZE; i++)
    {
        size_of_choices = calculate_choices(i, choices, cities, vector[i - 1], vector, pheromone, C_HEUR, 1.0);
        greedy = (rand() / (double) RAND_MAX) <= C_GREED;
        if (greedy)
            next_city = greedy_select(choices, size_of_choices);
        else
            next_city = prob_select(choices, size_of_choices);
        vector[i] = next_city;
    }
}

int calculate_choices(int size_of_exclude, Probability *choices, int cities[CITY_SIZE][2], int last_city, int *exclude, float pheromone[CITY_SIZE][CITY_SIZE], float c_heur, float c_hist)
{
    int i, j, flag = 0, k = 0;
    for (i = 0; i < CITY_SIZE; i++)
    {
        for (j = 0; j < size_of_exclude; j++)
            if (i == exclude[j])
            {
                flag = 1;
                break;
            }
        if (flag)
        {
            flag = 0;
            continue;
        }
            
        /*choices = (Probability*) realloc(choices, (k + 1) * sizeof(Probability));*/
        choices[k].city = i;
        choices[k].history = pow(pheromone[last_city][i], c_hist);
        choices[k].distance = euc_2d(cities[last_city], cities[i]);
        choices[k].heuristic = pow((1.0 / choices[k].distance), c_heur);
        choices[k].prob = choices[k].history * choices[k].heuristic;
        k++;
    }
    return k;
}

int prob_select(Probability *choices, int size_of_choices)
{
    float sum = 0.0, v;
    int i;
    for (i = 0; i < size_of_choices; i++)
        sum += choices[i].prob;
    if (sum == 0.0)
        return choices[rand() % size_of_choices].city;
    v = (float) rand() / (float) (RAND_MAX) * 1;
    for (i = 0; i < size_of_choices; i++)
    {
        v -= choices[i].prob / sum;
        if (v <= 0.0)
            return choices[i].city;
    }
    return choices[size_of_choices - 1].city;
}

int greedy_select(Probability *choices, int size_of_choices)
{
    float max_prob = choices[0].prob;
    int i, max_prob_city = choices[0].city;
    for (i = 0; i < size_of_choices; i++)
        if (max_prob < choices[i].prob)
        {
            max_prob = choices[i].prob;
            max_prob_city = choices[i].city;
        }
    return max_prob_city;
}

void global_update_pheromone(float pheromone[CITY_SIZE][CITY_SIZE], Solution cand, float decay)
{
    int i, j;
    float value;
    for (i = 0; i < CITY_SIZE; i++)
    {
        if (i == CITY_SIZE - 1)
            j = cand.vector[0];
        else
            j = cand.vector[i + 1];
        value = ((1.0 - decay) * pheromone[cand.vector[i]][cand.vector[j]]) + (decay * (1.0 / cand.cost));
        pheromone[cand.vector[i]][cand.vector[j]] = value;
        pheromone[cand.vector[j]][cand.vector[i]] = value;
    }
}

void local_update_pheromone(float pheromone[CITY_SIZE][CITY_SIZE], Solution cand, float c_local_phero, float init_phero)
{
    int i, j;
    float value;
    for (i = 0; i < CITY_SIZE; i++)
    {
        if (i == CITY_SIZE - 1)
            j = cand.vector[0];
        else
            j = cand.vector[i + 1];
        value = ((1.0 - c_local_phero) * pheromone[cand.vector[i]][cand.vector[j]]) + (c_local_phero * init_phero);
        pheromone[cand.vector[i]][cand.vector[j]] = value;
        pheromone[cand.vector[j]][cand.vector[i]] = value;
    }
}
