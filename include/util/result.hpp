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
		template <typename U> struct _Wrap { U _item; _Wrap(U item) : _item(item) {} };

		bool _valid;
		std::variant<std::monostate, _Wrap<T>, _Wrap<E>> _data;

		bool good() const { return this->_data.index() == 1; }
		bool bad() const { return this->_data.index() != 1; }

		operator bool() const { return this->good(); }

		T const& get_data(const char* file = __FILE__, int line = __LINE__) const
		{
			if (this->bad())
				panic("Attempted to access data from failed result", file, line);
			else
				return std::get<_Wrap<T>>(this->_data)._item;
		}

		T const& operator*() { return this->get_data(); }
		T const* operator->() { return &this->get_data(); }

		E const& get_error(const char* file = __FILE__, int line = __LINE__) const
		{
			if (this->good())
				panic("Attempted to access erro from successful result", file, line);
			else
				return std::get<_Wrap<E>>(this->_data)._item;
		}

		T const& except(std::string const& msg, const char* file = __FILE__, int line = __LINE__) const
		{
			if (this->bad())
				panic(msg, file, line);
			else
				return std::get<_Wrap<T>>(this->_data)._item;
		}

		void ignore() const {}

		static Result<T, E> success(T data = T())
		{
			Result<T, E> r(true);
			r._data = _Wrap<T>(data);
			return r;
		}

		static Result<T, E> failure(E error = E())
		{
			Result<T, E> r(false);
			r._data = _Wrap<E>(error);
			return r;
		}

		Result(bool valid) : _valid(valid) {}
	};

	template <typename E>
	using Status = Result<char, E>;
}

#endif
