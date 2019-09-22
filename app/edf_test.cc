#include <utility/ostream.h>
#include <synchronizer.h>
#include <real-time.h>


using namespace EPOS;

OStream cout;

int T1()
{
    TSC_Chronometer chron;
    do {
        chron.reset();
        chron.start();
        cout << "T1 started:" << endl;
        for(;chron.read() < 500000 - 5000;);
        chron.stop();
        cout << "Thread 1: finished in " << chron.read() << " microseconds" << endl;
        chron.reset();
    } while(Periodic_Thread::wait_next());
    return 0;
}
int T2()
{
    TSC_Chronometer chron;
    do {
        chron.reset();
        chron.start();
        cout << "T2 started:" << endl;
        for(;chron.read() < 1000000 - 10000;);
        chron.stop();
        cout << "Thread 2: finished in " << chron.read() << " microseconds" << endl;
        chron.reset();
    } while(Periodic_Thread::wait_next());
    return 0;
}

int T3()
{
    TSC_Chronometer chron;
    do {
        chron.reset();
        chron.start();
        cout << "T3 started:" << endl;
        for(;chron.read() < 2000000 - 20000;);
        chron.stop();
        cout << "Thread 3: finished in " << chron.read() << " microseconds" << endl;
        chron.reset();
    } while(Periodic_Thread::wait_next());
    return 0;
}

int test() {
    TSC_Chronometer chron;
    chron.start();
    for(int i = 0; chron.read() < 20000; i++);
    chron.stop();
    cout << "Thread 3: finished in " << chron.read() << " microseconds" << endl;
    chron.reset();
}
int main()
{
    //Thread *t = new Thread(&test);
    const unsigned long m1 = 3000000;
    Periodic_Thread::Configuration c1(m1, 5);

    const unsigned long m2 = 4000000;
    Periodic_Thread::Configuration c2(m2, 5);

    const unsigned long m3 = 6000000;
    Periodic_Thread::Configuration c3(m3, 5);
    Periodic_Thread *tr1 = new Periodic_Thread(c1, &T1);
    Periodic_Thread *tr2 = new Periodic_Thread(c2, &T2);
    Periodic_Thread *tr3 = new Periodic_Thread(c3, &T3);

    tr1->join();
    tr2->join();
    tr3->join();

    delete tr1;
    delete tr2;
    delete tr3;
   // t->join();
    return 0;
}
