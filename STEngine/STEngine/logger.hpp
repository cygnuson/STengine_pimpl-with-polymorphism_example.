#ifndef LOGGER_HPP
#define LOGGER_HPP



#include <iostream>
#include <memory>

#include "stringer.hpp"
#if defined(STRINGER_HPP)

namespace cg{

class testclass;

class logger
{
public:
	/**Logger levels are intended to be bitwise or`d*/
	enum level : unsigned int
	{
		none = 0,
		note1 = 2, /*For the least important notes.*/
		note2 = 4, /* for the almost important notes.*/
		note3 = 8, /*For the important notes.*/
		warning = 16,
		error = 32,
		error_warning = error | warning,
		important_only = note3 | error | warning, /*exclude note1 and note2*/
		notes = note1 | note2 | note3,
		all = note1 | note2 | note3 | warning | error,
	};
private:
	static uint32_t      sm_current_level;
	static std::ostream* sm_log_stream;
	static bool	         sm_is_initialized;
	const static char    sm_error_label[7];
	const static char    sm_warning_label[7];
	const static char    sm_note1_label[7];
	const static char    sm_note2_label[7];
	const static char    sm_note3_label[7];

	static bool sanity_check();

	template<typename T>
	static void log_helper(T&& t);
	template<typename T, typename...Args>
	static void log_helper(T&& t, Args&&...args);

public:
	/**Initialize the logging system.  Use bitwise OR to have multiple levels.
	EG: init(level::note | level::error,&std::cout)*/
	static void init(uint32_t level, std::ostream &logStrm);
	/**Log any amount of things to the log. ALL objects logged must be one of
	the following:
	A fundamental type,
	A std::string (or convertible to std::string EG. const char*),
	A type that passes the is_stringable<type> test.
	\sa cg::serialization::is_stringable
	*/
	template<typename...Args>
	static void log(logger::level level, Args&&...args);

	template<typename...Types>
	static void log_note(int l, Types&&...types);

	template<typename...Types>
	static void log_error(Types&&...types);

	template<typename...Types>
	static void log_warning(Types&&...types);
};

template<typename T>
void logger::log_helper(T&& t)
{
	*sm_log_stream << cg::stringer::to_string(std::forward<T>(t));
}
template<typename T, typename...Args>
void logger::log_helper(T&& t, Args&&...args)
{
	*sm_log_stream << cg::stringer::to_string(t);
	log_helper(std::forward<Args>(args)...);
}
template<typename...Args>
void logger::log(logger::level level, Args&&...args)
{
	if(!sanity_check())
	{
		/*if the logger is not in a sane state, do nothing.*/
		return;
	}
	if((level & sm_current_level) > 0)
	{
		switch(level)
		{
		case logger::level::warning:
			*sm_log_stream << sm_warning_label;
			break;
		case logger::level::error:
			*sm_log_stream << sm_error_label;
			break;
		case logger::level::note1:
			*sm_log_stream << sm_note1_label;
			break;
		case logger::level::note2:
			*sm_log_stream << sm_note2_label;
			break;
		case logger::level::note3:
			*sm_log_stream << sm_note3_label;
			break;
		default:
			*sm_log_stream << "Invalid Level:";
			break;
		}
		log_helper(std::forward<Args>(args)...);
		*sm_log_stream << std::endl;
	}
}

template<typename...Types>
void logger::log_note(int l, Types&&...types)
{
	switch(l)
	{
	case 1:
		log(level::note1,std::forward<Types>(types)...);
		break;
	case 2:
		log(level::note2,std::forward<Types>(types)...);
		break;
	case 3:
		log(level::note3,std::forward<Types>(types)...);
		break;
	default:
		log(level::error, "Tried to log an invalid note-level.");
	}


}
template<typename...Types>
void logger::log_error(Types&&...types)
{
	log(level::error,std::forward<Types>(types)...);
}
template<typename...Types>
void logger::log_warning(Types&&...types)
{
	log(level::warning,std::forward<Types>(types)...);
}



}
#endif // STRINGER_HPP
#endif // LOGGER_HPP




























