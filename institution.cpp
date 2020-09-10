#include "institution.h"

#include <functional>
#include <iostream>
#include <string>


namespace institution {

    Institution::Institution() :
            _heartbeat_worker_is_running{true},
            _heartbeat_statistics_worker{std::bind(&Institution::heartbeat_worker_task, this)} {
    }


    Institution::~Institution() {
        _heartbeat_worker_is_running.store(false);
        _heartbeat_statistics_worker.join();
        calculate_all_statistics();
    }


    void Institution::connect() {
        auto visitor_id = std::this_thread::get_id();
        start_session(visitor_id);
    }


    void Institution::disconnect() noexcept {
        auto visitor_id = std::this_thread::get_id();
        end_session(visitor_id);
    }


    void Institution::start_session(std::thread::id visitor_id) {
        auto start_time = config::Clock::now();
        auto visit_id = ++_last_id;
        {
            std::unique_lock lock(_visitors_mutex);

            if (_current_visitors.count(visitor_id)) {
                throw std::logic_error("one thread start session twice");
            }

            _current_visitors.insert({visitor_id, visit_id});

            if (_visitors_log.count(visitor_id)) {
                auto visitor_log_iter = _visitors_log.find(visitor_id);
                visitor_log_iter->second.emplace_back(visit_id);
            } else {
                std::list new_list = {visit_id};
                _visitors_log.emplace(visitor_id, std::move(new_list));
            }
        }
        std::unique_lock lock{_visits_mutex};
        _visits_log.insert({visit_id, StartEndPair{start_time, start_time}});
    }


    void Institution::end_session(std::thread::id visitor_id) noexcept {
        auto end_time = config::Clock::now();
        VisitId current_id;
        {
            std::unique_lock lock(_visitors_mutex);

            auto visitor_session = _current_visitors.find(visitor_id);
            if (visitor_session == _current_visitors.end()) {
                return; // by noexcept
            }
            current_id = visitor_session->second;
            _current_visitors.erase(visitor_session);
        }

        std::unique_lock duration_lock(_visits_mutex);
        auto log_value = _visits_log.find(current_id);
        log_value->second.second = end_time;
    }


    void Institution::heartbeat_worker_task() {
        while (_heartbeat_worker_is_running) {
            std::this_thread::sleep_until(
                    config::Clock::now() + config::institution::BRIEF_STATISTICS_CALCULATION_INTERVAL);

            std::size_t current_visitors_count;
            {
                std::shared_lock lock{_visitors_mutex};
                current_visitors_count = _current_visitors.size();
            }
            std::unique_lock lock{_visitors_per_tick_mutex};
            _visitors_per_tick.emplace_back(current_visitors_count);
        }
    };


    void Institution::calculate_all_statistics() const {
        std::size_t all_visits_count;
        std::size_t all_visitors_count;
        std::chrono::milliseconds all_durations{0};
        {
            std::shared_lock lock{_visitors_mutex};
            all_visits_count = _visits_log.size();
            all_visitors_count = _visitors_log.size();
            for (auto &visit_value : _visits_log) {
                auto current_duration = visit_value.second.second - visit_value.second.first;;
                all_durations += std::chrono::duration_cast<std::chrono::milliseconds>(current_duration);
            }
        }
        auto average_duration = all_durations / all_visits_count;

        std::cout << "all visits count: " << std::to_string(all_visits_count) << std::endl;
        std::cout << "unique visitors count: " << std::to_string(all_visitors_count) << std::endl;
        std::cout << "average visit duration: " << std::to_string(average_duration.count()) << " ms" << std::endl;
        std::size_t tick_number = 0;
        std::shared_lock lock{_visitors_per_tick_mutex};
        for (auto visitors_per_one_tick: _visitors_per_tick) {
            std::cout << "visitor in " << std::to_string(++tick_number) << " tick: "
                      << std::to_string(visitors_per_one_tick)
                      << std::endl;
        }
    }

} //namespace institution
