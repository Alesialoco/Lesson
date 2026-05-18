#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <stdexcept>

class ExpressionFactory {
    std::unordered_map<int, class Constant*> alwaysExistConst;
    std::unordered_map<int, class Constant*> sometimesConst;
    std::unordered_map<std::string, class Variable*> variables;
    
    std::unordered_map<int, int> constantCount;
    std::unordered_map<std::string, int> variableCount;

public:
    ExpressionFactory();
    ~ExpressionFactory();
    
    class Constant* createConstant(int value);
    class Variable* createVariable(const std::string& name);

    void releaseConstant(class Constant* c);
    void releaseVariable(class Variable* v);
};

class Expression {
public:
    virtual ~Expression() = default;
    virtual double calculate(const std::map<std::string, double>& context) const = 0;
    virtual void release() = 0;
};

class Constant : public Expression {
    int value;
    ExpressionFactory* factory;
    Constant(int val, ExpressionFactory* f = nullptr) : value(val), factory(f) {}
    friend class ExpressionFactory;

public:
    double calculate(const std::map<std::string, double>& context) const override {
        return static_cast<double>(value);
    }

    void release() override {
        if (factory) {
            factory->releaseConstant(this);
        }
    }

    int getValue() const { 
        return value; 
    }
    ExpressionFactory* getFactory() const { 
        return factory; 
    }
};

class Variable : public Expression {
    std::string name;
    ExpressionFactory* factory;
    Variable(const std::string& n, ExpressionFactory* f = nullptr) : name(n), factory(f) {}
    friend class ExpressionFactory;

public:
    double calculate(const std::map<std::string, double>& context) const override {
        auto it = context.find(name);
        if (it != context.end()) {
            return it->second;
        }
        throw std::runtime_error("Variable '" + name + "' not found");
    }

    void release() override {
        if (factory) {
            factory->releaseVariable(this);
        }
    }

    std::string getName() const { 
        return name; 
    }
    ExpressionFactory* getFactory() const { 
        return factory; 
    }
};

class Operator : public Expression {
protected:
    Expression* left;
    Expression* right;

public:
    Operator(Expression* l, Expression* r) : left(l), right(r) {}
    
    virtual ~Operator() {
        release();
    }
    
    void release() override {
        if (left) {
            left->release();
            left = nullptr;
        }
        if (right) {
            right->release();
            right = nullptr;
        }
    }
};

class Addition : public Operator {
public:
    Addition(Expression* l, Expression* r) : Operator(l, r) {}

    double calculate(const std::map<std::string, double>& context) const override {
        return left->calculate(context) + right->calculate(context);
    }
};

class Subtraction : public Operator {
public:
    Subtraction(Expression* l, Expression* r) : Operator(l, r) {}

    double calculate(const std::map<std::string, double>& context) const override {
        return left->calculate(context) - right->calculate(context);
    }
};

class Multiplication : public Operator {
public:
    Multiplication(Expression* l, Expression* r) : Operator(l, r) {}

    double calculate(const std::map<std::string, double>& context) const override {
        return left->calculate(context) * right->calculate(context);
    }
};