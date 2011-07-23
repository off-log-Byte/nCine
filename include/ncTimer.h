#ifndef CLASS_NCTIMER
#define CLASS_NCTIMER

#include <sys/time.h>

/// Basic timer and synchronization class
class ncTimer
{
private:
	/// Init time mark
	/*! It is needed in order not to overflow when returning milliseconds since epoch */
	struct timeval m_initTv;
protected:
	/// Is the timer running?
	bool m_bRunning;
	/// Start time mark
	unsigned long int m_uStartTime;
	/// Stop time mark
	unsigned long int m_uStopTime;
	/// Total elapsed time
	unsigned long int m_uTotal;
public:
	/// Empty constructor
	ncTimer();
	// Reset the timer
	void Reset();
	// Start the timer
	void Start();
	// Stop the timer without resetting it
	void Stop();
	// Resume the timer
	void Continue();
	// Return (stop - start) or (now-start) time interval
	unsigned long int Interval();
	// Return total elapsed time
	unsigned long int Total();
	// Return current time
	unsigned long int Now();
};

#endif
