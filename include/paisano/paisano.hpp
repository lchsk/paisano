#ifndef PAISANO_HPP
#define PAISANO_HPP

#include <vector>

namespace paisano {

	class None {};

	class IndexBase {};

	template <typename T>
	class Index : public IndexBase {
	public:
		Index(const std::vector<T>& index);

	private:
		std::vector<T> index_;
	};

	template <typename T>
	Index<T>::Index(const std::vector<T>& index) :
		index_(index)
	{
	}

	class RangeIndex : public IndexBase {
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
		Series(const std::vector<T>& data, const Index<U>& index);

		const std::vector<T>& data() const;

	private:
		std::vector<T> data_;
		std::unique_ptr<IndexBase> index_;
	};

	template <typename T, typename U>
	Series<T, U>::Series(const std::vector<T>& data) :
		data_(data),
		index_(std::make_unique<RangeIndex>(0, data.size(), 1))
	{
	}

	template <typename T, typename U>
	Series<T, U>::Series(const std::vector<T>& data, const Index<U>& index) :
		data_(data),
		index_(std::make_unique<Index<U> >(index))
	{
	}

	template <typename T, typename U>
	const std::vector<T>& Series<T, U>::data() const
	{
		return data_;
	}
}

#endif
