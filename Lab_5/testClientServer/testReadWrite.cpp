#include <boost/test/unit_test.hpp> 
#include "../ClientServerHead/ClientServerHead.h"
#include <thread>
#include <mutex>
#include <vector>

BOOST_AUTO_TEST_CASE(testRWlock)
{
    ReadWriteLock lock("TestSem", "TestMut");

    lock.startRead();
    lock.endRead();

    lock.startWrite();
    lock.endWrite();

    BOOST_CHECK(true); 
}

BOOST_AUTO_TEST_CASE(testParRead)
{
    ReadWriteLock lock("TestSem2", "TestMut2");

    int counter = 0;
    std::mutex m;

    auto reader = [&]() {
        lock.startRead();
        {
            std::lock_guard<std::mutex> g(m);
            counter++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        lock.endRead();
    };

    std::thread t1(reader);
    std::thread t2(reader);
    t1.join(); t2.join();

    BOOST_CHECK_EQUAL(counter, 2);
}

BOOST_AUTO_TEST_CASE(testWBlocks)
{
    ReadWriteLock lock("Sem3", "Mut3");
    bool writerStarted = false;

    std::thread writer([&]() {
        lock.startWrite();
        writerStarted = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        lock.endWrite();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    bool readerEntered = false;
    std::thread reader([&]() {
        lock.startRead();
        readerEntered = true;
        lock.endRead();
    });

    writer.join();
    reader.join();

    BOOST_CHECK(writerStarted);
    BOOST_CHECK(readerEntered);
}
