#include "test.h"

#include <thread>
#include <random>


std::chrono::milliseconds get_random_interval(std::size_t minimum, std::size_t maximum) {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    std::chrono::milliseconds time{minimum + (mt() % (maximum - minimum))};
    return time;
}


namespace visitor {

    Visitor::Visitor(institution::Institution &target) :
            _target{target} {
        target.connect();
    }


    Visitor::~Visitor() {
        _target.disconnect();
    }


    void visitor_session(institution::Institution &target) {
        Visitor visitor{target};
        std::this_thread::sleep_until(
                config::Clock::now() +
                get_random_interval(config::test::MINIMAL_DISCONNECT_INTERVAL,
                                    config::test::MAXIMUM_DISCONNECT_INTERVAL));
    }

} // namespace visitor


void test(std::chrono::seconds statistic_range) {
    std::atomic<bool> running{true};

    std::thread shutdown_timer{[&running, statistic_range]() {
        std::this_thread::sleep_until(config::Clock::now() + statistic_range);
        running.store(false);
    }};

    institution::Institution target_subject;
    std::list<std::thread> started_visitors;
    while (running.load()) {
        std::this_thread::sleep_until(
                config::Clock::now() +
                get_random_interval(config::test::MINIMAL_NEW_CONNECTION_INTERVAL,
                                    config::test::MAXIMUM_NEW_CONNECTION_INTERVAL));
        started_visitors.emplace_back(visitor::visitor_session, std::ref(target_subject));
    }

    shutdown_timer.join();
    for (auto &visitor: started_visitors) {
        visitor.join();
    }
}