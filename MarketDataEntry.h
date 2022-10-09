#pragma once

#include <iostream>

// TODO: Reference additional headers your program requires here.
#include "MarketDataStock.h"
#include <string>

class MarketDataEntry {
	std::string _tick_info;
	MarketDataStock* _stock_file_info;

public:
	MarketDataEntry(const std::string& strtick, MarketDataStock* ptr) : _tick_info(strtick), _stock_file_info(ptr) {}
	bool operator > (const MarketDataEntry& other) const {
		auto diff = strncmp(this->_tick_info.c_str(), other.get_tick_info().c_str(), 23);
		if (diff == 0)
			return this->get_stock_name() > other.get_stock_name();

		return diff > 0;
	}
	std::string get_tick_info() const {
		return _tick_info;
	}
	std::string get_stock_name() const {
		return _stock_file_info->get_stock_name();
	}
	void update_pending_entries_in_pq(int delta) const {
		_stock_file_info->update_pending_entries_in_pq(delta);
	}
};