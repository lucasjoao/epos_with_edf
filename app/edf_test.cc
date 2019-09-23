#include <utility/ostream.h>
#include <real-time.h>

using namespace EPOS;

OStream cout;

int function_thread01() {
    TSC_Chronometer chronometer;
    do {
        chronometer.reset();
        chronometer.start();
        cout <<  "start thread01" << endl;
        for(; chronometer.read() < 1000000; );
        chronometer.stop();
        cout << "thread01 terminou em " << chronometer.read() << " microseconds" << endl;
        chronometer.reset();
    } while (Periodic_Thread::wait_next());
    return 0;
}
int function_thread02() {
    TSC_Chronometer chronometer;
    do {
        chronometer.reset();
        chronometer.start();
        cout << "start thread02" << endl;
        for(; chronometer.read() < 2000000; );
        chronometer.stop();
        cout << "thread02 terminou em " << chronometer.read() << " microseconds" << endl;
        chronometer.reset();
    } while (Periodic_Thread::wait_next());
    return 0;
}

int function_thread03() {
    TSC_Chronometer chronometer;
    do {
        chronometer.reset();
        chronometer.start();
        cout << "start thread03" << endl;
        for(; chronometer.read() < 3000000; );
        chronometer.stop();
        cout << "thread03 terminou em " << chronometer.read() << " microseconds" << endl;
        chronometer.reset();
    } while (Periodic_Thread::wait_next());
    return 0;
}

int main() {
    // https://microcontrollerslab.com/earliest-deadline-first-scheduling/
    // teste tenta simular exemplo da figura 2 do link
    // lembrando que no exemplo há vários momentos em que a prioridade é a mesma,
    // e com isso a thread escolhida para execução é aleatória

    const unsigned long period01 = 4000000;
    const unsigned long period02 = 6000000;
    const unsigned long period03 = 8000000;

    Periodic_Thread::Configuration config01(period01, 6);
    Periodic_Thread::Configuration config02(period02, 4);
    Periodic_Thread::Configuration config03(period03, 3);

    Periodic_Thread *thread01 = new Periodic_Thread(config01, &function_thread01);
    Periodic_Thread *thread02 = new Periodic_Thread(config02, &function_thread02);
    Periodic_Thread *thread03 = new Periodic_Thread(config03, &function_thread03);

    thread01->join();
    thread02->join();
    thread03->join();

    delete thread01;
    delete thread02;
    delete thread03;
    return 0;
}
