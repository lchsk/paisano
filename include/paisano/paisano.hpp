#ifndef PAISANO_HPP
#define PAISANO_HPP

#include <vector>

namespace paisano {

	template <typename T>
	class Series {
	public:
		Series(const std::vector<T>& data);

	private:
		std::vector<T> data_;
	};

	template <typename T>
	Series<T>::Series(const std::vector<T>& data) :
		data_(data)
	{
	}
}

#endif
