#include "logger.hpp"
namespace cg{

uint32_t      logger::sm_current_level = logger::none;
std::ostream* logger::sm_log_stream = nullptr;
bool          logger::sm_is_initialized = false;

const char logger::sm_error_label[7]   = "ERROR:";
const char logger::sm_warning_label[7] = " WARN:";
const char logger::sm_note1_label[7]   = "NOTE1:";
const char logger::sm_note2_label[7]   = "NOTE2:";
const char logger::sm_note3_label[7]   = "NOTE3:";

void logger::init(uint32_t level, std::ostream &logStrm)
{
	sm_current_level = level;
	sm_log_stream = &logStrm;
	sm_is_initialized = true;
}

bool logger::sanity_check()
{
	if(!sm_is_initialized)
	{
		return false;
	}

	return true;
}

}



















