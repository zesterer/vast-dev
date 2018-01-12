#ifndef VAST_HPP_UTIL_RESULT
#define VAST_HPP_UTIL_RESULT

// Local
#include <util/panic.hpp>

// Std
#include <type_traits>
#include <variant>

namespace vast::util
{
	#define USE_RESULT __attribute__((warn_unused_result))

	//template <typename U> void _murder(U& obj) { obj.~U(); }
	//template <typename U> void _murder(U obj) {}
	//void _murder(char obj) {}

	template <typename T, typename E>
	struct Result
	{
		bool _valid;
		std::variant<T, E> _data;

		bool is_success() const { return this->_data.index() == 0; }
		bool is_failure() const { return this->_data.index() != 0; }

		T const& get_data(const char* file = __FILE__, int line = __LINE__) const
		{
			if (!this->is_success())
				panic("Attempted to access data from failed result", file, line);
			else
				return std::get<T>(this->_data);
		}

		E const& get_error(const char* file = __FILE__, int line = __LINE__) const
		{
			if (this->is_success())
				panic("Attempted to access erro from successful result", file, line);
			else
				return std::get<E>(this->_data);
		}

		T const& except(std::string const& msg, const char* file = __FILE__, int line = __LINE__) const
		{
			if (!this->is_success())
				panic(msg, file, line);
			else
				return std::get<T>(this->_data);
		}

		void ignore() const {}

		static Result<T, E> success(T data = T())
		{
			Result<T, E> r(true);
			r._data = data;
			return r;
		}

		static Result<T, E> failure(E error = E())
		{
			Result<T, E> r(false);
			r._data = error;
			return r;
		}

		Result(bool valid) : _valid(valid) {}
	};

	template <typename E>
	using Status = Result<char, E>;
}

#endif
