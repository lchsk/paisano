#ifndef PAISANO_HPP
#define PAISANO_HPP

#include <vector>

namespace paisano {

	class None {};

	class RangeIndex {
	public:
		RangeIndex();
		RangeIndex(const int64_t start, const int64_t stop, const int step);

	private:
		int64_t start_, stop_, step_;
	};

	RangeIndex::RangeIndex()
	{
	}

	RangeIndex::RangeIndex(const int64_t start,
						   const int64_t stop,
						   const int step) :
		start_(start),
		stop_(stop),
		step_(step)
	{
	}


	template <typename T, typename U = None>
	class Series {
	public:
		Series(const std::vector<T>& data);
		Series(const std::vector<T>& data, const std::vector<U>& index);

		const std::vector<T>& data() const;

	private:
		std::vector<T> data_;
		std::vector<U> index_;
		RangeIndex range_index_;
	};

	template <typename T, typename U>
	Series<T, U>::Series(const std::vector<T>& data) :
		data_(data),
		range_index_(RangeIndex(0, data.size(), 1))
	{
	}

	template <typename T, typename U>
	Series<T, U>::Series(const std::vector<T>& data, const std::vector<U>& index) :
		data_(data),
		index_(index)
	{
	}

	template <typename T, typename U>
	const std::vector<T>& Series<T, U>::data() const
	{
		return data_;
	}
}

#endif
