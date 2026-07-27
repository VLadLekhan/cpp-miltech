#include "config/ComponentFactory.hpp"
#include "config/FileConfigLoader.hpp"
#include "providers/JsonTargetProvider.hpp"
#include "solvers/AnalyticalSolver.hpp"

#include <stdexcept>

namespace homework_08{
    IBallisticSolver* ComponentFactory::createSolver(SolverType type){
        switch (type) {
            case SolverType ::ANALYTICAL:
              return new AnalyticalSolver();
            default:
             throw std::runtime_error("ComponentFactory Error: Unsupported solver type!");
        }
    }

    IConfigLoader* ComponentFactory::createLoader(LoaderType type){
        switch (type) {
            case LoaderType::FILE:
             return new FileConfigLoader("data/config.json");
            default:
             throw std::runtime_error("ComponentFactory Error: Unsupported loader type!");
        }
    }

    ITargetProvider* ComponentFactory::createProvider(ProviderType type, std::string filepath){
        switch(type){
            case ProviderType::JSON:
            if(filepath.empty()){
                throw std::runtime_error("ComponentFactory Error: JSON provider requires file path params!");
            }
            return new JsonTargetProvider(filepath);
            default:
            throw std::runtime_error("ComponentFactory Error: Unsupported provider type!");
        }
    }

}