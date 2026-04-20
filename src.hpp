#ifndef SRC_HPP
#define SRC_HPP

#include <any>

// Forward declarations
class num_node;
class add_node;
class sub_node;
class mul_node;
class div_node;

// Base node class - assuming this exists in visitor.h
class node {
public:
    virtual ~node() = default;
    virtual std::any accept(class visitor *v) = 0;
};

// Visitor interface
class visitor {
public:
    virtual ~visitor() = default;
    virtual std::any visit_num(num_node *node) = 0;
    virtual std::any visit_add(add_node *node) = 0;
    virtual std::any visit_sub(sub_node *node) = 0;
    virtual std::any visit_mul(mul_node *node) = 0;
    virtual std::any visit_div(div_node *node) = 0;
};

// Number node
class num_node : public node {
public:
    long long val;

    num_node(long long v) : val(v) {}

    std::any accept(visitor *v) override {
        return v->visit_num(this);
    }
};

// Binary operation nodes
class add_node : public node {
public:
    node *left;
    node *right;

    add_node(node *l, node *r) : left(l), right(r) {}

    std::any accept(visitor *v) override {
        return v->visit_add(this);
    }

    ~add_node() {
        delete left;
        delete right;
    }
};

class sub_node : public node {
public:
    node *left;
    node *right;

    sub_node(node *l, node *r) : left(l), right(r) {}

    std::any accept(visitor *v) override {
        return v->visit_sub(this);
    }

    ~sub_node() {
        delete left;
        delete right;
    }
};

class mul_node : public node {
public:
    node *left;
    node *right;

    mul_node(node *l, node *r) : left(l), right(r) {}

    std::any accept(visitor *v) override {
        return v->visit_mul(this);
    }

    ~mul_node() {
        delete left;
        delete right;
    }
};

class div_node : public node {
public:
    node *left;
    node *right;

    div_node(node *l, node *r) : left(l), right(r) {}

    std::any accept(visitor *v) override {
        return v->visit_div(this);
    }

    ~div_node() {
        delete left;
        delete right;
    }
};

// Helper functions to create nodes
inline num_node* create_num(long long val) {
    return new num_node(val);
}

inline add_node* create_add(node *left, node *right) {
    return new add_node(left, right);
}

inline sub_node* create_sub(node *left, node *right) {
    return new sub_node(left, right);
}

inline mul_node* create_mul(node *left, node *right) {
    return new mul_node(left, right);
}

inline div_node* create_div(node *left, node *right) {
    return new div_node(left, right);
}

// Calculator visitor implementation
class calculator : public visitor {
public:
    std::any visit_num(num_node *node) override {
        return node->val;
    }

    std::any visit_add(add_node *node) override {
        long long left_val = std::any_cast<long long>(node->left->accept(this));
        long long right_val = std::any_cast<long long>(node->right->accept(this));
        return left_val + right_val;
    }

    std::any visit_sub(sub_node *node) override {
        long long left_val = std::any_cast<long long>(node->left->accept(this));
        long long right_val = std::any_cast<long long>(node->right->accept(this));
        return left_val - right_val;
    }

    std::any visit_mul(mul_node *node) override {
        long long left_val = std::any_cast<long long>(node->left->accept(this));
        long long right_val = std::any_cast<long long>(node->right->accept(this));
        return left_val * right_val;
    }

    std::any visit_div(div_node *node) override {
        long long left_val = std::any_cast<long long>(node->left->accept(this));
        long long right_val = std::any_cast<long long>(node->right->accept(this));
        return left_val / right_val;
    }
};

#endif // SRC_HPP
