#include <iostream>
#include <memory>

#include "Types.hpp"
#include "MissionProcessor.hpp"

#include "config/FileConfigLoader.hpp"
#include "providers/JsonTargetProvider.hpp"
#include "solvers/AnalyticalSolver.hpp"

using namespace homework_08;

int main() {
    std::cout << "=== Наземна станція керування: Старт місії ===" << std::endl;

    auto solver = std::make_unique<AnalyticalSolver>();
    auto loader = std::make_unique<FileConfigLoader>("data/config.json");
    auto provider = std::make_unique<JsonTargetProvider>("data/targets.json");

    MissionProcessor processor (solver.get(), loader.get(), provider.get());

    processor.init();

    std::cout << "\nЗапуск покрокового розрахунку балістики..." << std::endl;

    int counter = 1;

    while (processor.hasNext()) {
        std::cout << "\n--- [Ціль N" << counter << "] ---" << std::endl;

        DropPoint dropPoint = processor.step();

        if(dropPoint.has_inter_point){
            std::cout << "Точка скиду РОЗРАХОВАНА успішно!" << std::endl;
            std::cout << "Координати скиду: X = " << dropPoint.fire.x 
                      << ", Y = " << dropPoint.fire.y << std::endl;
        } else {
            std::cout << "Помилка: Не вдалося розрахувати балістику для цієї цілі." << std::endl;
        }

        ++counter;

    }

    std::cout << "\n=============================================" << std::endl;
    std::cout << "Усі цілі оброблені. Скидаємо місію на початок..." << std::endl;
    processor.reset(); 

    std::cout << "=== Місія завершена успішно ===" << std::endl;
    return 0;
}

