#ifndef SFINAE_HPP
#define SFINAE_HPP

template<typename T, typename = decltype(&T::Draw),
	typename = decltype(&T::HandleInput),
	typename = decltype(&T::GetView),
	typename = decltype(&T::UpdateLogic),
	typename = decltype(&T::SanityCheck),
	typename = decltype(&T::GetState),
	typename = decltype(&T::Freeze),
	typename = decltype(&T::Unfreeze)
>
static inline std::true_type CanBeStateImpl(int) {};

template<typename T>
static inline std::false_type CanBeStateImpl(...) {};

template<typename T>
struct CanBeState : decltype(CanBeStateImpl<T>(int{}))
{};


#endif //SFINAE_HPP