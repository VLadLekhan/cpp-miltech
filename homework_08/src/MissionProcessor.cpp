#include "MissionProcessor.hpp"
#include "Types.hpp"
#include "interfaces/IConfigLoader.hpp"

#include <iostream>

namespace homework_08{

   MissionProcessor::MissionProcessor(IBallisticSolver* solver, IConfigLoader* loader, ITargetProvider* provider) :
   solver_(solver), loader_(loader), provider_(provider),targetIdx_{0} {}

   void MissionProcessor::init(){
        if(loader_ != nullptr){
            loader_->load();
        }

       if (provider_ != nullptr) {
        targets_ = provider_->getTargetPosition();

        if (!targets_.empty()) {
            std::cout << "[DEBUG INIT] Перша ціль у векторі відразу після завантаження: X=" 
                      << targets_.at(0).x << ", Y=" << targets_.at(0).y << std::endl;
        } else {
            std::cout << "[DEBUG INIT] КРИТИЧНО: Вектор цілей ПУСТИЙ!" << std::endl;
        }
    }
   }

   bool MissionProcessor::hasNext(){
        return targetIdx_ < targets_.size();
   }
   
   DropPoint MissionProcessor::step(){
    DropPoint result = {{0.0f, 0.0f}, {0.0,0.0}, false};

    if (solver_ == nullptr || loader_ == nullptr || targets_.empty()) {
        return result;
    }

    DronePosition dronPosition;
    dronPosition.x = 100.0f;
    dronPosition.y = 100.0f;
    Config config = loader_->getConfig();
    AmmoParams ammo = loader_->getAmmoParams();

    Target currentTarget = targets_.at(targetIdx_);
    std::cout << "[DEBUG PROCESSOR STEP] Поточна ціль перед викликом солвера: X=" 
          << currentTarget.x << ", Y=" << currentTarget.y << std::endl;

    result = solver_->csolve(dronPosition.x, dronPosition.y, config.zd, currentTarget.x, currentTarget.y, config.attackSpeed, config.accelerationPath, ammo);

    ++targetIdx_;

    return result;
    }

    void MissionProcessor::reset() {
    targetIdx_ = 0;
    std::cout << "Mission processor reset to initial target." << std::endl;
    }

    void MissionProcessor::changeSolver(IBallisticSolver* s) {
        if (s != nullptr) {
            solver_= s;
        }
    }

}