#ifndef PAISANO_HPP
#define PAISANO_HPP

#include <vector>

namespace paisano {

	class None {};

	template <typename T>
	class IndexBase {
	public:
		virtual std::vector<T>& get_index(){};
	};

	template <typename T>
	class Index : public IndexBase<T> {
	public:
		Index(const std::vector<T>& index);

	private:
		std::vector<T> index_;
		std::vector<T>& get_index();
	};

	template <typename T>
	Index<T>::Index(const std::vector<T>& index) :
		index_(index)
	{
	}

	template <typename T>
	std::vector<T>& Index<T>::get_index()
	{
		return index_;
	}

	class RangeIndex : public IndexBase<None> {
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
		Series(const std::vector<T>& data, const RangeIndex& index);
		Series(const std::map<U, T>& map);

		const std::vector<T>& data() const;

	private:
		std::vector<T> data_;
		std::unique_ptr<IndexBase<U>> index_;
	};

	template <typename T, typename U>
	Series<T, U>::Series(const std::vector<T>& data, const RangeIndex& index) :
		data_(data),
		index_(std::make_unique<RangeIndex>(index))
	{
	}

	template <typename T, typename U>
	Series<T, U>::Series(const std::vector<T>& data) :
		Series<T, U>(data, RangeIndex(0, data.size(), 1))
	{
	}

	template <typename T, typename U>
	Series<T, U>::Series(const std::vector<T>& data, const Index<U>& index) :
		data_(data),
		index_(std::make_unique<Index<U> >(index))
	{
	}

	template <typename T, typename U>
	Series<T, U>::Series(const std::map<U, T>& map) :
		index_(std::make_unique<Index<U> >(std::vector<U>(map.size())))
	{
		data_.resize(map.size());

		for (const auto &i : map) {
			index_->get_index().push_back(i.first);
			data_.push_back(i.second);
		}
	}

	template <typename T, typename U>
	const std::vector<T>& Series<T, U>::data() const
	{
		return data_;
	}
}

#endif
