#ifndef STRINGER_HPP
#define STRINGER_HPP

#include <sstream>
#include <string>

namespace cg{ namespace stringer{


/**`value` will be non-zero if the type T has a member function names to_string
*/
template<typename T>
class is_stringable
{
private:
	using real_type = std::remove_reference_t<T>;
	struct valid     {char zero[2];};
	struct not_valid {char zero;};

	template<typename C, std::string (C::*)()>
	struct SFINAE {};

	template<typename C>
	static valid test(SFINAE<C,&C::to_string>*)
	{
		return valid();
	}
	template<typename C>
	static not_valid test(...)
	{
		return not_valid();
	}
public:
	enum
	{
		value = (sizeof(test<real_type>(0)) == sizeof(valid))
	};
};

/**for const char* and strings. This functions will call whenever an object
needs logged that is a std::string or anything that can be implicitly
converted to one.*/
inline std::string
to_string(const std::string& str)
{
	return str;
}


/*******************************************containers with only two elements.*/
template<
	template<typename, typename> class Pair,
	typename A,
	typename B
>
std::string to_string(const Pair<A,B>& p)
{
	std::stringstream ss;
	ss << "(" << p.first << "," << p.second << ")";
	return ss.str();
}
/****************************************************Container with allocator.*/

template<
	typename Type,
	template<typename,typename> class List,
	template<typename> class Alloc
>
std::string to_string(const List<Type,Alloc<Type>>& o)
{
	std::stringstream ss;
	for(const auto& e: o)
	{
		ss << e << ",";
	}
	return ss.str();
}
/********************************************Containers with alloc and compare*/

template<
	template<typename,typename,typename> class List,
	template<typename> class Comp,
	template<typename> class Alloc,
	typename Type
>
std::string to_string(const List<Type,Comp<Type>,Alloc<Type>>& o)
{
	std::stringstream ss;
	for(const auto& e: o)
	{
		ss << e << ",";
	}
	return ss.str();
}
/*************************************************************************Maps*/
template<
	template<typename,typename,typename,typename> class Map,
	typename Key,
	typename Type,
	template<typename> class Comp,
	template<typename> class Alloc,
	template<typename, typename> class Pair
>
std::string to_string(
	const Map<Key,Type,Comp<Key>,Alloc<Pair<const Key,Type>>>& o)
{
	std::stringstream ss;
	for(const auto& e : o)
	{
		ss << to_string(e);
	}
	return ss.str();
}
/*************************************************Unordered sets and multisets*/
template<
	template<typename,typename,typename,typename> class USet,
	typename Key,
	template<typename> class Hasher,
	template<typename> class Pred,
	template<typename> class Alloc
>
std::string to_string(const USet<Key,Hasher<Key>,Pred<Key>,Alloc<Key>>& o)
{
	std::stringstream ss;
	for(const auto& e: o)
	{
		ss << e << ",";
	}
	return ss.str();
}
/****************************************************************Unordered Map*/
template<
	template<typename,typename,typename,typename,typename> class Map,
	typename Key,
	typename Type,
	template<typename> class Hasher,
	template<typename> class Comp,
	template<typename> class Alloc,
	template<typename, typename> class Pair
>
std::string to_string(
	const Map<Key,Type,Hasher<Key>,Comp<Key>,Alloc<Pair<const Key,Type>>>& o)
{
	std::stringstream ss;
	for(const auto& e : o)
	{
		ss << to_string(e);
	}
	return ss.str();

}
/***************************************************************For std::array*/
template<
	template<typename,std::size_t> class Array,
	typename Type,
	std::size_t Size
>
std::string to_string(const Array<Type,Size>& o)
{
	std::stringstream ss;
	for(const auto& e: o)
	{
		ss << e << ",";
	}
	return ss.str();
}
/******************************************************For std::shared_pointer*/
template<template<typename> class Ptr,typename T>
std::string to_string(const Ptr<T>& o)
{
	return to_string(*o);
}
/**For non-strings. This function only calls on types that pass the test
is_stringable<type>. Meaning that it must impliment
std::string to_string(bool)
\sa cg::serialization::is_stringable
*/
template<typename T>
std::enable_if_t<
is_stringable<
std::decay_t<T>>::value,std::string>
to_string(T&& t)
{
	return t.to_string();
}

/**For fundamentals. This function will call whenever a fundamental type that
is not implicitly converible is logged.*/
template<typename T>
std::enable_if_t<
std::is_fundamental<
std::decay_t<T>>::value,std::string>
to_string(T&& t)
{
	return std::to_string(std::forward<T>(t));
}



}}
#endif // STRINGER_HPP


























