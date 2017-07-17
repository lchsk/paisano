#ifndef PAISANO_HPP
#define PAISANO_HPP

#include <vector>

namespace paisano {

	template <typename T>
	class Series {
	public:
		Series(const std::vector<T>& data);

		const std::vector<T>& data() const;

	private:
		std::vector<T> data_;
	};

	template <typename T>
	Series<T>::Series(const std::vector<T>& data) :
		data_(data)
	{
	}

	template <typename T>
	const std::vector<T>& Series<T>::data() const
	{
		return data_;
	}
}

#endif
