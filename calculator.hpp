#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <iostream>
#include <string>
#include <memory>
#include <stack>
#include <cctype>
#include <stdexcept>
#include <any>
#include <vector>

// Forward declarations
class visitor;

// Abstract base class for expressions
class expr {
public:
    virtual ~expr() = default;
    virtual std::any accept(visitor *v) = 0;
};

// Number expression
class number : public expr {
public:
    double value;

    number(double val) : value(val) {}

    std::any accept(visitor *v) override;
};

// Binary operation expression
class binary_op : public expr {
public:
    std::unique_ptr<expr> left;
    std::unique_ptr<expr> right;
    char op;

    binary_op(std::unique_ptr<expr> l, std::unique_ptr<expr> r, char operation)
        : left(std::move(l)), right(std::move(r)), op(operation) {}

    std::any accept(visitor *v) override;
};

// Visitor interface
class visitor {
public:
    virtual ~visitor() = default;
    virtual std::any visit(number *num) = 0;
    virtual std::any visit(binary_op *binOp) = 0;
};

// Implementations of accept methods
std::any number::accept(visitor *v) {
    return v->visit(this);
}

std::any binary_op::accept(visitor *v) {
    return v->visit(this);
}

// Calculator visitor (evaluator)
class calculator : public visitor {
public:
    std::any visit(number *num) override {
        return num->value;
    }

    std::any visit(binary_op *binOp) override {
        double leftVal = std::any_cast<double>(binOp->left->accept(this));
        double rightVal = std::any_cast<double>(binOp->right->accept(this));

        switch (binOp->op) {
            case '+': return leftVal + rightVal;
            case '-': return leftVal - rightVal;
            case '*': return leftVal * rightVal;
            case '/':
                if (rightVal == 0) {
                    throw std::runtime_error("Division by zero");
                }
                return leftVal / rightVal;
            default:
                throw std::runtime_error("Unknown operator");
        }
    }
};

// Parser class
class parser {
private:
    std::string expr_str;
    size_t pos;

    void skipWhitespace() {
        while (pos < expr_str.length() && std::isspace(expr_str[pos])) {
            pos++;
        }
    }

    char peek() {
        skipWhitespace();
        if (pos < expr_str.length()) {
            return expr_str[pos];
        }
        return '\0';
    }

    char get() {
        skipWhitespace();
        if (pos < expr_str.length()) {
            return expr_str[pos++];
        }
        return '\0';
    }

    std::unique_ptr<expr> parseNumber() {
        skipWhitespace();

        std::string numStr;
        bool hasDecimal = false;

        if (pos < expr_str.length() && expr_str[pos] == '-') {
            numStr += expr_str[pos++];
        }

        while (pos < expr_str.length() && (std::isdigit(expr_str[pos]) || expr_str[pos] == '.')) {
            if (expr_str[pos] == '.') {
                if (hasDecimal) break;
                hasDecimal = true;
            }
            numStr += expr_str[pos++];
        }

        if (numStr.empty() || numStr == "-" || numStr == ".") {
            throw std::runtime_error("Invalid number");
        }

        return std::make_unique<number>(std::stod(numStr));
    }

    std::unique_ptr<expr> parsePrimary() {
        skipWhitespace();

        if (peek() == '(') {
            get(); // consume '('
            auto expr_result = parseExpression();
            if (get() != ')') {
                throw std::runtime_error("Expected ')'");
            }
            return expr_result;
        }

        return parseNumber();
    }

    std::unique_ptr<expr> parseTerm() {
        auto left = parsePrimary();

        while (true) {
            char op = peek();
            if (op != '*' && op != '/') {
                break;
            }
            get(); // consume operator
            auto right = parsePrimary();
            left = std::make_unique<binary_op>(std::move(left), std::move(right), op);
        }

        return left;
    }

    std::unique_ptr<expr> parseExpression() {
        auto left = parseTerm();

        while (true) {
            char op = peek();
            if (op != '+' && op != '-') {
                break;
            }
            get(); // consume operator
            auto right = parseTerm();
            left = std::make_unique<binary_op>(std::move(left), std::move(right), op);
        }

        return left;
    }

public:
    parser(const std::string& expression) : expr_str(expression), pos(0) {}

    std::unique_ptr<expr> parse() {
        auto result = parseExpression();
        skipWhitespace();
        if (pos < expr_str.length()) {
            throw std::runtime_error("Unexpected character at position " + std::to_string(pos));
        }
        return result;
    }
};

#endif // CALCULATOR_HPP
