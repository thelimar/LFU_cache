#include <iostream>
#include <list>
#include <unordered_map>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include "Cash.h"

enum errors
{
	FAILED_TEST      = 1,
	WRONG_ARG_AMOUNT = 2,
};

bool e2e_test (std::string input_file, std::string output_output);


int main (int argc, char** argv)
{
	if (argc == 1)
	{
		size_t cache_size, n_elements = 0;
		std::cin >> cache_size >> n_elements;

		cache_t<page_t> new_cache{ cache_size };

		int* page_ids = (int*) calloc (n_elements, sizeof(int));
		assert (page_ids);

		int hits = 0;

		for (size_t i = 0; i < n_elements; ++i)
		{
			std::cin >> page_ids[i];

			assert (std::cin.good ());

			if (new_cache.lookup_update (page_ids[i], slow_get_page)) hits += 1;
		}

		cache_t<page_t> ideal_cache{ cache_size };

		std::cout << "total hits: " << hits << "\r\n";
		std::cout << "ideal hits: " << ideal_cache.vanga_cache (page_ids, n_elements, slow_get_page) << "\r\n";

		free (page_ids);
		return 0;
	}
	else if (argc == 3)
	{
		if (!e2e_test (std::string (argv[1]), std::string (argv[2])))
		{
			std::cout << "test " << std::string (argv[1]) << "did not pass\r\n";
			return FAILED_TEST;
		}
		else
			return 0;
	}
	else
	{	
		std::cout << "please, launch program with no args, or with two\r\n";
		return WRONG_ARG_AMOUNT;
	}
}


bool e2e_test (std::string input_file, std::string output_file)
{
	std::ifstream input (input_file);

	if (input.is_open ())
	{
		size_t cache_size, n_elements = 0;
		input >> cache_size >> n_elements;

		cache_t<page_t> new_cache{ cache_size };

		int* page_ids = (int*) calloc (n_elements, sizeof(int));
		assert (page_ids);

		int hits = 0;

		for (size_t i = 0; i < n_elements; ++i)
		{
			input >> page_ids[i];

			assert(std::cin.good ());

			if (new_cache.lookup_update (page_ids[i], slow_get_page)) hits += 1;
		}

		cache_t<page_t> ideal_cache { cache_size };
		int ideal_hits = ideal_cache.vanga_cache (page_ids, n_elements, slow_get_page);

		std::ifstream output(output_file);

		if (output.is_open ())
		{
			size_t answer_hits       = 0;
			int    answer_ideal_hits = 0;

			output >> answer_hits >> answer_ideal_hits;

			if (answer_hits == hits)
			{
				if (answer_ideal_hits == ideal_hits)
					return 1;
				else
				{
					std::cout << "Wrong ideal hits ammount.\r\n Answer is " << answer_ideal_hits << "\r\nCounted is " << ideal_hits;
					return 0;
				}
			}
			else
			{
				std::cout << "Wrong hits ammount.\r\n Answer is " << answer_hits << "\r\nCounted is " << hits;
				return 0;
			}
		}

		else
		{
			perror("ERROR OCCURED WHILE OPENING FILE\r\n");
			return 0;
		}
	}
	else
	{
		perror ("ERROR OCCURED WHILE OPENING FILE\r\n");
		return 0;
	}
}