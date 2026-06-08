#pragma once 

#include <vector>

#include "Types.hpp"
#include "interfaces/IBallisticSolver.hpp"
#include "interfaces/IConfigLoader.hpp"
#include "interfaces/ITargetProvider.hpp"

namespace homework_08{
    class MissionProcessor {
        private:
        IBallisticSolver* solver_;
        IConfigLoader* loader_;
        ITargetProvider* provider_;

        std::vector<Target> targets_;

        size_t targetIdx_;

        public:
        MissionProcessor(IBallisticSolver* solver, IConfigLoader* loader, ITargetProvider* provider);
        
        void init();
        bool hasNext();
        DropPoint step();

        void reset();
        void changeSolver(IBallisticSolver* s);
    }; 
}
