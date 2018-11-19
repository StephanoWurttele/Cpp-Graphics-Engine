#include "Timer.h"

namespace utils {

  Timer::Timer() {
	startTime = glfwGetTime();
  }

  void Timer::reset() {
	startTime = glfwGetTime();
  }

};

