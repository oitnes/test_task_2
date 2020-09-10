#pragma once

#include <typeinfo>
#include <chrono>


namespace config {

    using Clock = std::chrono::high_resolution_clock;

    namespace test {
        static constexpr std::size_t MINIMAL_NEW_CONNECTION_INTERVAL{0};
        static constexpr std::size_t MAXIMUM_NEW_CONNECTION_INTERVAL{1000};
        static constexpr std::size_t MINIMAL_DISCONNECT_INTERVAL{20};
        static constexpr std::size_t MAXIMUM_DISCONNECT_INTERVAL{1000};
    }

    namespace institution {
        static constexpr std::chrono::milliseconds BRIEF_STATISTICS_CALCULATION_INTERVAL{1000};
    }
}
