// EPOS Time Declarations

#ifndef __time_h
#define __time_h

#include <architecture.h>
#include <utility/queue.h>
#include <utility/handler.h>
#include <machine/rtc.h>
#include <machine/ic.h>
#include <machine/timer.h>

__BEGIN_SYS

class Clock
{
public:
    typedef RTC::Microsecond Microsecond;
    typedef RTC::Second Second;
    typedef RTC::Date Date;

public:
    Clock() {}

    Microsecond resolution() { return 1000000; }

    Second now() { return RTC::seconds_since_epoch(); }

    Date date() { return RTC::date(); }
    void date(const Date & d) { return RTC::date(d); }
};


class Alarm
{
    friend class System;
    friend class Alarm_Chronometer;
    friend class Periodic_Thread;
    friend class Scheduling_Criteria::FCFS;

private:
    typedef Timer::Tick Tick;
    typedef Relative_Queue<Alarm, Tick> Queue;

public:
    typedef TSC::Hertz Hertz;
    typedef RTC::Microsecond Microsecond;

    // Infinite times (for alarms)
    enum { INFINITE = RTC::INFINITE };

public:
    Alarm(const Microsecond & time, Handler * handler, int times = 1);
    ~Alarm();

    const Microsecond & period() const { return _time; }
    void period(const Microsecond & p);

    void reset();

    static Hertz frequency() { return _timer->frequency(); }

    static void delay(const Microsecond & time);

private:
    static void init();

    static Microsecond timer_period() { return 1000000 / frequency(); }
    static Tick ticks(const Microsecond & time) { return (time + timer_period() / 2) / timer_period(); }

    static void lock();
    static void unlock();

    static void handler(const IC::Interrupt_Id & i);

private:
    Microsecond _time;
    Handler * _handler;
    int _times;
    Tick _ticks;
    Queue::Element _link;

    static Alarm_Timer * _timer;
    static volatile Tick _elapsed;
    static Queue _request;
};


class Delay
{
private:
    typedef RTC::Microsecond Microsecond;

public:
    Delay(const Microsecond & time): _time(time)  { Alarm::delay(_time); }

private:
    Microsecond _time;
};


class TSC_Chronometer
{
private:
    typedef TSC::Time_Stamp Time_Stamp;

public:
    typedef TSC::Hertz Hertz;
    typedef RTC::Microsecond Microsecond;

public:
    TSC_Chronometer() : _start(0), _stop(0) {}

    Hertz frequency() { return tsc.frequency(); }

    void reset() { _start = 0; _stop = 0; }
    void start() { if(_start == 0) _start = tsc.time_stamp(); }
    void lap() { if(_start != 0) _stop = tsc.time_stamp(); }
    void stop() { lap(); }

    Microsecond read() { return ticks() * 1000000 / frequency(); }

private:
    Time_Stamp ticks() {
        if(_start == 0)
            return 0;
        if(_stop == 0)
            return tsc.time_stamp() - _start;
        return _stop - _start;
    }

private:
    TSC tsc;
    Time_Stamp _start;
    Time_Stamp _stop;
};


class Alarm_Chronometer
{
private:
    typedef Alarm::Tick Time_Stamp;

public:
    typedef TSC::Hertz Hertz;
    typedef RTC::Microsecond Microsecond;

public:
    Alarm_Chronometer() : _start(0), _stop(0) {}

    Hertz frequency() { return Alarm::frequency(); }

    void reset() { _start = 0; _stop = 0; }
    void start() { if(_start == 0) _start = Alarm::_elapsed; }
    void lap() { if(_start != 0) _stop = Alarm::_elapsed; }
    void stop() { lap(); }

    // The parenthesis reduces precision even more, but avoids overflow
    // Casting to LARGER<Ticks> would provide resolution for intermediate calculations, but it is very inefficient on most microcontrollers
    Microsecond read() { return ticks() * (1000000 / frequency()); }

private:
    Time_Stamp ticks() {
        if(_start == 0)
            return 0;
        if(_stop == 0)
            return Alarm::_elapsed - _start;
        return _stop - _start;
    }

private:
    TSC tsc;
    Time_Stamp _start;
    Time_Stamp _stop;
};

class Chronometer: public IF<Traits<TSC>::enabled, TSC_Chronometer, Alarm_Chronometer>::Result {};

__END_SYS

#endif
