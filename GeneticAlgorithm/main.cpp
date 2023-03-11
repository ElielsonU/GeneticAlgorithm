#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
using namespace std;

string getIndividualStr(int* individual, int genes_n)
{
	string str = "[ ";
	for (int i = 0; i < genes_n; i++)
	{
		str += individual[i] ? '1' : '0';
		str += ", ";
	}
	str += "]";
			
	return str;
}

int getScore(int *individual, int genes_n)
{
	int score = 0;

	for (int i = 0; i < genes_n; i++)
		if (individual[i])
			score++;

	return score;
}

void reproduce(int *father1, int* father2, int genes_n)
{
	int *son = new int[genes_n];
	int x = (rand() % genes_n);
	for (int i = 0; i <= x; i++)
		son[i] = father1[i];

	for (int i = x; i < genes_n; i++)
		son[i] = father2[i];

	if (getScore(son, genes_n) > getScore(father1, genes_n))
		for (int i = 0; i < genes_n; i++)
			father1[i] = son[i];
}

void mutate(int *individual, int genes_n)
{
	int x = (rand() % genes_n);
	individual[x] = !!!individual[x];
}

void printPopulation(int **table, int population_n, int genes_n)
{
	for (int i = 0; i < population_n; i++)
	{
		for (int j = 0; j < genes_n; j++)
			cout << table[i][j] << " ";
		cout << endl;
	}
}

int newGeneration(
	int** table,
	int population_n, 
	int genes_n, int changes_n, 
	double reproduction_chance, 
	double mutation_chance)
{
	for (int i = 0; i < changes_n; i++)
	{
		double random = ((double)(rand()%100)/100);
		if(random < reproduction_chance)
		{
			random = (rand()%population_n);
			int random2 = (rand() % population_n);
			while (random == random2)
				random2 = (rand() % population_n);

			reproduce(table[(int)random], table[random2], genes_n);
		}

		random = ((double)(rand() % 100) / 100);

		if (random < mutation_chance)
		{
			random = (rand() % population_n);
			mutate(table[(int)random], genes_n);
		}
	}

	int biggestScore = 0, max = 0;

	for (int j = 0; j < population_n; j++)
	{
		if (getScore(table[j], genes_n) > max)
		{
			max = getScore(table[j], genes_n);
			biggestScore = j;
		}
	}

	cout << "Biggest score: " << getScore(table[biggestScore], genes_n) << endl
		<< "Best individual: " << getIndividualStr(table[biggestScore], genes_n) << endl << endl;

	return getScore(table[biggestScore], genes_n);
}

int **startPopulation(int population_n, int genes_n)
{
	int **table;

	table = new int* [population_n];
	for (int i = 0; i < population_n; i++)
	{
		table[i] = new int[genes_n];
		for (int j = 0; j < genes_n; j++)
			table[i][j] = ((double)(rand()%10000)/100) < 10 ? 1 : 0;
	} 

	int biggestScore = 0, max = 0;

	for (int j = 0; j < population_n; j++)
	{
		if (getScore(table[j], genes_n) > max)
		{
			max = getScore(table[j], genes_n);
			biggestScore = j;
		}
	}

	return table;
}

int main()
{
	int timer = time(0);
	srand(timer);

	int generations = 300;
	int population_n = 50;
	int genes_n = 10;
	int changes_n = 30;
	double reproduction_chance = 0.7;
	double mutation_chance = 0.07;
	
	int** table = startPopulation(population_n, genes_n);

	for (int i = 0; i < generations; i++)
	{
		cout << "Generation " << (i + 1) << ": " << endl;
		if (newGeneration(table, population_n, genes_n, changes_n, reproduction_chance, mutation_chance) == genes_n)
			break;
	}

	cout << endl << "Time reached: " << (time(0) - timer) << endl;

	delete[] table;
}