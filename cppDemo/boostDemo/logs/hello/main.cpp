#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
//#include <boost/log/sources/severity_logger.hpp>
//#include <boost/log/sources/record_ostream.hpp>
//#include <boost/log/utility/setup/file.hpp>
//#include <boost/log/utility/setup/console.hpp>
//#include <boost/log/utility/setup/common_attributes.hpp>
//#include <boost/log/support/date_time.hpp>
//#include <boost/log/core/record.hpp>


namespace logging = boost::log;
//namespace src = boost::log::sources;
//namespace expr = boost::log::expressions;
//namespace keywords = boost::log::keywords;
//namespace sinks = boost::log::sinks;
//namespace attributes     = boost::log::attributes;


void init(){
	logging::core::get()->set_filter(
		 logging::trivial::severity >= logging::trivial::info
		);
}

int main(int, char*[]){
	init();

	BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
	BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
	BOOST_LOG_TRIVIAL(info) << "An informational severity message";
	BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
	BOOST_LOG_TRIVIAL(error) << "An error severity message";
	BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

	return 0;
}
