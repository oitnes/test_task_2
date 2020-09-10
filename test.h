#pragma once

#include "institution.h"
#include "config.h"


namespace visitor {

    class Visitor {
    public:
        explicit Visitor(institution::Institution &target);

        ~Visitor();

    private:
        institution::Institution &_target;
    };

    void visitor_session(institution::Institution &target);

} //namespace visitor


void test(std::chrono::seconds statistic_range);