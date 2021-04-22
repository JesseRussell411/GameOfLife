//#include "GameOfLife.h"
//#include "Stopwatch.h"
//#include <iostream>
//#include<string>
//#include<thread>
//#include<vector>
//using namespace JesseRussell::Diagnostics;
//
//
//int main() {
//	std::vector<std::thread> jobs;
//
//	for (int i = 0; i < 2; ++i) {
//		jobs.push_back(
//			std::thread([]() {
//				GameOfLife game(10000, 10000);
//				game.scramble();
//				Stopwatch watch;
//				std::cout << game.getThreadCount() << "\n";
//
//				while (true) {
//					watch.restart();
//					game.step();
//					watch.stop();
//					std::cout << watch.getElapsedMilliseconds() << "\n";
//				}
//				}
//		));
//	}
//	for (int i = 0; i < jobs.size(); ++i) {
//		jobs[i].join();
//	}
//	
//}