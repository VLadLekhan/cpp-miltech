#include "interfaces.hpp"
#include "MissionProcessor.hpp"
#include <iostream>

int main() {

    std::cout << "====== MISSION START! ======" << std::endl;

    IConfigLoader* loader = createLoader(LoaderType::FILE);
    ITargetProvider* provider = createProvider(ProviderType::JSON, "target.json");
    IBallisticSolver* solver = createSolver(SolverType::ANALYTICAL);

    if (loader == nullptr || provider == nullptr || solver == nullptr) {
        std::cerr << "[CRITICAL]: Fabric initialization components are broke!" << std::endl;

        delete loader;
        delete provider;
        delete solver;

        return 1;
    }

        MissionProcessor mission (solver, loader, provider);

        mission.init("config.json");
        std::cout << "----- CONFIG.JSON IS DOWNLOADED! -----" << std::endl;

        int targetCounter = 1;

        while(mission.hasNext()) {
            std::cout << "\n[PROCESSING] Target №" << targetCounter << std::endl;

            DropPoint point = mission.step();

            std::cout << "fire: X = " << point.fire.x 
                  << ", Y = " << point.fire.y << std::endl;

            if (point.has_inter_point) {
            std::cout << ">>> inter (acceleration): X = " << point.inter.x 
                      << ", Y = " << point.inter.y << std::endl;
            } else {
            std::cout << ">>> inter: Not required for this target." << std::endl;
            }

            targetCounter++;       
        }

        delete loader;
        delete provider;
        delete solver;

    std::cout << "\n====== MISSION SUCCESS! ======" << std::endl;    
    return 0;
}      