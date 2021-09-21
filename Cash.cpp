#include <iostream>
#include <list>
#include <unordered_map>
#include <stdlib.h>
#include <assert.h>
#include "Cash.h"


int main ()
{
	size_t cache_size, n_elements = 0;
	std::cin >> cache_size >> n_elements;

	cache_t<page_t> new_cache {cache_size};

	int* page_ids = (int*) calloc (n_elements, sizeof(int));
	assert (page_ids);

	int hits = 0;

	for (size_t i = 0; i < n_elements; ++i) 
	{
		std::cin >> page_ids[i];

		assert(std::cin.good ());

		if (new_cache.lookup_update (page_ids[i], slow_get_page)) hits += 1;

		new_cache.dump ();
	}

	cache_t<page_t> ideal_cache {cache_size};

	std::cout << "total hits: " << hits << "\r\n";
	std::cout << "ideal hits: " << ideal_cache.vanga_cache (page_ids, n_elements, slow_get_page) << "\r\n";
}
