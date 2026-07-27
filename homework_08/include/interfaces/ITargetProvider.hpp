#pragma once
#include <vector>
#include "Types.hpp"

namespace homework_08 {

    class ITargetProvider {
        public:
        virtual int getTargetsCount() = 0;
        virtual std::vector<Target> getTargetPosition() = 0;
        virtual ~ITargetProvider() {};
    };

}
    