#ifndef SRC_HPP
#define SRC_HPP

#include <any>

// Forward declarations
struct node;
struct visitor;
struct num_node;
struct add_node;
struct sub_node;
struct mul_node;
struct div_node;

// Calculator class
struct calculator : visitor {
    std::any visit_num(num_node *n) override {
        return n->number;
    }

    std::any visit_add(add_node *n) override {
        long long left = std::any_cast<long long>(n->lnode->accept(this));
        long long right = std::any_cast<long long>(n->rnode->accept(this));
        return left + right;
    }

    std::any visit_sub(sub_node *n) override {
        long long left = std::any_cast<long long>(n->lnode->accept(this));
        long long right = std::any_cast<long long>(n->rnode->accept(this));
        return left - right;
    }

    std::any visit_mul(mul_node *n) override {
        long long left = std::any_cast<long long>(n->lnode->accept(this));
        long long right = std::any_cast<long long>(n->rnode->accept(this));
        return left * right;
    }

    std::any visit_div(div_node *n) override {
        long long left = std::any_cast<long long>(n->lnode->accept(this));
        long long right = std::any_cast<long long>(n->rnode->accept(this));
        return left / right;
    }
};

#endif // SRC_HPP
