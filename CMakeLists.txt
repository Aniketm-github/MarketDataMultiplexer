# CMakeList.txt : CMake project for MarketDataMultiplexer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
project ("MarketDataMultiplexer")

# Add source to this project's executable.
add_executable (MarketDataMultiplexer "main.cpp"
"MarketDataMultiplexer.cpp"
"MarketDataMultiplexer.h"
"InitialPopulation.cpp"
"InitialPopulation.h"
"MarketDataEntry.cpp"
"MarketDataEntry.h"
"MarketDataStock.cpp"
"MarketDataStock.h"
"MarketDataOrderVerifier.cpp"
"MarketDataOrderVerifier.h"
 "SPSCQueue.h")

# TODO: Add tests and install targets if needed.
