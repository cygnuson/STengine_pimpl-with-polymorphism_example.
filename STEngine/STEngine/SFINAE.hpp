#ifndef SFINAE_HPP
#define SFINAE_HPP
/**A Static checker to make sure that states passed into the SFMLApplication
have the correct fucntions.  The true type for when the class T has the correct
functions.*/
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
/**A Static checker to make sure that states passed into the SFMLApplication
have the correct fucntions.  The false type for when class T does not have
the correct functions.*/
template<typename T>
static inline std::false_type CanBeStateImpl(...) {};
/**The front end for the CanBeState implementation.*/
template<typename T>
struct CanBeState : decltype(CanBeStateImpl<T>(int{}))
{};


#endif //SFINAE_HPP