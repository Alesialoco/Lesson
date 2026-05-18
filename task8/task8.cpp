#include "task8.h"

ExpressionFactory::ExpressionFactory() {
    for (int i = -5; i <= 256; ++i) {
        alwaysExistConst[i] = new Constant(i, this);
        constantCount[i] = 0;
    }
}

ExpressionFactory::~ExpressionFactory() {
    for (auto& pair : alwaysExistConst) {
        delete pair.second;
    }
    for (auto& pair : sometimesConst) {
        delete pair.second;
    }
    for (auto& pair : variables) {
        delete pair.second;
    }
}

Constant* ExpressionFactory::createConstant(int value) {
    if (value >= -5 && value <= 256) {
        constantCount[value]++;
        return alwaysExistConst[value];
    }
    
    if (sometimesConst.find(value) == sometimesConst.end()) {
        sometimesConst[value] = new Constant(value, this);
        constantCount[value] = 0;
    }
    
    constantCount[value]++;
    return sometimesConst[value];
}

Variable* ExpressionFactory::createVariable(const std::string& name) {
    if (variables.find(name) == variables.end()) {
        variables[name] = new Variable(name, this);
        variableCount[name] = 0;
    }
    
    variableCount[name]++;
    return variables[name];
}

void ExpressionFactory::releaseConstant(Constant* c) {
    if (!c) return;
    int val = c->getValue();
    
    if (constantCount.find(val) != constantCount.end()) {
        constantCount[val]--;
        
        if (constantCount[val] <= 0 && (val < -5 || val > 256)) {
            delete sometimesConst[val];
            sometimesConst.erase(val);
            constantCount.erase(val);
        }
    }
}

void ExpressionFactory::releaseVariable(Variable* v) {
    if (!v) return;
    std::string name = v->getName();
    
    if (variableCount.find(name) != variableCount.end()) {
        variableCount[name]--;
        
        if (variableCount[name] <= 0) {
            delete variables[name];
            variables.erase(name);
            variableCount.erase(name);
        }
    }
}

int main() {
    ExpressionFactory factory;
    Constant *c = factory.createConstant(2);
    Variable *v = factory.createVariable("x");
    Addition *expression = new Addition(c, v);
    std::map<std::string, double> context;
    context["x"] = 3;
    std::cout << expression->calculate(context) << std::endl;
    delete expression;
    
    return 0;
}