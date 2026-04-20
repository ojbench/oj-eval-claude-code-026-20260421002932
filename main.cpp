#include <iostream>
#include <string>
#include <memory>
#include <stack>
#include <cctype>
#include <stdexcept>
#include <sstream>

// Forward declarations
class Visitor;

// Abstract base class for expressions
class Expr {
public:
    virtual ~Expr() = default;
    virtual double accept(Visitor& v) = 0;
};

// Number expression
class Number : public Expr {
public:
    double value;

    Number(double val) : value(val) {}

    double accept(Visitor& v) override;
};

// Binary operation expression
class BinaryOp : public Expr {
public:
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    char op;

    BinaryOp(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r, char operation)
        : left(std::move(l)), right(std::move(r)), op(operation) {}

    double accept(Visitor& v) override;
};

// Visitor interface
class Visitor {
public:
    virtual ~Visitor() = default;
    virtual double visitNumber(Number* num) = 0;
    virtual double visitBinaryOp(BinaryOp* binOp) = 0;
};

// Implementations of accept methods
double Number::accept(Visitor& v) {
    return v.visitNumber(this);
}

double BinaryOp::accept(Visitor& v) {
    return v.visitBinaryOp(this);
}

// Evaluator visitor
class Evaluator : public Visitor {
public:
    double visitNumber(Number* num) override {
        return num->value;
    }

    double visitBinaryOp(BinaryOp* binOp) override {
        double leftVal = binOp->left->accept(*this);
        double rightVal = binOp->right->accept(*this);

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
class Parser {
private:
    std::string expr;
    size_t pos;

    void skipWhitespace() {
        while (pos < expr.length() && std::isspace(expr[pos])) {
            pos++;
        }
    }

    char peek() {
        skipWhitespace();
        if (pos < expr.length()) {
            return expr[pos];
        }
        return '\0';
    }

    char get() {
        skipWhitespace();
        if (pos < expr.length()) {
            return expr[pos++];
        }
        return '\0';
    }

    std::unique_ptr<Expr> parseNumber() {
        skipWhitespace();

        std::string numStr;
        bool hasDecimal = false;

        if (pos < expr.length() && expr[pos] == '-') {
            numStr += expr[pos++];
        }

        while (pos < expr.length() && (std::isdigit(expr[pos]) || expr[pos] == '.')) {
            if (expr[pos] == '.') {
                if (hasDecimal) break;
                hasDecimal = true;
            }
            numStr += expr[pos++];
        }

        if (numStr.empty() || numStr == "-" || numStr == ".") {
            throw std::runtime_error("Invalid number");
        }

        return std::make_unique<Number>(std::stod(numStr));
    }

    std::unique_ptr<Expr> parsePrimary() {
        skipWhitespace();

        if (peek() == '(') {
            get(); // consume '('
            auto expr = parseExpression();
            if (get() != ')') {
                throw std::runtime_error("Expected ')'");
            }
            return expr;
        }

        return parseNumber();
    }

    std::unique_ptr<Expr> parseTerm() {
        auto left = parsePrimary();

        while (true) {
            char op = peek();
            if (op != '*' && op != '/') {
                break;
            }
            get(); // consume operator
            auto right = parsePrimary();
            left = std::make_unique<BinaryOp>(std::move(left), std::move(right), op);
        }

        return left;
    }

    std::unique_ptr<Expr> parseExpression() {
        auto left = parseTerm();

        while (true) {
            char op = peek();
            if (op != '+' && op != '-') {
                break;
            }
            get(); // consume operator
            auto right = parseTerm();
            left = std::make_unique<BinaryOp>(std::move(left), std::move(right), op);
        }

        return left;
    }

public:
    Parser(const std::string& expression) : expr(expression), pos(0) {}

    std::unique_ptr<Expr> parse() {
        auto result = parseExpression();
        skipWhitespace();
        if (pos < expr.length()) {
            throw std::runtime_error("Unexpected character at position " + std::to_string(pos));
        }
        return result;
    }
};

int main() {
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }

        try {
            Parser parser(line);
            auto expr = parser.parse();

            Evaluator evaluator;
            double result = expr->accept(evaluator);

            // Output with proper precision
            std::cout << result << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}
