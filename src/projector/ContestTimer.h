#ifndef PROJECTOR_CONTESTTIMER_H
#define PROJECTOR_CONTESTTIMER_H

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

#include <QObject>
#include <QTime>
#include <algorithm>

class ContestTimer : public QObject
{
	Q_OBJECT

	QTime m_time_tracker;
	unsigned int m_interval, m_duration, m_offset;
	int m_timer_id;
public:
	ContestTimer( unsigned int interval );
	void start();
	void restart();
	void restart(unsigned int duration);
	void stop();
	void pause();

	bool isRunning() const { return m_timer_id > 0; }
	void setInterval(unsigned int interval);
	/*!
	  \param duration The duration of the timer in miliseconds.
	  */
	void setDuration(unsigned int duration);
	int timeLeft() const {
		if ( isRunning() )
			return std::max(0, (int)m_duration - (int)(m_time_tracker.elapsed() + m_offset));
		return std::max(0, (int)m_duration - (int)m_offset);
	}

protected:
	virtual void timerEvent(QTimerEvent *event);
signals:
	void timeUpdate(unsigned int msec);
};

#endif // CONTESTTIMER_H
