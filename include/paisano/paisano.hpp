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

        virtual const int64_t get_start_() {};
        virtual const int64_t get_stop_() {};
        virtual const int get_step_() {};
    };

    template <typename T>
    class Index : public IndexBase<T> {
    public:
        Index(const std::vector<T>& index);

    private:
        std::vector<T>& assert_invariants_(const std::size_t data_size);
        std::vector<T>& get_index_();

        const int64_t get_start_() { return 0; };
        const int64_t get_stop_() { return index_.size(); };
        const int get_step_() { return 1; };

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

        const int64_t get_start_() { return start_; };
        const int64_t get_stop_() { return stop_; };
        const int get_step_() { return step_; };

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
    class BaseSeries {
    public:
        BaseSeries(const std::vector<T>& data);
        BaseSeries(const std::vector<T>& data, const Index<U>& index);
        BaseSeries(const std::vector<T>& data, const RangeIndex& index);
        BaseSeries(const std::map<U, T>& map);
        BaseSeries(const std::unordered_map<U, T>& map);

        const T& get_index_by_int_(const int index) const;
        T& get_index_by_int_(const int index);

        const T& get_index_by_U_(const U& index) const;
        T& get_index_by_U_(const U& index);

        const std::vector<T>& data() const;

    protected:
        template <typename MAP>
        void init_map_(const MAP& map);
        void assert_invariants_();

        std::vector<T> data_;
        std::unique_ptr<IndexBase<U>> index_;
    };

    template <typename T, typename U>
    BaseSeries<T, U>::BaseSeries(const std::vector<T>& data,
                                 const RangeIndex& index) :
        data_(data),
        index_(std::make_unique<RangeIndex>(index))
    {
        assert_invariants_();
    }

    template <typename T, typename U>
    BaseSeries<T, U>::BaseSeries(const std::vector<T>& data) :
        BaseSeries<T, U>(data, RangeIndex(0, data.size(), 1))
    {
        assert_invariants_();
    }

    template <typename T, typename U>
    BaseSeries<T, U>::BaseSeries(const std::vector<T>& data,
                                 const Index<U>& index) :
        data_(data),
        index_(std::make_unique<Index<U> >(index))
    {
        assert_invariants_();
    }

    template <typename T, typename U = None>
    class Series : public BaseSeries<T, U> {
    public:
        Series(const std::vector<T>& data, const Index<U>& index)
            : BaseSeries<T, U>(data, index) {}
        Series(const std::map<U, T>& map)
            : BaseSeries<T, U>(map) {}
        Series(const std::unordered_map<U, T>& map)
            : BaseSeries<T, U>(map) {}

        const T& operator[](const U& index) const
        {
            return BaseSeries<T, U>::get_index_by_U_(index);
        };

        T& operator[](const U& index)
        {
            return BaseSeries<T, U>::get_index_by_U_(index);
        };

        const T& operator[](const int index) const
        {
            return BaseSeries<T, U>::get_index_by_int_(index);
        };

        T& operator[](const int index)
        {
            return BaseSeries<T, U>::get_index_by_int_(index);
        };
    };

    template <typename T>
    class Series<T, None> : public BaseSeries<T, None> {
    public:
        Series(const std::vector<T>& data)
            : BaseSeries<T>(data) {}
        Series(const std::vector<T>& data, const RangeIndex& index)
            : BaseSeries<T, None>(data, index) {};

        const T& operator[](const int index) const
        {
            return BaseSeries<T, None>::get_index_by_int_(index);
        };

        T& operator[](const int index)
        {
            return BaseSeries<T, None>::get_index_by_int_(index);
        };
    };

    template <typename T>
    class Series<T, int> : public BaseSeries<T, int> {
    public:
        Series(const std::vector<T>& data, const Index<int>& index)
            : BaseSeries<T, int>(data, index) {}

        const T& operator[](const int index) const
        {
            return BaseSeries<T, int>::get_index_by_int_(index);
        };

        T& operator[](const int index)
        {
            return BaseSeries<T, int>::get_index_by_int_(index);
        };
    };

    template <typename T, typename U>
    BaseSeries<T, U>::BaseSeries(const std::map<U, T>& map) :
        index_(std::make_unique<Index<U> >(std::vector<U>(map.size())))
    {
        init_map_(map);
    }

    template <typename T, typename U>
    BaseSeries<T, U>::BaseSeries(const std::unordered_map<U, T>& map) :
        index_(std::make_unique<Index<U> >(std::vector<U>(map.size())))
    {
        init_map_(map);
    }

    template <typename T, typename U>
    const T& BaseSeries<T, U>::get_index_by_int_(const int index) const
    {
        if (index % index_->get_step_() != 0) {
            throw std::out_of_range("Out of range");
        }

        return data_.at((index - index_->get_start_()) / index_->get_step_());
    }

    template <typename T, typename U>
    T& BaseSeries<T, U>::get_index_by_int_(const int index)
    {
        return const_cast<T&>(static_cast<const BaseSeries&>(*this)
                              .get_index_by_int_(index));
    }

    template <typename T, typename U>
    const T& BaseSeries<T, U>::get_index_by_U_(const U& index) const
    {
        auto it = std::find(index_->get_index_().begin(),
                            index_->get_index_().end(),
                            index);

        if (it == index_->get_index_().end()) {
            throw std::out_of_range("Out of range");
        }

        return data_[std::distance(index_->get_index_().begin(), it)];
    }

    template <typename T, typename U>
    T& BaseSeries<T, U>::get_index_by_U_(const U& index)
    {
        return const_cast<T&>(static_cast<const BaseSeries&>(*this)
                              .get_index_by_U_(index));
    }

    template <typename T, typename U>
    template <typename MAP>
    void BaseSeries<T, U>::init_map_(const MAP& map)
    {
        data_.resize(map.size());

        for (const auto &i : map) {
            index_->get_index_().push_back(i.first);
            data_.push_back(i.second);
        }
    }

    template <typename T, typename U>
    void BaseSeries<T, U>::assert_invariants_()
    {
        index_->assert_invariants_(data_.size());
    }

    template <typename T, typename U>
    const std::vector<T>& BaseSeries<T, U>::data() const
    {
        return data_;
    }
}

#endif
