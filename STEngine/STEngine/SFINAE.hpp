#ifndef SFINAE_HPP
#define SFINAE_HPP

template<typename T, typename = decltype(&T::Draw),
	typename = decltype(&T::HandleInput),
	typename = decltype(&T::GetView)
>
static inline std::true_type CanBeStateImpl(int) {};

template<typename T>
static inline std::false_type CanBeStateImpl(...) {};

template<typename T>
struct CanBeState : decltype(CanBeStateImpl<T>(int{}))
{};


#endif //SFINAE_HPP