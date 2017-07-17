#ifndef PAISANO_HPP
#define PAISANO_HPP

#include <vector>

namespace paisano {

	class RangeIndex {
	public:
		RangeIndex(const int64_t start, const int64_t stop, const int step);

	private:
		int64_t start_, stop_, step_;
	};

	RangeIndex::RangeIndex(const int64_t start,
						   const int64_t stop,
						   const int step) :
		start_(start),
		stop_(stop),
		step_(step)
	{
	}


	template <typename T>
	class Series {
	public:
		Series(const std::vector<T>& data);

		const std::vector<T>& data() const;

	private:
		std::vector<T> data_;
		RangeIndex index_;
	};

	template <typename T>
	Series<T>::Series(const std::vector<T>& data) :
		data_(data),
		index_(RangeIndex(0, data.size(), 1))
	{
	}

	template <typename T>
	const std::vector<T>& Series<T>::data() const
	{
		return data_;
	}
}

#endif
