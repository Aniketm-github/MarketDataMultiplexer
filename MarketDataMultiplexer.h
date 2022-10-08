// MarketDataMultiplexer.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

// TODO: Reference additional headers your program requires here.
#include "MarketDataStock.h"
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>

using Entry = std::pair<std::string, MarketDataStock*>;

struct comparator {
	bool operator() (const Entry& p1, const Entry& p2) const {
		auto diff = strncmp(p1.first.c_str(), p2.first.c_str(), 23);
		if (diff == 0)
			return p1.second->get_stock_name() > p2.second->get_stock_name();
		
		return diff > 0;
	}
};

class MarketDataMultiplexer {
	std::priority_queue <Entry, std::vector<Entry>, comparator> _pq;
	std::queue<std::string> _reader_stock_tick;
	std::queue<std::string> _writer_stock_tick;
	std::vector<std::shared_ptr<MarketDataStock>> _stock_files;

	// Synchronize the read-write threads
	std::mutex _mtx_synch_stock_queue;
	std::condition_variable _cv_synch_stock_queue;
	bool _ready_to_fill_stock_queue = true;
	std::atomic_bool _is_write_queue_empty = true, _read_thread_finished = false;

	bool initialize();
	void write_to_output_file();
	void read_from_input_files();
public:
	MarketDataMultiplexer();
	void run();
};