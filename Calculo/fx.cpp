#include "fx.hpp"
#include <iostream>
#include <cmath>
#include "exprtk.hpp"
#include <string>

struct fx::Impl {
    exprtk::symbol_table<double> symbol_table;
    exprtk::expression<double>   expression;
    exprtk::parser<double>       parser;
};

fx::fx(const std::string& expression_str)
    : _function_string(), _x_var_ref(0.0), pimpl(new Impl()) {
    compile_expression_internal(expression_str);
}

fx::~fx() {
    delete pimpl;
}

bool fx::compile_expression_internal(const std::string& expression_str) {
    _function_string = ln_to_log(expression_str);
    _x_var_ref = 0.0;

    pimpl->symbol_table.clear();
    pimpl->symbol_table.add_variable("x", _x_var_ref);
    pimpl->symbol_table.add_constant("e", std::exp(1.0));
    pimpl->symbol_table.add_constant("pi", std::acos(-1.0));

    pimpl->expression.register_symbol_table(pimpl->symbol_table);

    if (pimpl->parser.compile(_function_string, pimpl->expression)) {
        return true;
    } else {
        std::cerr << "ERRO (fx::compile_expression_internal): Falha ao compilar a expressao '"
                  << _function_string << "'. Mensagem: " << pimpl->parser.error() << std::endl;
        return false;
    }
}

std::string fx::ln_to_log(std::string str) {
    const std::string de = "ln";
    const std::string para = "log";
    size_t pos=0;
    
    while ((pos = str.find(de, pos)) != std::string::npos) {
        str.replace(pos, de.length(), para);
        pos += para.length();
    }

    return str;

}

double fx::operator()(double x_value) {
    _x_var_ref = x_value;
    return pimpl->expression.value();
}

fx& fx::operator=(const fx& outro) {
    if (this != &outro) {
        compile_expression_internal(outro._function_string);
    }
    return *this;
}

fx& fx::operator=(const std::string& new_expression_str) {
    compile_expression_internal(new_expression_str);
    return *this;
}

std::string fx::get_function_string() const {
    return _function_string;
}
