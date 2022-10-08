// MarketDataMultiplexer.cpp : Defines the entry point for the application.
//

#include "MarketDataMultiplexer.h"
#include <filesystem>
#include <thread>

using namespace std;
namespace fs = std::filesystem;

MarketDataMultiplexer::MarketDataMultiplexer() {
    initialize();
}

bool MarketDataMultiplexer::initialize() {
	// read all the files in data directory and create a vector of MarketDataStock
    std::string path = "data";
    for (const auto& file_entry : fs::directory_iterator(path)) {
        if (file_entry.is_regular_file()) {
            auto path = file_entry.path().string();
            auto pos1 = path.find_last_of('.'), pos2 = path.find_last_of('\\');
            string stock_name = path.substr(pos2+1, pos1-pos2-1);
            auto ptr = make_shared<MarketDataStock>(stock_name, file_entry.path().string());
            _stock_files.push_back(ptr);
        }
    }

    // Clear the output.txt file before starting
    ofstream file("output.txt", std::ios::out | std::ios::trunc);

    return true;
}

void MarketDataMultiplexer::write_to_output_file() {
    bool exit_thread = false;
    ofstream file("output.txt", std::ios::binary | std::ios::app);
    if (!file.is_open()) {
        std::cout << "Unable to open output.txt..." << endl;
        return;
    }
    while (!exit_thread) {
        unique_lock lock(_mtx_synch_stock_queue);
        _cv_synch_stock_queue.wait(lock, [&] { return !_ready_to_fill_stock_queue; });
        // at this moment if queue is empty, it means all the data is written into file and 
        // we can stop the thread
        if (_writer_stock_tick.size() > 0) {
            // flushes the queue into output.txt
            while (_writer_stock_tick.size()) {
                file << _writer_stock_tick.front() << '\n';
                _writer_stock_tick.pop();
            }
            // Notify reader thread to push into queue
            _is_write_queue_empty = true;
            _ready_to_fill_stock_queue = true;
            _cv_synch_stock_queue.notify_one();
        }
        if (_read_thread_finished) {
            exit_thread = true;
        }
    }
}

void MarketDataMultiplexer::read_from_input_files() {
    bool exit_thread = false;
    // Run a for loop over stocks
    // open stock file, fill pq with one batch
    while (!exit_thread) {
        vector<string> tick_entries;
        for (auto ptr : _stock_files) {
            tick_entries.clear();
            if (ptr->read_one_batch(tick_entries)) {
                auto size = tick_entries.size();
                // Push entry into pq and increament the count
                for (auto& entry : tick_entries) {
                    _pq.emplace(entry, ptr.get());
                }
                ptr->update_pending_entries_in_pq(static_cast<int>(size));
            }
        }
        if (_pq.size() > 0) {
            // remove one batch from pq and push into a queue 
            auto n_remove_items = BATCH_SIZE < _pq.size() ? BATCH_SIZE : _pq.size();
            for (auto i = 0; i < n_remove_items; ++i) {
                // remove entry from priority_queue 
                // Reduce the _pending_entries_in_pq from MarketDataStock
                auto& entry = _pq.top();
                entry.second->update_pending_entries_in_pq(-1);
                _reader_stock_tick.emplace(entry.second->get_stock_name() + ", " + entry.first);
                _pq.pop();
            }
            // Wait till write operation is completed
            if (_reader_stock_tick.size() > 0 && _is_write_queue_empty) {
                unique_lock lock(_mtx_synch_stock_queue);
                _cv_synch_stock_queue.wait(lock, [&] { return _ready_to_fill_stock_queue; });
                std::swap(_reader_stock_tick, _writer_stock_tick);
                // Notify write thread to continue writing
                _is_write_queue_empty = false;
                _ready_to_fill_stock_queue = false;
                _cv_synch_stock_queue.notify_one();
            }
        }
        else {
            exit_thread = true;
        }
    }
    // Reader thread is done, now notify write thread to finish writing
    unique_lock lock(_mtx_synch_stock_queue);
    _cv_synch_stock_queue.wait(lock, [&] { return _ready_to_fill_stock_queue; });
    std::swap(_reader_stock_tick, _writer_stock_tick);
    _is_write_queue_empty = false;
    _ready_to_fill_stock_queue = false;
    _cv_synch_stock_queue.notify_one();

    _read_thread_finished = true;
}

void MarketDataMultiplexer::run() {
    // launch reader and writer thread 
    std::thread reader(&MarketDataMultiplexer::read_from_input_files, this);
    std::thread writer(&MarketDataMultiplexer::write_to_output_file, this);

    // wait for both the threads to finish
    reader.join();
    writer.join();
}

