#ifndef PAISANO_HPP
#define PAISANO_HPP

#include <vector>
#include <unordered_map>

namespace paisano {
	class None {};
}

namespace std {
	template <>
	struct hash<paisano::None>
	{
		size_t operator()(const paisano::None& none) const
		{
			// TODO: Make it a compilation error
			throw std::runtime_error("Cannot use paisano::None as a key");
		}
	};
}

namespace paisano {
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
		Series(const std::unordered_map<U, T>& map);

		const std::vector<T>& data() const;

	private:
		template <typename MAP>
		void init_map_(const MAP& map);

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
		init_map_(map);
	}

	template <typename T, typename U>
	Series<T, U>::Series(const std::unordered_map<U, T>& map) :
		index_(std::make_unique<Index<U> >(std::vector<U>(map.size())))
	{
		init_map_(map);
	}

	template <typename T, typename U>
	template <typename MAP>
	void Series<T, U>::init_map_(const MAP& map)
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
