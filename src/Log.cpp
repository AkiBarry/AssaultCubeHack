#include "Log.hpp"
#include <fstream>
#include <Windows.h>
#include "Console.hpp"
#include "Globals.hpp"

namespace
{
	std::ofstream log_output;
	std::ofstream log_all_output;
	std::ofstream error_output;
	std::ofstream error_all_output;
}

bool NLog::Initiate()
{	
	log_output = std::ofstream(NGlobals::dll_file_path);
	log_all_output = std::ofstream(NGlobals::dll_file_path);
	error_output = std::ofstream(NGlobals::dll_file_path);
	error_all_output = std::ofstream(NGlobals::dll_file_path);

	log_output.open("log.log", std::ios_base::out | std::ios_base::trunc);
	log_all_output.open("log_all.log", std::ios_base::out | std::ios_base::app);
	error_output.open("error.log", std::ios_base::out | std::ios_base::trunc);
	error_all_output.open("error_all.log", std::ios_base::out | std::ios_base::app);

	return log_output.good()
		&& log_all_output.good()
		&& error_output.good()
		&& error_all_output.good();
}

void NLog::Terminate()
{
	log_output.close();
	log_all_output.close();
	error_output.close();
	error_all_output.close();
}
