// --------------------------------------------------------------------------------------------
// C++11 any_ptr class  -  C. Sullivan
// A type erasure smart pointer that can be cast at run-time to the appropriate type
// Extended from Cassio Neri's implementation with boost::shared_ptr
// His original article can be found at http://drdobbs.com/cpp/229401004
// --------------------------------------------------------------------------------------------

#ifndef _ANY_PTR_H_
#define _ANY_PTR_H_

#include <memory>
#include <algorithm>

// Need to use std::__shared_ptr
// (std::shared_ptr base class)
// for access private member _M_ptr
#define shared_ptr __shared_ptr

class any_ptr;
namespace { class dummy; }
namespace std {

	// A dummy specialization of std::shared_ptr<dummy> that exposes
	// std::shared_ptr<T>'s private member _M_ptr so that it can be cast.
	// This avoids problems with multiple inheritence when casting a pointer
	// might change the pointed-to address. This way the internal pointer
	// is cast so that there is no difference in address between U* p in the catch
	// and the returned result. See below in any_ptr::operator shared_ptr<T>()
	template <>
	class shared_ptr<dummy> {

		friend class ::any_ptr;

		// This class isn't meant to be constructed.
		shared_ptr();

		template <typename T>
		static void cast_stored_ptr(shared_ptr<T>& orig, void* ptr) {
			orig._M_ptr = static_cast<T*>(ptr);
		}
	};
}

//------------------------------------------------------------------------------
// A smart pointer to any type.
//
// This class wrapps a std::shared_ptr<void> and makes type annotation
// allowing for safe run-time casts.
//------------------------------------------------------------------------------
class any_ptr {

	std::shared_ptr<void> ptr_;
	void (any_ptr::*thr_)() const;

	template <typename T>
	void thrower() const { throw static_cast<T*>(ptr_.get()); }

public:

	// Default constructor builds a NULL any_ptr.
	any_ptr() : thr_(0) {}

	// Constructor taking a T*.
	template <typename T>
	explicit any_ptr(T* ptr)
		: ptr_(ptr), thr_(&any_ptr::thrower<T>) {}

	// Constructor taking a std::shared_ptr<T>.
	template <typename T>
	explicit any_ptr(std::shared_ptr<T> ptr)
		: ptr_(ptr),
		  thr_(&any_ptr::thrower<T>) {}

	// Automatic conversion to std::shared_ptr<T>.
	template <typename U>
	operator std::shared_ptr<U>() const {

		if (ptr_) {

			try { (this->*thr_)(); }

			catch (U* p) {
			        std::shared_ptr<U> result = std::static_pointer_cast<U>(ptr_);
			        std::shared_ptr<dummy>::cast_stored_ptr(result, p);
			        return result;
			}

			catch (...) {}
		}

		return std::shared_ptr<U>();
	}

	// Automatic conversion to bool.
	operator bool() const { return ptr_ != nullptr; }

	// Non throwing swap.
	void swap(any_ptr& other) /* throw() */ {
		ptr_.swap(other.ptr_);
		std::swap(thr_, other.thr_);
	}

};

// Utilities

namespace std {

	// Specialization of std::swap.
	template <>
	void swap<any_ptr>(any_ptr& a, any_ptr& b) {
		a.swap(b);
	}
}

template<typename T, typename... Args>
any_ptr make_any(Args&& ...args) {
	return any_ptr( new T( std::forward<Args>(args)... ) );
}

template<typename T, typename U>
any_ptr make_any(std::initializer_list<U> list) {
	return any_ptr( new T( list ) );
}

#endif // _ANY_PTR_H
