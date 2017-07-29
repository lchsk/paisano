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
		virtual std::vector<T>& get_index_(){};
		virtual std::vector<T>& assert_invariants_(const std::size_t data_size){};
	};

	template <typename T>
	class Index : public IndexBase<T> {
	public:
		Index(const std::vector<T>& index);

	private:
		std::vector<T>& assert_invariants_(const std::size_t data_size);
		std::vector<T>& get_index_();

		std::vector<T> index_;
	};

	template <typename T>
	Index<T>::Index(const std::vector<T>& index) :
		index_(index)
	{
	}

	template <typename T>
	std::vector<T>& Index<T>::get_index_()
	{
		return index_;
	}

	template <typename T>
	std::vector<T>& Index<T>::assert_invariants_(const std::size_t data_size)
	{
		if (data_size != index_.size()) {
			std::stringstream s;
			s << "Invalid number of arguments, data has "
			  << data_size
			  << " elements and index has "
			  << index_.size()
			  << " elements";

			throw std::invalid_argument(s.str());
		}
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
		void assert_invariants_();

		std::vector<T> data_;
		std::unique_ptr<IndexBase<U>> index_;
	};

	template <typename T, typename U>
	Series<T, U>::Series(const std::vector<T>& data, const RangeIndex& index) :
		data_(data),
		index_(std::make_unique<RangeIndex>(index))
	{
		assert_invariants_();
	}

	template <typename T, typename U>
	Series<T, U>::Series(const std::vector<T>& data) :
		Series<T, U>(data, RangeIndex(0, data.size(), 1))
	{
		assert_invariants_();
	}

	template <typename T, typename U>
	Series<T, U>::Series(const std::vector<T>& data, const Index<U>& index) :
		data_(data),
		index_(std::make_unique<Index<U> >(index))
	{
		assert_invariants_();
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
			index_->get_index_().push_back(i.first);
			data_.push_back(i.second);
		}
	}

	template <typename T, typename U>
	void Series<T, U>::assert_invariants_()
	{
		index_->assert_invariants_(data_.size());
	}

	template <typename T, typename U>
	const std::vector<T>& Series<T, U>::data() const
	{
		return data_;
	}
}

#endif
