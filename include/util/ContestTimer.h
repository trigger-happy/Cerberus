#ifndef UTILS_CONTESTTIMER_H
#define UTILS_CONTESTTIMER_H

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

/**
 * \brief Countdown timer that reports the current remaining time at set intervals.
 *
 * \note All units used in this timer are in milliseconds
 */
class ContestTimer : public QObject
{
	Q_OBJECT

	QTime m_time_tracker;
	unsigned int m_interval, m_duration, m_offset;
	int m_timer_id;
public:
	///Constant for time of indefinite length
	static const unsigned int INDEFINITE;

	/** Constructs the timer reporting at specified interval
	  * \param interval Reporting interval in milliseconds */
	ContestTimer( unsigned int interval );

	/** Starts the timer */
	void start();
	/** Restarts the timer, starting countdown at full duration */
	void restart();
	/** Restarts the timer and sets the duration.
	  * Use if the timer needs to be restarted with a different duration (like a time update)/
	  * \param interval Reporting interval in milliseconds */
	void restart(unsigned int duration);
	/** Stops the timer. Starting again will start the timer from full duration */
	void stop();
	/** Pauses the timer. Resumes upon calling start() */
	void pause();

	bool isRunning() const { return m_timer_id > 0; }
	void setInterval(unsigned int interval);
	/**
	  \param duration The duration of the timer in miliseconds.
	  */
	void setDuration(unsigned int duration);

	/** Time left on the timer, identical to the one reported by the timeUpdate signal */
	int timeLeft() const {
		if ( isRunning() )
			return std::max(0, (int)m_duration - (int)(m_time_tracker.elapsed() + m_offset));
		return std::max(0, (int)m_duration - (int)m_offset);
	}

protected:
	virtual void timerEvent(QTimerEvent *event);
signals:
	/** Signaled at specified interval reporting the number of milliseconds left on the timer. */
	void timeUpdate(unsigned int msec);
};

#endif // UTILS_CONTESTTIMER_H
