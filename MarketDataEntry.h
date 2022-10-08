#pragma once

#include <iostream>

// TODO: Reference additional headers your program requires here.
#include <string>
#include <time.h>


class MarketDataEntry {
	std::string _tick_info;
	//double _tick_time;
	std::string _tick_time;

	//void create_entry();

public:
	MarketDataEntry(const std::string& strtick);
	bool operator > (const MarketDataEntry& other) const;
	bool operator == (const MarketDataEntry& other) const;

	std::string get_tick_time() const {
		return _tick_time;
	}
	std::string get_tick_info() const {
		return _tick_info;
	}
};