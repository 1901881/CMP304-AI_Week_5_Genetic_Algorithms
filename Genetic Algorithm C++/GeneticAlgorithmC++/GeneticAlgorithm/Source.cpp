#include "openGA.hpp"
#include <iostream>
using namespace std;

struct Chromosome
{
	string a;
};

struct Cost
{
	float score;
};

typedef EA::Genetic<Chromosome, Cost> Genetic_Algorithm;
typedef EA::GenerationType<Chromosome, Cost> Generation_Type;

std::string valid_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,.|!#$£%^&*()=+?@ 1234567890";
std::string target_string = "To be, or not to be.";


char generate_random_character(const std::function<double(void)>& rnd01)
{
	//cout << "Random Charater: " << valid_chars[(int)(valid_chars.size() * rnd01())] << endl;
	return valid_chars[(int)(valid_chars.size() * rnd01())];
}

void shakespeare_init_genes(Chromosome& s, const std::function<double(void)>& rnd01)
{
	for (int i = 0; i < target_string.size(); i++)
	{
		s.a += generate_random_character(rnd01);
	}
}

double calculate_total_fitness(const Genetic_Algorithm::thisChromosomeType& X)
{
	return X.middle_costs.score;
}

bool calculate_cost(const Chromosome& individual, Cost& cost)
{
	// Create a score value that starts at (int, float, double, ...) 
	// Iterate through the individuals string and check if the match 
	// For each mismatching pair, add to the score value 
	// Assign the score value to the Cost objects cost value and return true
	int score = 0;

	for (int i = 0; i < individual.a.size(); i++)
	{
		if (individual.a[i] != target_string[i])
		{
			score++;
		}
	}

	cost.score = score;
	return true;
}

Chromosome mutation_function(const Chromosome& X_base, const 
	std::function<double(void)> &rnd01, double shrink_scale)
{
	//cout << "X_base = " <<  X_base.a << endl;
	// Create a new solution 
	// Select a random character in the string 
	// Replace it with a random character 
	// Return new solutuin
	Chromosome newChromosome = X_base;
	//string newChromosomeString = newChromosome.a;
	//int stringSize = newChromosomeString.size();
	int random_index = rand() % X_base.a.size();
	newChromosome.a[random_index] = generate_random_character(rnd01);
	//newChromosome.a = newChromosomeString;
	//cout << "Mutation: " << newChromosome.a << endl;
	return newChromosome;
}
	

Chromosome crossover_function(const Chromosome& X1, const Chromosome& X2, const
	std::function<double(void) >& rnd01)
{
	// Create new chromosome 
	// Iterate through the string of one parent 
	// For each character, randomly add either Xl's value or X2's value 
	// Return new chromosome
	Chromosome newChromosome;
	newChromosome.a = "";
	for (int i = 0; i < X1.a.size(); i++)
	{
		int random_number = rand() % 100;
		//cout << "Random Number" << random_number << endl;
		if (random_number >= 50)
		{
			newChromosome.a += X1.a[i];
		}
		else
		{
			newChromosome.a += X2.a[i];
		}
	}
	//cout << "Crossover: " << newChromosome.a << endl;
	return newChromosome;
}

void report_generation(int generation_number, const Generation_Type& last_generation,
	const Chromosome& best_genes)
{
	std::cout << "Generation [" << generation_number << "]," << std::endl;
	std::cout << "The best total cost is: " << last_generation.best_total_cost << std::endl;
	std::cout << "The best average cost is: " << last_generation.average_cost << std::endl;
	std::cout << "The best genes is: " << best_genes.a << std::endl;
	std::cout << "The best execution time is: " << last_generation.exe_time << std::endl;
}


int main()
{
	EA::Chronometer timer;
	timer.tic();

	Genetic_Algorithm ga_obj;
	ga_obj.problem_mode = EA::GA_MODE::SOGA;	// State the Genetic Algorithm is aiming for a single objective.
	ga_obj.population = 200;
	ga_obj.generation_max = 1000;				// We want this to keep attempting for a long time.
	ga_obj.init_genes = shakespeare_init_genes;
	ga_obj.calculate_SO_total_fitness = calculate_total_fitness;
	ga_obj.eval_solution = calculate_cost;
	ga_obj.mutate = mutation_function;
	ga_obj.crossover = crossover_function;
	ga_obj.crossover_fraction = 1;
	ga_obj.mutation_rate = 0.05;
	ga_obj.best_stall_max = 1000;
	ga_obj.average_stall_max = 1000;
	ga_obj.SO_report_generation = report_generation;//user added
	EA::StopReason reason = ga_obj.solve();

	std::cout << "The problem is optimized in " << timer.toc() << " seconds." << std::endl;
	

	std::cin.get();
}