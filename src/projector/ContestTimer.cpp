#include "ContestTimer.h"
#include <algorithm>

ContestTimer::ContestTimer( unsigned int interval ) :
		m_interval(interval), m_duration(0), m_timer_id(0) {
}

void ContestTimer::start() {
	if ( isRunning() )return;
	m_time_tracker.start();
	m_timer_id = startTimer(m_interval);
}

void ContestTimer::stop() {
	if ( !isRunning() ) return;
	killTimer(m_timer_id);
	m_timer_id = 0;
}

void ContestTimer::setInterval(unsigned int interval) {
	m_interval = interval;
	if ( isRunning() ) {
		killTimer(m_timer_id);
		m_timer_id = startTimer(m_interval);
	}
}

void ContestTimer::setDuration(unsigned int duration) {
	m_duration = duration;
}

void ContestTimer::timerEvent(QTimerEvent *event) {
	const int timeLeft = std::max(0, (int)m_duration - m_time_tracker.elapsed());
	emit timeUpdate(timeLeft);
	if ( timeLeft == 0 )
		stop();
}
