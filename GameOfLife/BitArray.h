#pragma once
#define STR_BIT_ARRAY_BLOCK_TYPE int
#include<vector>

namespace str{
	class BitArray
	{
	private:
		std::vector<STR_BIT_ARRAY_BLOCK_TYPE> data;
		size_t _size = 0;
		const size_t BLOCK_SIZE = sizeof(STR_BIT_ARRAY_BLOCK_TYPE);

	public:
		BitArray(size_t size) {
			for (int i = 0; i < size; i += BLOCK_SIZE) {
				data.push_back(0);
			}
			_size = size;
		}

		const size_t &size() const { return _size; }

		void push_back(bool value) {
			if (_size % BLOCK_SIZE == 0) {
				data.push_back(0);
			}

			set(_size, value);
			++_size;
		}

		bool get(size_t i) const {
			return data[i / BLOCK_SIZE] & (1 << i % BLOCK_SIZE);
		}

		void set(size_t i, bool value) {
			if (value) {
				data[i / BLOCK_SIZE] |= (1 << (i % BLOCK_SIZE));
			}
			else {
				data[i / BLOCK_SIZE] &= ~(1 << (i % BLOCK_SIZE));
			}
		}

		bool operator[] (size_t i) const { return get(i); }
	};

}