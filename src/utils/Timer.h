#pragma once

#include <GLFW/glfw3.h>

namespace utils {

  class Timer {
	private:
	  double startTime;
	public:
	  /**
	   * Constructs an object that acts as a timer
	   */
	  Timer();

	  /**
	   * Resets the timers starting time
	   */
	  void reset();

	  // Getter
	  inline double elapsed() const { return glfwGetTime() - startTime; }
  };

} 
