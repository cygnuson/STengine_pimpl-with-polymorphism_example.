#include "logger.hpp"
namespace cg{

uint32_t      logger::_sm_current_level = logger::none;
std::ostream* logger::_sm_log_stream = nullptr;
bool          logger::_sm_is_initialized = false;
const char    logger::_sm_error_label[7]   = "ERROR:";
const char    logger::_sm_warning_label[7] = " WARN:";
const char    logger::_sm_note1_label[7]   = "NOTE1:";
const char    logger::_sm_note2_label[7]   = "NOTE2:";
const char    logger::_sm_note3_label[7]   = "NOTE3:";
std::mutex    logger::_logMutex;

void logger::init(uint32_t level, std::ostream &logStrm)
{
	_sm_current_level = level;
	_sm_log_stream = &logStrm;
	_sm_is_initialized = true;
}

bool logger::sanity_check()
{
	if(!_sm_is_initialized)
	{
		return false;
	}

	return true;
}

}



















