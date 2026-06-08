#pragma once 
#include <vector>
#include <string>

#include "interfaces/ITargetProvider.hpp"


namespace homework_08 {

    class JsonTargetProvider : public ITargetProvider {
         private: 
         std::vector<Target> json_targets; 
         std::string json_path;

         public: 
         JsonTargetProvider (const std::string& filePath);
         int getTargetsCount() override;
         std::vector<Target> getTargetPosition() override;
         ~JsonTargetProvider() override {} ;

    };
}