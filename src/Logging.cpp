#include "Logging.hpp"
#include <fstream>
#include "Console.hpp"
#include "Globals.hpp"

namespace
{
	std::ofstream log_output;
	std::ofstream log_all_output;
	std::ofstream error_output;
	std::ofstream error_all_output;
}

bool NLogging::Initiate()
{
	log_output.open(NGlobals::dll_folder_path + "log.log", std::ios_base::out | std::ios_base::trunc);
	log_all_output.open(NGlobals::dll_folder_path + "log_all.log", std::ios_base::out | std::ios_base::app);
	error_output.open(NGlobals::dll_folder_path + "error.log", std::ios_base::out | std::ios_base::trunc);
	error_all_output.open(NGlobals::dll_folder_path + "error_all.log", std::ios_base::out | std::ios_base::app);

	return log_output.good()
		&& log_all_output.good()
		&& error_output.good()
		&& error_all_output.good();
}

void NLogging::Terminate()
{
	log_output.close();
	log_all_output.close();
	error_output.close();
	error_all_output.close();
}
