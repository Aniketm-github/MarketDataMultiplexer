// MarketDataMultiplexer.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <exception>
#include <memory>
#include <chrono>
#include "InitialPopulation.h"
#include "MarketDataMultiplexer.h"
#include "MarketDataOrderVerifier.h"

using namespace std;
using namespace chrono;

int main(int argc, char* argv[])
{
    try {

        if (argc != 2) {
            cout << "No Command Line Argument given to run the application." << endl;
            cout << "Use one of the below param to run the application..." << endl << endl;
            cout << "-i : Initial population of market data with random stocks and ticks in data directory." << endl;

            cout << "-m : Reads the market data from data directory and write into output.txt in sorted order." << endl;
            cout << "-v : Verifies the market data in output.txt file is in sorted order." << endl;
        }
        else {
            cout << "Application running with command line parameter " << argv[1] << endl;
            if (strcmp(argv[1], "-i") == 0) {
                unique_ptr<InitialPopulation> initial_population = make_unique<InitialPopulation> ();

                initial_population->run();                
            }
            else if (strcmp(argv[1], "-m") == 0) {
                for (auto i = 0; i < 10; ++i) {
                    auto start = high_resolution_clock::now();
                    // Create and initialize the multiplexer class instance
                    unique_ptr<MarketDataMultiplexer> multiplexer = make_unique<MarketDataMultiplexer>();

                    // run the ordering mechanism to read from files and write to output.txt
                    multiplexer->run();
                    auto stop = high_resolution_clock::now();
                    auto duration2 = duration_cast<microseconds>(stop - start);
                    cout << "Run no ==> " << i << " ==> Time taken to generate output.txt: "
                        << duration2.count() << " microseconds" << endl;
                }
            }
            else if (strcmp(argv[1], "-v") == 0) {
                unique_ptr<MarketDataOrderVerifier> verifier = make_unique<MarketDataOrderVerifier>();

                verifier->run();
            }
            else {
                cout << "Invalid Command Line Argument given to run the application." << endl;
                cout << "Use one of the below param to run the application..." << endl << endl;
                cout << "-i : Initial population of market data with random stocks and ticks in data directory." << endl;
                cout << "-m : Reads the market data from data directory and write into output.txt in sorted order." << endl;
                cout << "-v : Verifies the market data in output.txt file is in sorted order." << endl;
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Program terminating with exception : " << e.what() << endl;
    }
    catch (...) {
        std::cout << "Program terminating due to unknown exception." << endl;
    }

	return 0;
}
