// MarketDataMultiplexer.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

// TODO: Reference additional headers your program requires here.
#include "MarketDataEntry.h"
#include "SPSCQueue.h"
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
	//bool operator() (const std::unique_ptr<MarketDataEntry>& entry1, const std::unique_ptr<MarketDataEntry>& entry2) const {
	//	return *entry1 > *entry2;
	//}
	//bool operator() (const MarketDataEntry* entry1, const MarketDataEntry* entry2) const {
	//	return *entry1 > *entry2;
	//}
	//bool operator() (const MarketDataEntry& entry1, const MarketDataEntry& entry2) const {
	//	return entry1 > entry2;
	//}

	bool operator() (const Entry& p1, const Entry& p2) const {
		auto diff = strncmp(p1.first.c_str(), p2.first.c_str(), 23);
		if (diff == 0)
			return p1.second->get_stock_name() > p2.second->get_stock_name();

		return diff > 0;
	}
};

class MarketDataMultiplexer {
	//std::priority_queue <std::unique_ptr<MarketDataEntry>, std::vector<std::unique_ptr<MarketDataEntry>>, comparator> _pq;
	//std::queue<std::unique_ptr<MarketDataEntry>> _reader_stock_tick;
	//std::queue<std::unique_ptr<MarketDataEntry>> _writer_stock_tick;

	std::priority_queue <Entry, std::vector<Entry>, comparator> _pq;
	std::queue<std::string> _reader_stock_tick;
	std::queue<std::string> _writer_stock_tick;

	//std::priority_queue <MarketDataEntry*, std::vector<MarketDataEntry*>, comparator> _pq;
	//std::queue<MarketDataEntry*> _reader_stock_tick;
	//std::queue<MarketDataEntry*> _writer_stock_tick;

	//SPSCQueue<std::string> _stock_ticks;

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