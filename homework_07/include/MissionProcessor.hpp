#pragma once
#include "interfaces.hpp"

class MissionProcessor {
    private:
        IBallisticSolver* solver_;
        IConfigLoader* configLoader_;
        ITargetProvider* targetProvider_;

        int currentIdx_;

    public:
        MissionProcessor(IBallisticSolver* solver, IConfigLoader* loader, ITargetProvider* provider);

        void init(const char* configPath);
        bool hasNext();
        DropPoint step();

        void reset();
        void changeSolver(IBallisticSolver* s);
};
