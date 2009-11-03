#ifndef CONTESTTIMER_H
#define CONTESTTIMER_H

#include <QObject>
#include <QTime>

class ContestTimer : public QObject
{
	Q_OBJECT

	QTime m_time_tracker;
	unsigned int m_interval, m_duration;
	int m_timer_id;
public:
	ContestTimer( unsigned int interval );
	void start();
	void stop();

	bool isRunning() { return m_timer_id > 0; }
	void setInterval(unsigned int interval);
	void setDuration(unsigned int duration);
protected:
	virtual void timerEvent(QTimerEvent *event);
signals:
	void timeUpdate(unsigned int msec);
};

#endif // CONTESTTIMER_H
