
/*
Copyright (C) 2009 Wilhansen Li

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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
