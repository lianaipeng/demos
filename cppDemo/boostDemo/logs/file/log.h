#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/support/date_time.hpp>

#include <boost/log/core/record.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>


namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;

namespace attributes     = boost::log::attributes;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;

enum log_level
{
	trace = 0,
	debug,
	info,
	warning,
	error,
	fatal
};
#define FILE_T __FILE__
#define BLT BOOST_LOG_TRIVIAL(debug)
#define BLT_D BOOST_LOG_TRIVIAL(debug)<<"("<<FILE_T<<" +"<<__LINE__<<")["<<__FUNCTION__<<"] "
#define BLT_I BOOST_LOG_TRIVIAL(info)<<"("<<FILE_T<<" +"<<__LINE__<<")["<<__FUNCTION__<<"] "
#define BLT_W BOOST_LOG_TRIVIAL(warning)<<"("<<FILE_T<<" +"<<__LINE__<<")["<<__FUNCTION__<<"] "
#define BLT_E BOOST_LOG_TRIVIAL(error)<<"("<<FILE_T<<" +"<<__LINE__<<")["<<__FUNCTION__<<"] "

void log_init(const std::string& prefix, const log_level& level);
