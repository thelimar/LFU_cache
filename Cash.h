#pragma once

#include <iostream>
#include <list>
#include <unordered_map>
#include <stdlib.h>

template <typename T, typename KeyT = int>

struct cache_t
{
private:

	struct cache_unit_t
	{
		T cache_element_;
		size_t frequency_ = 0;

		cache_unit_t (T cache_element) : cache_element_ (cache_element) {}
	};

	size_t size_;
	std::list<cache_unit_t> cache_;

	using ListIt = typename std::list<cache_unit_t>::iterator;
	std::unordered_map<KeyT, ListIt> hash_;

public:

	cache_t (size_t sz) : size_ (sz) {}

	bool full () const
	{
		return cache_.size () == size_;
	}

	template <typename F>
	bool lookup_update (KeyT key, F slow_get_page)  //checks for element in cache; calls slow_get_page if failure
	{
		auto hit = hash_.find (key);

		if  (hit == hash_.end ())
		{
			if  (full ())
			{
				auto lfu_element = get_LFU_element ();

				hash_.erase (lfu_element->cache_element_.id_);
				cache_.erase (lfu_element);
			}
			cache_.push_front (slow_get_page (key));
			hash_[key] = cache_.begin ();
			return false;
		}

		hit->second->frequency_++;

		return true;
	}

	ListIt get_LFU_element ()
	{
		auto lfu_element = cache_.begin ();

		for  (auto i = cache_.begin (); i != cache_.end (); i++)
		{
			if  (lfu_element->frequency_ > i->frequency_)
				lfu_element = i;
		}

		return lfu_element;
	}

	void dump ()
	{
		for  (auto i = cache_.begin (); i != cache_.end (); i++)
			std::cout << i->cache_element_.id_ << " (" << i->frequency_ << ")   ";

		std::cout << "\r\n";
	}

	template <typename F>
	int vanga_cache (KeyT* elements, int n_elements, F slow_get_page) //ideal cache algorithm
	{
		int hits = 0;

		for  (int i = 0; i < n_elements; i++)
		{
			auto hit = hash_.find (elements[i]);

			if  (hit == hash_.end ())
			{
				if  (full ())
				{
					ListIt max_distance_element = cache_.begin ();
					int max_distance = check_distance (elements + i, n_elements - i, max_distance_element);

					for  (auto iter = cache_.begin (); iter != cache_.end (); iter++)
					{
						int current_distance = check_distance (elements + i, n_elements - i, iter);

						if  (current_distance > max_distance)
						{
							max_distance = current_distance;
							max_distance_element = iter;
						}
					}

					hash_.erase (max_distance_element->cache_element_.id_);
					cache_.erase (max_distance_element);
				}
				cache_.push_front (slow_get_page (elements[i]));
				hash_[elements[i]] = cache_.begin ();
			}
			else
				hits++;
		}


		return hits;
	}

	int check_distance (KeyT* elements, int n_elements, ListIt cache_unit) //returns distance to next occurance of element in elements
	{
		int distance = 0;

		for  (int i = 0; i < n_elements; i++)
		{
			if  (elements[i] == cache_unit->cache_element_.id_)
			{
				distance = i;
				break;
			}

			distance++;
		}

		return distance;
	}
};


struct page_t
{
	int id_;

	page_t (int id) : id_ (id) {}
};


page_t slow_get_page (int key)
{
	return { key };
}