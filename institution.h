#pragma once

#include "config.h"

#include <thread>
#include <atomic>
#include <shared_mutex>
#include <mutex>
#include <list>
#include <map>


namespace institution {

    class Institution {
        using VisitId = std::size_t;
        using StartEndPair = std::pair<std::chrono::time_point<config::Clock>, std::chrono::time_point<config::Clock>>;

    public:
        explicit Institution();

        ~Institution();

        void connect();

        void disconnect() noexcept;

    private:
        mutable std::shared_mutex _visitors_per_tick_mutex;
        std::atomic<bool> _heartbeat_worker_is_running;
        std::thread _heartbeat_statistics_worker;
        std::list<std::size_t> _visitors_per_tick;

        mutable std::shared_mutex _visitors_mutex;
        std::map<std::thread::id, VisitId> _current_visitors;
        std::map<std::thread::id, std::list<VisitId> > _visitors_log;

        mutable std::shared_mutex _visits_mutex;
        std::map<VisitId, StartEndPair> _visits_log;

        std::atomic<std::size_t> _last_id{0};

        void start_session(std::thread::id visitor_id);

        void end_session(std::thread::id visitor_id) noexcept;

        void heartbeat_worker_task();

        void calculate_all_statistics() const;

    };

} // namespace institution
