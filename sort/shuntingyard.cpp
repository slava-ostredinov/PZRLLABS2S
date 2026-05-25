#include <iostream>
#include <stack>
#include <string>
#include <map>
#include <sstream>
#include <cctype>
#include <vector>
#include <cmath>

class SimpleShuntingYard {
private:
    std::map<std::string, int> precedence = {
        {"+", 1}, {"-", 1},
        {"*", 2}, {"/", 2},
        {"^", 3}
    };

    bool isOperator(const std::string& token) {
        return precedence.find(token) != precedence.end();
    }

    bool isUnaryMinus(const std::string& token, const std::string& prevToken) {
        return token == "-" && (prevToken.empty() ||
               isOperator(prevToken) || prevToken == "(");
    }

    std::vector<std::string> tokenize(const std::string& expression) {
        std::vector<std::string> tokens;
        std::string currentToken;

        for (size_t i = 0; i < expression.length(); i++) {
            char c = expression[i];

            if (std::isdigit(c)) {
                currentToken = c;
                while (i + 1 < expression.length() && std::isdigit(expression[i + 1])) {
                    currentToken += expression[i + 1];
                    i++;
                }
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
                tokens.push_back(std::string(1, c));
            }
            else if (c == '(' || c == ')') {
                tokens.push_back(std::string(1, c));
            }
            else if (c == ' ') {
                continue;
            }
            else {
                throw std::runtime_error("Недопустимый символ: " + std::string(1, c));
            }
        }

        return tokens;
    }

public:
    std::string infixToPostfix(const std::string& infix) {
        std::stack<std::string> operators;
        std::string postfix;
        std::vector<std::string> tokens = tokenize(infix);
        std::string prevToken = "";

        for (size_t idx = 0; idx < tokens.size(); idx++) {
            std::string token = tokens[idx];

            bool isNumber = !token.empty();
            for (char c : token) {
                if (!isdigit(c)) {
                    isNumber = false;
                    break;
                }
            }

            
            if (token == "-" && isUnaryMinus(token, prevToken)) {
                operators.push("u-"); 
                prevToken = token;
                continue;
            }

            if (isNumber) {
                postfix += token + " ";
                prevToken = token;
            }
            else if (token == "(") {
                operators.push(token);
                prevToken = token;
            }
            else if (token == ")") {
                while (!operators.empty() && operators.top() != "(") {
                    postfix += operators.top() + " ";
                    operators.pop();
                }
                if (!operators.empty() && operators.top() == "(") {
                    operators.pop();
                } else {
                    throw std::runtime_error("Несоответствие скобок");
                }
                prevToken = token;
            }
            else if (isOperator(token)) {
                while (!operators.empty() &&
                       operators.top() != "(" &&
                       precedence[operators.top()] >= precedence[token]) {
                    postfix += operators.top() + " ";
                    operators.pop();
                }
                operators.push(token);
                prevToken = token;
            }
            else {
                throw std::runtime_error("Недопустимый токен: " + token);
            }
        }

        while (!operators.empty()) {
            if (operators.top() == "(" || operators.top() == ")") {
                throw std::runtime_error("Несоответствие скобок");
            }
            postfix += operators.top() + " ";
            operators.pop();
        }

        return postfix;
    }

    double evaluatePostfix(const std::string& postfix) {
        std::stack<double> values;
        std::istringstream iss(postfix);
        std::string token;

        while (iss >> token) {
            bool isNumber = true;
            bool hasDigits = false;
            for (size_t i = 0; i < token.length(); i++) {
                if (i == 0 && token[i] == '-') continue;
                if (!isdigit(token[i])) {
                    isNumber = false;
                    break;
                }
                hasDigits = true;
            }

            if (isNumber && hasDigits) {
                values.push(std::stod(token));
            }
            else if (token == "u-") { 
                if (values.empty()) {
                    throw std::runtime_error("Недостаточно операндов для унарного минуса");
                }
                double a = values.top(); values.pop();
                values.push(-a);
            }
            else if (isOperator(token)) {
                if (values.size() < 2) {
                    throw std::runtime_error("Недостаточно операндов для оператора " + token);
                }

                double b = values.top(); values.pop();
                double a = values.top(); values.pop();

                if (token == "+") values.push(a + b);
                else if (token == "-") values.push(a - b);
                else if (token == "*") values.push(a * b);
                else if (token == "/") {
                    if (b == 0) throw std::runtime_error("Деление на ноль");
                    values.push(a / b);
                }
                else if (token == "^") {
                    values.push(std::pow(a, b));
                }
                else {
                    throw std::runtime_error("Неизвестный оператор: " + token);
                }
            }
            else {
                throw std::runtime_error("Недопустимый токен в постфиксном выражении: " + token);
            }
        }

        if (values.empty()) {
            throw std::runtime_error("Пустое выражение");
        }

        if (values.size() > 1) {
            throw std::runtime_error("Слишком много операндов");
        }

        return values.top();
    }

    double calculate(const std::string& expression) {
        std::string postfix = infixToPostfix(expression);
        std::cout << "Постфиксная форма: " << postfix << std::endl;
        return evaluatePostfix(postfix);
    }
};

int main() {
    SimpleShuntingYard sy;
    std::string input;
    while (true) {
        std::cout << "\nВведите выражение: ";
        std::getline(std::cin, input);

        if (input == "exit" || input == "quit") {
            break;
        }

        if (input.empty()) {
            continue;
        }

        try {
            double result = sy.calculate(input);
            std::cout << "Результат: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
    }
    return 0;
}
