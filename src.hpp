#ifndef SRC_HPP
#define SRC_HPP

#include <any>

// Include visitor.h which should be provided by the OJ system
// and defines node, visitor, num_node, add_node, sub_node, mul_node, div_node
#ifndef VISITOR_H
#include "visitor.h"
#endif

// Helper function to safely extract numeric value from std::any
inline long long get_value(const std::any& a) {
    try {
        return std::any_cast<long long>(a);
    } catch (const std::bad_any_cast&) {
        try {
            return static_cast<long long>(std::any_cast<double>(a));
        } catch (const std::bad_any_cast&) {
            try {
                return static_cast<long long>(std::any_cast<int>(a));
            } catch (const std::bad_any_cast&) {
                return std::any_cast<long>(a);
            }
        }
    }
}

// Calculator class - implements the visitor interface for evaluation
struct calculator : visitor {
    std::any visit_num(num_node *n) override {
        return n->number;
    }

    std::any visit_add(add_node *n) override {
        long long left = get_value(n->lnode->accept(this));
        long long right = get_value(n->rnode->accept(this));
        return left + right;
    }

    std::any visit_sub(sub_node *n) override {
        long long left = get_value(n->lnode->accept(this));
        long long right = get_value(n->rnode->accept(this));
        return left - right;
    }

    std::any visit_mul(mul_node *n) override {
        long long left = get_value(n->lnode->accept(this));
        long long right = get_value(n->rnode->accept(this));
        return left * right;
    }

    std::any visit_div(div_node *n) override {
        long long left = get_value(n->lnode->accept(this));
        long long right = get_value(n->rnode->accept(this));
        return left / right;
    }
};

#endif // SRC_HPP
