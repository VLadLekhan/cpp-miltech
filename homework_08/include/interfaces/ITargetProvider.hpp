#pragma once
#include "Types.hpp"

namespace homework_08 {

    class ITargetProvider {
        public:
        virtual int getTargetsCount() = 0;
        virtual Target getTargetPosition(int idx) = 0;
        virtual ~ITargetProvider() {};
    };

}
