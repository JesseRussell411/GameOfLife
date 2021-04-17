#include<vector>
#include"BitArray.h"
#pragma once
class GameOfLife
{
private:
	std::vector<std::vector<bool>> world;
	size_t _height;

	void _set(size_t x, size_t y, bool value) {
		world[x][y] = value;
	}

	bool _get(size_t x, size_t y) const {
		return world[x][y];
	}

public:
	GameOfLife(size_t width, size_t height) {
		_height = height;
		for (size_t i = 0; i < width; ++i) {
			world.push_back(std::vector<bool>(height));
			for (size_t j = 0; j < height; ++j) {
				world[i].push_back(false);
			}
		}
	}

	size_t width() const  { return world.size(); }
	size_t height() const { return _height; }
	
	void set(size_t x, size_t y, bool value) {
		if (x < 0 || width() <= x)  { return; }
		if (y < 0 || height() <= y) { return; }

		world[x][y] = value;
	}
	
	bool get(size_t x, size_t y) const {
		if (x < 0 || width() <= x)  { return false; }
		if (y < 0 || height() <= y) { return false; }

		return world[x][y];
	}

	void reset(bool state = false) {
		for (size_t x = 0; x < width(); ++x) {
			for (size_t y = 0; y < height(); ++y) {
				_set(x, y, state);
			}
		}
	}

	void scramble() {
		for (size_t x = 0; x < width(); ++x) {
			for (size_t y = 0; y < height(); ++y) {
				_set(x, y, rand());
			}
		}
	}

	void step() {
		std::vector<std::vector<bool>> newWorld(width());
		for (size_t i = 0; i < width(); ++i) {
			newWorld[i] = std::vector<bool>(height());
		}

		for (size_t x = 0; x < width(); ++x) {
			for (size_t y = 0; y < height(); ++y) {
				int neighbors = 0;
				neighbors +=
					get(x - 1, y - 1) +
					get(x    , y - 1) +
					get(x + 1, y - 1) +
					get(x + 1, y    ) +
					get(x + 1, y + 1) +
					get(x    , y + 1) +
					get(x - 1, y + 1) +
					get(x - 1, y    );

				if (neighbors == 3){
					newWorld[x][y] = true;
				}
				else if (neighbors == 2) {
					newWorld[x][y] = _get(x, y);
				}
				else {
					newWorld[x][y] = false;
				}
			}
		}

		world = newWorld;



		// lower memory:

		//std::vector<bool> temp1(height());
		//std::vector<bool> temp2(height());
		//size_t x, y;
		//std::vector<bool> &tempa = temp1, &tempb = temp2;

		//auto swapTemp = [&]() {
		//	auto t = tempa;
		//	tempa = tempb;
		//	tempb = t;
		//};

		//auto find = [&](size_t column, size_t row) -> bool {
		//	if (column < 0 || width() <= column)  { return false; }
		//	if (row < 0 || height() <= row) { return false; }

		//	if (column == x - 1) {
		//		return tempa[row];
		//	}
		//	if (column == x && row == y - 1) {
		//		return tempb[row];
		//	}
		//	return _get(column, row);
		//};

		//for (x = 0; x < width(); ++x) {
		//	for (y = 0; y < height(); ++y) {
		//		int neighbors = 0;
		//		neighbors +=
		//			find(x - 1, y - 1) +
		//			find(x    , y - 1) +
		//			find(x + 1, y - 1) +
		//			find(x + 1, y    ) +
		//			find(x + 1, y + 1) +
		//			find(x    , y + 1) +
		//			find(x - 1, y + 1) +
		//			find(x - 1, y    );
		//		
		//		tempb[y] = _get(x, y);

		//		if (neighbors < 2 || 3 < neighbors) {
		//			_set(x, y, false);
		//		}
		//		else if (neighbors == 3) {
		//			_set(x, y, true);
		//		}
		//	}
		//	swapTemp();
		//}
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