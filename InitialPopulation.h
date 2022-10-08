#pragma once

#include <iostream>
// TODO: Reference additional headers your program requires here.
#include <string>
#include <queue>
#include <vector>

class InitialPopulation {
	std::vector<std::string> v_stocks;
	std::vector<double> v_stocks_prices;
	void populate_queue_with_ticks( std::queue<std::string>& market_data, double px_range);
	void write_market_data_to_file( std::queue<std::string>& market_data, const std::string& file_name);

public:
	InitialPopulation();

	void run();
};