#include<vector>
#include<memory>
#include<thread>
#include<iostream>
#include"BitArray.h"
#pragma once
class GameOfLife
{
public:
	struct World {
		friend class GameOfLife;
	private:
		std::shared_ptr<std::shared_ptr<bool[]>[]> data;

		size_t _height, _width;

		bool &ref_get(size_t x, size_t y) {
			return data[x][y];
		}

		bool _get(size_t x, size_t y) const {
			return data[x][y];
		}
		void _set(size_t x, size_t y, bool value) {
			ref_get(x, y) = value;
		}

		bool outOfBounds(size_t x, size_t y) const {
			if (x < 0 || width() <= x) { return true; }
			if (y < 0 || height() <= y) { return true; }
			return false;
		}

	private:
		World(size_t width, size_t height, bool initialize) {
			_height = height;
			_width = width;

			data = std::shared_ptr<std::shared_ptr<bool[]>[]>(new std::shared_ptr<bool[]>[width]);
			if (initialize) {
				for (size_t x = 0; x < width; ++x) {
					data[x] = std::shared_ptr<bool[]>(new bool[height]);
					for (size_t y = 0; y < height; ++y) {
						data[x][y] = false;
					}
				}

			}
			else {
				for (size_t x = 0; x < width; ++x) {
					data[x] = std::shared_ptr<bool[]>(new bool[height]);
				}
			}
		}

	public:
		size_t width()  const { return _width; }
		size_t height() const { return _height; }

		World(size_t width, size_t height) : World(width, height, true) {}

		bool get(size_t x, size_t y) const {
			if (outOfBounds(x, y)) { return false; }
			return _get(x, y);
		}

		void set(size_t x, size_t y, bool value) {
			if (outOfBounds(x, y)) { return; }
		    _set(x, y, value);
		}
	};

private:
	World world = World(0,0);
	size_t threadCount = 1;
public:
	GameOfLife(size_t width, size_t height) {
		world = World(width, height);
		threadCount = width * height / 1000000;
		if (threadCount < 1) { threadCount = 1; }
	}

	size_t width() const { return world.width(); }
	size_t height() const { return world.height(); }
	
	size_t getThreadCount() const { return threadCount; }
	void setThreadCount(size_t value) { threadCount = value; }

	void set(size_t x, size_t y, bool value) {
		world.set(x, y, value);
	}
	
	bool get(size_t x, size_t y) const {
		return world.get(x, y);
	}

	void reset(bool state = false) {
		for (size_t x = 0; x < width(); ++x) {
			for (size_t y = 0; y < height(); ++y) {
				world._set(x, y, state);
			}
		}
	}

	void scramble() {
		for (size_t x = 0; x < width(); ++x) {
			for (size_t y = 0; y < height(); ++y) {
				world._set(x, y, std::rand() > RAND_MAX / 2);
			}
		}
	}

	void step() {
		if (threadCount <= 1) {
			singlethreadStep();
		}
		else {
			multithreadStep(threadCount);
		}
	}

	void singlethreadStep() {
		World newWorld(width(), height(), false);
		size_t x, y;

		partialStep(0, width(), 0, height(), newWorld);

		world = newWorld;
	}

	void multithreadStep(size_t threadCountTarget) {
		size_t width = this->width(), height = this->height();
		bool chunkByWidthNotHeight = width >= height;
		World newWorld(width, height);

		size_t threadCount, chunkSize, remainder;

		auto part = [&](size_t n) {
			threadCount = n < threadCountTarget ? n : threadCountTarget;
			chunkSize = n / threadCount;
			remainder = n % threadCount;
			if (remainder != 0) { ++threadCount; }
		};

		if (chunkByWidthNotHeight) {
			part(width);
		}
		else {
			part(height);
		}


		std::thread *jobs = new std::thread[threadCount];

		for (size_t i = 0; i < threadCount; ++i) {
			if (remainder != 0 && i == threadCount - 1) {
				if (chunkByWidthNotHeight) {
					jobs[threadCount - 1] = std::thread(
						[&]() { partialStep(width - remainder, width, 0, height, newWorld); }
					);
				}
				else {
					jobs[threadCount - 1] = std::thread(
						[&]() { partialStep(0, width, height - remainder, height, newWorld); }
					);
				}
			}
			else if (chunkByWidthNotHeight) {
				jobs[i] = std::thread(
					[=]() { partialStep(i * chunkSize, i * chunkSize + chunkSize, 0, height, newWorld); }
				);
			}
			else {
				jobs[i] = std::thread(
					[=]() { partialStep(0, width, i * chunkSize, i * chunkSize + chunkSize, newWorld); }
				);
			}
		}

		for (size_t i = 0; i < threadCount; ++i) {
			jobs[i].join();
		}
		world = newWorld;

		delete[] jobs;
	}

	void partialStep(size_t x1, size_t x2, size_t y1, size_t y2, World newWorld) const {
		size_t x, y, largest_x = width() - 1, largest_y = height() - 1;
		for (x = x1; x < x2; ++x) {
			for (y = y1; y < y2; ++y) {
				int neighbors = 0;
				if (x == 0 || x == largest_x || y == 0 || y == largest_y) {
					neighbors +=
						world.get(x - 1, y - 1) +
						world.get(x, y - 1) +
						world.get(x + 1, y - 1) +
						world.get(x + 1, y) +
						world.get(x + 1, y + 1) +
						world.get(x, y + 1) +
						world.get(x - 1, y + 1) +
						world.get(x - 1, y);
				}
				else {
					neighbors +=
						world._get(x - 1, y - 1) +
						world._get(x, y - 1) +
						world._get(x + 1, y - 1) +
						world._get(x + 1, y) +
						world._get(x + 1, y + 1) +
						world._get(x, y + 1) +
						world._get(x - 1, y + 1) +
						world._get(x - 1, y);
				}

				if (neighbors == 3) {
					newWorld._set(x, y, true);
				}
				else if (neighbors == 2) {
					newWorld._set(x, y, world._get(x, y));
				}
				else {
					newWorld._set(x, y, false);
				}
			}
		}
	}
};




/*


#include<vector>
#include"BitArray.h"
#pragma once
class GameOfLife
{
private:
	std::vector<str::BitArray> world;
	size_t _height;

	void _set(size_t x, size_t y, bool value) {
		world[x].set(y, value);
	}

	bool _get(size_t x, size_t y) {
		return world[x].get(y);
	}

public:
	GameOfLife(size_t width, size_t height) {
		_height = height;
		for (size_t i = 0; i < width; ++i) {
			world.push_back(str::BitArray(height));
		}
	}

	size_t width() { return world.size(); }
	size_t height() { return _height; }

	void set(size_t x, size_t y, bool value) {
		if (x < 0 || width() <= x)  { return; }
		if (y < 0 || height() <= y) { return; }

		world[x].set(y, value);
	}

	bool get(size_t x, size_t y) {
		if (x < 0 || width() <= x)  { return false; }
		if (y < 0 || height() <= y) { return false; }

		return world[x].get(y);
	}

	void step() {
		std::vector<bool> temp1(height());
		std::vector<bool> temp2(height());
		size_t x, y;
		std::vector<bool> &tempa = temp1, &tempb = temp2;

		auto swapTemp = [&]() {
			auto t = tempa;
			tempa = tempb;
			tempb = t;
		};

		auto find = [&](size_t column, size_t row) -> bool {
			if (column < 0 || width() <= column)  { return false; }
			if (row < 0 || height() <= row) { return false; }

			if (column == x - 1) {
				return tempa[row];
			}
			if (column == x && row == y - 1) {
				return tempb[row];
			}
			return _get(column, row);
		};

		for (x = 0; x < width(); ++x) {
			for (y = 0; y < height(); ++y) {
				int neighbors = 0;
				neighbors +=
					find(x - 1, y - 1) +
					find(x    , y - 1) +
					find(x + 1, y - 1) +
					find(x + 1, y    ) +
					find(x + 1, y + 1) +
					find(x    , y + 1) +
					find(x - 1, y + 1) +
					find(x - 1, y    );

				tempb[y] = _get(x, y);

				if (neighbors < 2 || 3 < neighbors) {
					_set(x, y, false);
				}
				else if (neighbors == 3) {
					_set(x, y, true);
				}
			}
			swapTemp();
		}
	}
};



*/