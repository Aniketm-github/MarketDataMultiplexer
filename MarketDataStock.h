#pragma once

#include <iostream>

// TODO: Reference additional headers your program requires here.
#include <string>
#include <fstream>
#include <filesystem>
#include <queue>
#include <vector>

#define BATCH_SIZE 500
#define WRITE_BATCH_SIZE 100*BATCH_SIZE

class MarketDataStock {
	std::string _stock_name;
	std::string _stock_file_path;
	std::ifstream::pos_type _seek;
	int _pending_entries_in_pq;

public:
	MarketDataStock(const std::string& stock_name, const std::string& path);

	bool read_one_batch(std::vector<std::string>& tick_entries);
	std::string get_stock_name() const {
		return _stock_name;
	}
	void update_pending_entries_in_pq(int delta) {
		_pending_entries_in_pq += delta;
	}
};