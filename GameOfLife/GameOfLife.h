#include<vector>
#include<memory>
#include"BitArray.h"
#pragma once
class GameOfLife
{
public:
	struct World {
		friend class GameOfLife;
	private:
		std::shared_ptr<bool[]> data;

		size_t _height, _width;

		bool &ref_get(size_t x, size_t y) {
			return data[x + y * width()];
		}

		bool _get(size_t x, size_t y) const {
			return data[x + y * width()];
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
			data = std::shared_ptr<bool[]>(new bool[width * height]);

			_height = height;
			_width = width;

			if (initialize) {
				for (size_t i = 0; i < width * height; ++i) {
					data[i] = false;
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

public:
	GameOfLife(size_t width, size_t height) {
		world = World(width, height);
	}

	size_t width() const { return world.width(); }
	size_t height() const { return world.height(); }
	
	void set(size_t x, size_t y, bool value) {
		world.set(x, y, value);
	}
	
	bool get(size_t x, size_t y) const {
		return world.get(x, y);
	}

	void reset(bool state = false) {
		for (size_t x = 0; x < width(); ++x) {
			for (size_t y = 0; y < height(); ++y) {
				world.ref_get(x, y) = false;
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
		World newWorld(width(), height(), false);
		size_t x, y, largest_x = width() - 1, largest_y = height() - 1;

		for (x = 0; x < width(); ++x) {
			for (y = 0; y < height(); ++y) {
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

				if (neighbors == 3){
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