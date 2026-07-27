#pragma once

#include <string>

#include "Types.hpp"
#include "interfaces/IBallisticSolver.hpp"
#include "interfaces/IConfigLoader.hpp"
#include "interfaces/ITargetProvider.hpp"

namespace homework_08{

    class ComponentFactory {
        public:
        static IBallisticSolver* createSolver(SolverType type);
        static IConfigLoader* createLoader(LoaderType type);
        static ITargetProvider* createProvider(ProviderType type, std::string filepath);
    };
}