#include "calculator.hpp"
#include <iostream>
#include <string>

int main() {
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }

        try {
            parser p(line);
            auto expr_tree = p.parse();

            visitor *calc = new calculator;
            std::any result = expr_tree->accept(calc);
            double value = std::any_cast<double>(result);

            std::cout << value << std::endl;

            delete calc;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}
