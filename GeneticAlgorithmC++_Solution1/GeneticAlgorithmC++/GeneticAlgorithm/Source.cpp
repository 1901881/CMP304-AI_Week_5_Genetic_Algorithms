#include "openGA.hpp"

#include <iostream>

std::string valid_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,.|!#$?%^&*()=+?@ 1234567890";
std::string target_string = "To be, or not to be.";

struct Chromosome {
	std::string data;
};

struct Cost {
	float score;
};

typedef EA::Genetic<Chromosome, Cost> Genetic_Algorithm;
typedef EA::GenerationType<Chromosome, Cost> Generation_Type;


char generate_random_character(const std::function<double(void)> &rnd01)
{
	return valid_chars[(int)(valid_chars.size() * rnd01())];
}

void shakespeare_init_genes(Chromosome& s, const std::function<double(void)> &rnd01)
{
	for (int i = 0; i < target_string.size(); i++)
	{
		s.data += generate_random_character(rnd01);
	}
}

double calculate_total_fitness(const Genetic_Algorithm::thisChromosomeType & X)
{
	return X.middle_costs.score;
}

bool calculateCost(const Chromosome& individual, Cost& cost)
{
	// Create a score value that starts at 0
	// Iterate through the indivdual string and check for a match
	// For each mismatching pair, add the the score value
	// Assign the score value to the Cost objects cost value and return true

	float score = 0;

	for (int i = 0; i < individual.data.size(); ++i)
	{
		if (individual.data[i] != target_string[i])
		{
			++score;
		}
	}

	cost.score = score;

	return true;
}

Chromosome mutationFunc(const Chromosome& xBase, const std::function<double(void)>& rnd01, double shrinkScale)
{
	// Create a new solution
	// Select a random char in the string
	// Replace it with a rand char
	// Return new solution

	
	Chromosome newChromosome = xBase;
	if (!xBase.data.empty())
	{
		int randIndex = (int)(rnd01() * newChromosome.data.size());		// This returns an error due to divide by zero as they have no size

		newChromosome.data[randIndex] = generate_random_character(rnd01);
	}
	return newChromosome;
}

Chromosome crossoverFunc(const Chromosome& X1, const Chromosome& X2, const std::function<double(void)>& rnd01)
{
	// Create new Chromosome
	// Iterate through the string of one parent
	// For each char, randomly add either X1's value or X2's value
	// Return new Chromosome

	Chromosome newChromosome;

	//newChromosome.data += X1.data.substr(0, X1.data.size() / 2);
	//newChromosome.data += X2.data.substr(X2.data.size() / 2, X2.data.size() / 2);

	for (int i = 0; i < X1.data.size(); ++i)
	{
		if (rand() % 2 == 0)
		{
			newChromosome.data += X1.data[i];
		}
		else
		{
			newChromosome.data += X2.data[i];
		}
	}

	return newChromosome;
}

void reportGeneration(int genNumber, const Generation_Type& lastGen, const Chromosome& bestGenes)
{
	system("cls");

	std::cout << "Generation Number [ " << genNumber << " ]" << std::endl;
	std::cout << "Best Gene [ " << bestGenes.data << " ]" << std::endl << std::endl;

	std::cout << "Generation: " << std::endl;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			std::cout << lastGen.chromosomes[(i * 5) + j].genes.data << "\t";
		}
		std::cout << std::endl;
	}
}

int main()
{
	EA::Chronometer timer;
	timer.tic();

	Genetic_Algorithm ga_obj;
	ga_obj.problem_mode = EA::GA_MODE::SOGA;	// State the Genetic Algorithm is aiming for a single objective.
	ga_obj.population = 50;
	ga_obj.generation_max = 1000;				// We want this to keep attempting for a long time.
	ga_obj.init_genes = shakespeare_init_genes;
	ga_obj.calculate_SO_total_fitness = calculate_total_fitness;
	ga_obj.eval_solution = calculateCost;
	ga_obj.mutate = mutationFunc;
	ga_obj.crossover = crossoverFunc;
	ga_obj.crossover_fraction = 1;
	ga_obj.mutation_rate = 0.4;
	ga_obj.best_stall_max = 1000;
	ga_obj.average_stall_max = 1000;
	ga_obj.SO_report_generation = reportGeneration;
	ga_obj.solve();



	std::cout << "The problem is optimized in " << timer.toc() << " seconds." << std::endl;

	std::cin.get();
}