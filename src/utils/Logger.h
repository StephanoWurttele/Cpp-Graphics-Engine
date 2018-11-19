#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "Singleton.h"

namespace utils {

  class Logger : public Singleton {
	private:
	  Logger();
	public:
	  static Logger& getInstance();

	  void debug(std::string &module, const std::string &message);

	  void info(const std::string &module, const std::string &message);

	  void warning(const std::string &module, const std::string &message);

	  void error(const std::string &module, const std::string &message);

	private:
	  void logMessage(const int &priority, const std::string &module, const std::string &message);

	  // Clears out the contents all of the different files that have been assigned to
	  void clearFileContents();

	  enum {
		DEBUG, INFO, WARNING, ERROR
	  };

	  std::vector<std::string> filePaths;
  };

}
