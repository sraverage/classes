#ifndef FX_HPP
#define FX_HPP

#include <string>

class fx {
public:
    fx(const std::string& expression_str);
    ~fx();

    double operator()(double x_value);
    fx& operator=(const fx& outro);
    fx& operator=(const std::string& new_expression_str);
    std::string get_function_string() const;

private:
    std::string pt_br(std::string func);  
    bool compile_expression_internal(const std::string& expression_str);

    struct Impl;
    Impl* pimpl;

    std::string _function_string;
    double _x_var_ref;
};

#endif
