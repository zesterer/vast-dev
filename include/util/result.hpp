#ifndef VAST_HPP_UTIL_RESULT
#define VAST_HPP_UTIL_RESULT

// Local
#include <util/panic.hpp>

namespace vast::util
{
	#define USE_RESULT __attribute__((warn_unused_result))

	template <typename T, typename E>
	struct Result
	{
		bool _valid;
		union { T _data; E _error; };

		bool is_success() const { return this->_valid; }
		bool is_failure() const { return !this->_valid; }

		T const& get_data(const char* file = __FILE__, int line = __LINE__) const
		{
			if (!this->_valid)
				panic("Attempted to access data from failed result", file, line);
			else
				return this->_data;
		}

		T const& get_error(const char* file = __FILE__, int line = __LINE__) const
		{
			if (this->_valid)
				panic("Attempted to access erro from successful result", file, line);
			else
				return this->_error;
		}

		T const& except(std::string const& msg, const char* file = __FILE__, int line = __LINE__) const
		{
			if (!this->_valid)
				panic(msg, file, line);
			else
				return this->_data;
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
			r._error = error;
			return r;
		}

		Result(bool valid) : _valid(valid) {}
	};

	template <typename E>
	using Status = Result<char, E>;
}

#endif
