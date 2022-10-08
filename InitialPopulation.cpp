// InitialPopulation.cpp : Creates the market data files with ticks.
//

#include "InitialPopulation.h"
#include <sstream>
#include <locale>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>

using namespace std;
namespace fs = std::filesystem;

InitialPopulation::InitialPopulation() {
	v_stocks = { "RELIANCE", "TCS", "INFOSYS", "HDFC", "SBI", "MARUTI", "HUL", "LT", "CIPLA", "BPCL" };
	v_stocks_prices = { 2571, 3022, 1517, 1032, 512, 934, 2308, 1817, 34, 343 };
}


void InitialPopulation::populate_queue_with_ticks(queue<string>& market_data, double px_range) {
	for (auto i = 0; i < 216000; ++i) {
		string type, ecn, size, price, timestamp;
		auto num = rand() % 3;

		if (num == 0) {
			type = "Ask";
		}
		else if (num == 1) {
			type = "Bid";
		}
		else {
			type = "TRADE";
		}

		num = rand() % 2;
		ecn = num == 0 ? "NSE" : "BSE";

		num = rand() % 2;
		size = num == 0 ? "110" : "120";

		num = rand() % 500;
		
		price = to_string(px_range + (double)num / 100);
		price.erase(price.length() - 4);

		num = rand() % 1000;
		auto hr = 10 + i / 36000, mins = (i % 36000) / 600, secs = (i % 600) / 10;

		std::time_t t = std::time(nullptr);
		std::tm tm = *std::localtime(&t);

		tm.tm_hour = hr;
		tm.tm_min = mins;
		tm.tm_sec = secs;

		std::cout.imbue(std::locale("ru_RU.utf8"));
		stringstream transTime;
		transTime << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
		double millis = ((i % 10) * 100 + double(rand() % 100)) / 1000;
		timestamp = transTime.str() + to_string(millis).erase(0, 1);
		timestamp.erase(timestamp.length() - 3);

		string entry = timestamp + ", " + price + ", " + size + ", " + ecn + ", " + type;

		market_data.push(entry);
	}
}

void InitialPopulation::write_market_data_to_file(queue<string>& market_data, const string& file_name) {
	ofstream file(file_name);

	if (file.is_open()) {
		while (market_data.size()) {
			file << market_data.front() << endl;
			market_data.pop();
		}
	}
}

void InitialPopulation::run() {
	// Create a data directory 
	const fs::path data{"data"};

	if (!fs::exists(data)) {
		auto check = fs::create_directory("data");
		// check if directory is created or not
		if (!check)
			printf("Directory created\n");
		else {
			printf("Unable to create directory\n");
			exit(1);
		}
	}

	// Create 10 files with random stock names and
	// Add 216000 market data ticks in each file
	for (auto i = 0; i < v_stocks.size(); ++i) {
		queue<string> market_data;
		populate_queue_with_ticks(market_data, v_stocks_prices[i]);

		string filen_name_with_path = data.string() + "\\" + v_stocks[i] + ".txt";
		write_market_data_to_file(market_data, filen_name_with_path);
	}
}
