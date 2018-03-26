//======================================================================
/*! \file EventMonitor.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 7, 2015
 *///-------------------------------------------------------------------

#include <ibeosdk/EventMonitor.hpp>

#include <boost/version.hpp>

#include <cassert>

//======================================================================

namespace ibeosdk {

//======================================================================

EventMonitor::EventMonitor()
  : m_events(0),
    m_nextNewEvent(1)
{}

//======================================================================

EventMonitor::Mask EventMonitor::newEvent()
{
	assert(m_nextNewEvent); // can handle max. 8*sizeof(Mask) event bits
	Mask newEventBit = m_nextNewEvent;
	m_nextNewEvent <<= 1;   // becomes zero upon overflow, see assertion above
	return newEventBit;
}

//======================================================================

void EventMonitor::signal (const Mask eventMask, void (*onUnhandledEvent)(Mask)/*=NULL*/)
{
	boost::mutex::scoped_lock criticalSection (m_mtxMonitor);
	if (onUnhandledEvent && (m_events & eventMask))
		(*onUnhandledEvent) (m_events & eventMask);
	m_events |= eventMask;
	if (m_events)
		m_condAnyEvent.notify_all();
}

//======================================================================

EventMonitor::Mask EventMonitor::wait (const Mask eventMask/*=anyEvent*/)
{
	assert (eventMask != 0);
	Mask result = 0;
	if (eventMask != 0) { // prevent endless while-loop
		boost::mutex::scoped_lock criticalSection (m_mtxMonitor);
		while (!(m_events & eventMask))
			m_condAnyEvent.wait (criticalSection);
		result = m_events & eventMask;
		m_events &= ~eventMask;	// reset the signalling event
	}
	return result;
}

//======================================================================

EventMonitor::Mask EventMonitor::wait(const boost::posix_time::time_duration& duration, Mask eventMask)
{
	Mask result = 0;
	assert (eventMask != 0);
	if (eventMask != 0) { // prevent endless while-loop
		boost::mutex::scoped_lock criticalSection (m_mtxMonitor);

#if BOOST_VERSION >= 103700
		const boost::posix_time::time_duration& tmTimeOut = duration;
		// Boost 1.37.0 accepts a time_duration directly, so we better
		// use it here.
#else
		boost::xtime tmTimeOut (ibeo::timeOut (duration));
#endif

		while (!(m_events & eventMask))
			if (!m_condAnyEvent.timed_wait(criticalSection, tmTimeOut))
				return TimeOutEvent;
		result = m_events & eventMask;
		m_events &= ~eventMask;	// reset the signaling event
	} // if
	return result;
}

//======================================================================

} // END ibeosdk

//======================================================================
