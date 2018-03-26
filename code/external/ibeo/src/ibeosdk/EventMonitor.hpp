//======================================================================
/*! \file EventMonitor.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 7, 2015
 *///-------------------------------------------------------------------

#ifndef IBEOSDKI_EVENTMONITOR_HPP_SEEN
#define IBEOSDKI_EVENTMONITOR_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/inttypes.hpp>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class EventMonitor
 * \brief Thread-safe handler for a group of events.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Apr 8, 2015
 *
 * This class monitors \e signalling and \e waiting of multiple events.
 * It implements the "Monitor Object" programming pattern.
 *
 * A thread can call wait() to wait for one or more events. This will block
 * the thread (consuming no CPU time anymore) until another thread calls
 * signal() for one of these events. As soon as the other thread calls signal()
 * for that event the blocked thread enters the ready-state and both threads
 * continue running.
 *
 * This class extends boost::condition which is &quot;a synchronization primitive
 * used to cause a thread to wait until a particular shared-data condition
 * (or time) is met&quot; (see documentation of boost::condition). In contrast to
 * boost::condition, this class can wait for multiple events similar to
 * \c WaitForMultipleObjects() of the Windows API.
 *
 * An event is represented by one bit out of an UINT32. It is selected for
 * \e signalling if the corresponding event bit is \c true. The EventMonitor
 * may contain up to 32 user-defined bits. The events are \e automatic, i.e.
 * the corresponding bits are buffered and keep their states until they are
 * read, which means that one thread waits for this event bit. After that,
 * they are automatically reset to \e not \e signalled.
 *
 * Since events are used for communication between threads, special care must
 * be taken when accessing events. The EventMonitor provides thread-safe read
 * methods (called wait()) and write methods (called signal()) to modify event
 * bits.
 *
 * This class uses Boost threads for synchronization. Code examples for using
 * Boost threads can be found on Linux system in the folder
 * file:///usr/share/doc/libboost-doc/examples/libs/thread/example .
 *
 * The wait() methods in this class behave similar to the function
 * \c WaitForMultipleObjects() known from the Microsoft Windows API.
 * \c WaitForMultipleObjects() is reimplemented here by means of the
 * Boost library based on the ideas from
 * http://aspn.activestate.com/ASPN/Mail/Message/boost/1155002 .
 *///-------------------------------------------------------------------
class EventMonitor {
public:
	//========================================
	/*!\brief  Data type of an event mask.
	 *
	 * Variables of this type contain a bit mask that represents a
	 * combination of zero or more events. Note that each bit corresponds
	 * to an event.
	 *///-------------------------------------
	typedef uint32_t Mask;

	//========================================
	/*!\brief Enumeration of standard event masks
	 *///-------------------------------------
	enum StandardEventMasks {
		TimeOutEvent = 0 //!< Bit mask for the time-out event
	};

public:
	//========================================
	/*!\brief Default constructor
	 *///-------------------------------------
	EventMonitor();

	//========================================
	/*!\brief Get a new event bit.
	 *
	 * \return The new event bit or 0 if there are no more events available.
	 *         The current implementation supports up to 32 events per
	 *         EventMonitor object.
	 *///-------------------------------------
	Mask newEvent();

	//========================================
	/*!\brief Signal event(s).
	 *
	 * All waiting threads will be notified about the signalled event(s).
	 * All active (non-waiting) threads will detect the event(s) upon their
	 * next wait() call.
	 *
	 * \param eventMask Bit mask of the events to signal. To signal more
	 *                  than one event combine the corresponding bits with a
	 *                  logical OR.
	 * \param onUnhandledEvent
	 *                   optional pointer to a function that is
	 *                   called back if any of the bits in \a eventMask is
	 *                   still set, i.e. if an event has not been handled yet.
	 *                   This feature can be used e.g. to detect real-time
	 *                   violations.
	 *///-------------------------------------
	void signal(const Mask eventMask, void (*onUnhandledEvent)(Mask) = NULL);

	//========================================
	/*!\brief Wait for at least one event.
	 *
	 * If the event has been signalled already since the last wait() call,
	 * thread execution continues immediately. Otherwise, the thread will
	 * wait until one of the specified events is signalled. The event bits
	 * are reset automatically when wait() returns.
	 *
	 * \param eventMask  Bit mask of the expected events. To wait for more
	 *                   than one event combine the corresponding bits using
	 *                   logical OR. value should be non-null.
	 *
	 * \return The bit mask of the events that have waked up the thread.
	 * Multiple events are logically OR'ed in the mask.
	 *
	 * \note Multiple events will be returned if any of the following
	 * conditions is true:
	 * - Multiple events are passed to signal().
	 * - While handling an event, at least two different events are
	 *   signalled by other threads. These new events will be returned
	 *   altogether at the next wait() call (assuming that \a eventMask
	 *   matches).
	 *
	 * \sa wait(const boost::posix_time::time_duration&, Mask)
	 *///-------------------------------------
	Mask wait(const Mask eventMask);

	//========================================
	/*!\brief Wait a limited time for at least one event.
	 *
	 * The same as wait(Mask), but waiting is canceled if none of the
	 * specified events is signaled within the specified duration or
	 * shortly after that, depending on when this thread is scheduled
	 * again by the operating system scheduler.
	 *
	 * \param duration  The approximate time limit for waiting, e.g.
	 *                  \c boost::posix_time::milliseconds(100). Keep
	 *                  in mind this is only a minimum time for waiting.
	 *                  There is no guarantee that this thread will
	 *                  wake up after exactly the time specified. The
	 *                  operating system scheduler might choose to wait
	 *                  much longer than this requested waiting duration
	 *                  to activate this thread again, especially if
	 *                  another thread is still active. That being said,
	 *                  the accuracy for the waiting time on Linux seems
	 *                  to be approx. 1-2 milliseconds, whereas on Windows
	 *                  the accuracy is significantly worse and has been
	 *                  observed to be anything between 3 and 40 milliseconds.*
	 * \param eventMask Bit mask of the events to signal. To wait for more
	 *                  than one event combine the corresponding bits with a
	 *                  logical OR. Value must be non-null.
	 * \return The bit mask of the events that have woken up the thread or
	 *         EventMonitor::timeOutEvent. Multiple events are combined in
	 *         the mask using logical \c OR. See also note at wait(Mask).
	 * \sa wait(Mask)
	 *///-------------------------------------
	Mask wait(const boost::posix_time::time_duration& duration, Mask eventMask);

protected:
	//========================================
	/*!\brief Mutex to implement the monitor concept.
	 *
	 * The so-called \e monitor ensures that each (public) method of this
	 * class is executed only by a single thread at a time.
	 *///-------------------------------------
	boost::mutex m_mtxMonitor;

	//========================================
	/*!\brief Condition to signal any event.
	 *
	 * Threads can wait for events. Signaling this condition wakes them up.
	 * Note that the signal will be detected by \e waiting threads only.
	 *///-------------------------------------
	boost::condition m_condAnyEvent;

	//========================================
	/*!\brief Bit-set, each bit represents one user-defined event.
	 *
	 * The number of events is limited by the word size, currently 32.
	 *///-------------------------------------
	Mask m_events;

	//========================================
	/*!\brief The next event returned by NewEvent().
	 *///-------------------------------------
	Mask m_nextNewEvent;
}; // EventMonitor

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDKI_EVENTMONITOR_HPP_SEEN

//======================================================================
