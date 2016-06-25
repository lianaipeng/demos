#include "log.h"

void log_init(const std::string& prefix, const log_level& level){
	logging::add_file_log(
		 keywords::file_name = prefix+"_%Y-%m-%d.log",                                        
		 keywords::open_mode = (std::ios::out | std::ios::app),
		 keywords::rotation_size = 10 * 1024 * 1024,                                   
		 keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), 
		 keywords::format = "[%TimeStamp%]:%Message%",
		 keywords::auto_flush = true
		);

	logging::core::get()->set_filter(
		 /*logging::trivial::severity >= logging::trivial::info*/
		 logging::trivial::severity >= level
		);
	logging::add_common_attributes();

	std::cout << "###################" << std::endl;
}
