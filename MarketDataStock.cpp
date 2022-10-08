// MarketDataStock.cpp : Defines the market data stock information.
//

#include "MarketDataStock.h"
#include <sstream>
#include <locale>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace chrono;

MarketDataStock::MarketDataStock(const string& stock_name, const std::string& path)
	: _stock_name(stock_name), _stock_file_path(path), _seek(0), _pending_entries_in_pq(0) {
}

bool MarketDataStock::read_one_batch(vector<string>& tick_entries) {
	if (_pending_entries_in_pq < BATCH_SIZE) {
        std::ifstream stock_file(_stock_file_path, std::ios::in);
        if (stock_file.is_open()) {
            stock_file.seekg(_seek);
            tick_entries.reserve(BATCH_SIZE);
            string line;
            for (auto i = 0; i < BATCH_SIZE; ++i) {
                line.clear();
                // Check if reached end of the file
                if (stock_file.peek() == EOF)
                    break;
                std::getline(stock_file, line);
                tick_entries.emplace_back(line);
            }
            _seek = stock_file.tellg();
        }
	}
    return true;
}
