#include "MissionProcessor.hpp"
#include <iostream>
#include "interfaces.hpp"

MissionProcessor::MissionProcessor(IBallisticSolver* solver, IConfigLoader* loader, ITargetProvider* provider)
    : solver_(solver), targetProvider_(provider), configLoader_(loader), currentIdx_(0)
    {}

    void MissionProcessor::init(const char* configPath) {
        if(configLoader_ != nullptr) {
            configLoader_->load(configPath);
        }

        currentIdx_ = 0;
    }

    bool MissionProcessor::hasNext(){
        if (targetProvider_ != nullptr) {
            return currentIdx_ < targetProvider_->getTargetCount();
        } else {
            return false;
        }
    }

    DropPoint MissionProcessor::step() {

        DropPoint result = {{0.0, 0.0}, {0.0, 0.0}, false};

        if (solver_ == nullptr || targetProvider_ == nullptr) {
            return result;
        }

        Config config = configLoader_->getConfig();
        AmmoParams ammo = configLoader_->getAmmoParams();
        Target currentTarget =  targetProvider_->getTarget(currentIdx_);

        result = solver_->csolve(0.0, 0.0, config.zd, currentTarget.x, currentTarget.y, config.attackSpeed, config.accelerationPath, ammo);
        ++currentIdx_;

        return result;
    }

    void MissionProcessor::reset() {
        currentIdx_ = 0;
        std::cout << "Mission reset!" << std::endl;
    }

    void MissionProcessor::changeSolver(IBallisticSolver* s) {
        if (s != nullptr) {
            solver_ = s;
        }
    }