// MarketDataEntry.cpp : Defines the market data tick information.
//

#include "MarketDataEntry.h"
#include <sstream>
#include <locale>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace chrono;

#define SEPARATOR ','
#define MILLIS_PRECISION_SEPARATOR '.'

MarketDataEntry::MarketDataEntry (const string& strtick) : _tick_info(strtick)/*, _tick_time(0.0)*/ {
	auto pos = _tick_info.find(SEPARATOR);
	if (pos == string::npos)
		throw std::exception(string("Invalid Market Data - " + _tick_info).c_str());

	_tick_time = _tick_info.substr(0, pos);

	//create_entry();
}

//void MarketDataEntry::create_entry() {
//	//static long entry_time = 0;
//	//auto start = high_resolution_clock::now();
//
//	auto pos = _tick_info.find(SEPARATOR);
//	if (pos == string::npos)
//		throw std::exception(string("Invalid Market Data - " + _tick_info).c_str());
//
//	_tick_time = _tick_info.substr(0, pos);
//
//	//std::tm t = {};
//	//std::istringstream ss(_tick_info.substr(0, pos));
//	//ss.imbue(std::locale("de_DE.utf-8"));
//	//// 2021-03-05 10:00:00.123
//	//ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
//
//	//auto gmtime = _mkgmtime(&t);
//	//pos = ss.str().find(MILLIS_PRECISION_SEPARATOR);
//	//if (pos == string::npos)
//	//	throw std::exception(string("Invalid Market Data - " + _tick_info).c_str());
//
//	//stringstream ssmillis(ss.str().substr(pos+1));
//	//int millis = 0;
//	//ssmillis >> millis;
//	//_tick_time = gmtime + (double)millis / 1000;
//
//	//auto end = high_resolution_clock::now();
//	//auto duration = duration_cast<microseconds>(end - start);
//	//entry_time += duration.count();
//
//	//cout << "Time taken to create entry : "
//	//	<< duration.count() << " microseconds" << endl;
//}


bool MarketDataEntry::operator > (const MarketDataEntry& other) const {
	return this->_tick_time > other.get_tick_time();
}

bool MarketDataEntry::operator == (const MarketDataEntry& other) const {
	return this->_tick_time == other.get_tick_time();
}
