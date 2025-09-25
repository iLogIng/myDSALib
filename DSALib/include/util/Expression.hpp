#pragma once
#include <cmath>
#include <cctype>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <initializer_list>
#include <utility>
#include <unordered_map>

namespace myDSALib
{
namespace util
{

// sign enum
enum class ExpSignMod
{
    lParen,     // ( [ {
    rParen,     // ) ] }
    digital,    // number
    operation,  // + - * / % ^
    other,      // 
    end         // #
};


class ExpressionEvaluator
{
private:
    // Priority Enum
    enum class OpPriority
    {
        Lowest = 0,
        Add_Sub = 1,
        Mul_Div = 2,
        Highest = 3
    };
    // operator Priority
    std::unordered_map<char, OpPriority> precedence;

// constructor
public:
    ExpressionEvaluator()
        : precedence({
            {'+', OpPriority::Add_Sub},
            {'-', OpPriority::Add_Sub},
            {'*', OpPriority::Mul_Div},
            {'/', OpPriority::Mul_Div},
            {'%', OpPriority::Mul_Div},
            {'^', OpPriority::Highest}
        }) { }
    ExpressionEvaluator(const ExpressionEvaluator&) = delete;
    ExpressionEvaluator& operator=(const ExpressionEvaluator&) = delete;
    ExpressionEvaluator(ExpressionEvaluator&&) = delete;
    ExpressionEvaluator& operator=(ExpressionEvaluator&&) = delete;

// Jugment and Parser
public:
    // is operator
    bool isOperator(char op) noexcept {
        return precedence.find(op) != precedence.end();
    }
    // get precedence
    OpPriority getPrecedence(char op) noexcept {
        return precedence[op];
    }
    // get Number String
    std::string extractNumber(const std::string& str, size_t& index) {
        std::string snumber;
        bool hasDecimal = false;

        while(index < str.length()) {
            char c = str[index];
            if(std::isdigit(c)) {
                snumber += c;
                ++index;
            }
            else if(c == '.' && !hasDecimal) {
                snumber += c;
                hasDecimal = true;  // stop extra '.' search
                ++index;
            }
            else {
                break;
            }
        }
        --index;

        return snumber;
    }

// Expression Transform
public:
    // Infix to Polish
    std::string infixToPrefix(const std::string& infix);

    // Polish to Infix
    std::string PrefixToInfix(const std::string& postfix);

    // Infix to Reverse Polish
    std::string infixToPostfix(const std::string& infix);

    // Reverse Polish to Infix
    std::string PostfixToInfix(const std::string& postfix);

// compute
public:
    // compute
    double compute(double a, double b, char op) {
        switch(op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': {
                if(b == 0.0)
                    throw std::runtime_error("Division by zero.");
                return a / b;
            }
            case '%': return std::fmod(a, b);
            case '^': return std::pow(a, b);
            default: throw std::runtime_error("Unknown operator.");
        }
    }

    // compute Polish
    double computePrefix(const std::string& exp);

    // compute Infix
    double computeInfix(const std::string& exp);

    // compute rPolish
    double computePostfix(const std::string& exp);

// Token Parser
public:

};

// =====================================================

std::string ExpressionEvaluator::infixToPostfix(const std::string& infix) {
    std::stack<char> opStack;
    std::string postfix;

    for(size_t i = 0; i < infix.length(); ++i) {
        char c = infix[i];

        // check spare space
        if(std::isspace(c)) {
            continue;
        }

        // digital parser as a float64
        if(std::isdigit(c) || c == '.') {
            postfix += extractNumber(infix, i);
            postfix += ' ';
            continue;
        }

        // push left parn
        if(c == '(') {
            opStack.push(c);
            continue;
        }

        // mark the right parn
        if(c == ')') {
            while(!opStack.empty() && opStack.top() != '(') {
                postfix += opStack.top();
                postfix += ' ';
                opStack.pop();
            }

            // not a full expression
            if(opStack.empty() || opStack.top() != '(') {
                throw std::runtime_error("error expression.");
            }

            // pop the left parn
            opStack.pop();
            continue;
        }

        // operator parser
        if(isOperator(c)) {
            while(!opStack.empty() && opStack.top() != '('
                && (getPrecedence(opStack.top()) > getPrecedence(c)
                || getPrecedence(opStack.top()) == getPrecedence(c))
            ) {
                postfix += opStack.top();
                postfix += ' ';
                opStack.pop();
            }

            opStack.push(c);
            continue;
        }

        throw std::runtime_error("error.");
    }

    // pop the spare op
    while (!opStack.empty()) {
        if (opStack.top() == '(') {
            throw std::runtime_error("Mismatched parentheses");
        }
        postfix += opStack.top();
        postfix += ' ';
        opStack.pop();
    }

    // remove the spare tail space
    if (!postfix.empty() && postfix.back() == ' ') {
        postfix.pop_back();
    }

    // return the postfix list
    return postfix;
}

// =========================================================

double ExpressionEvaluator::computePostfix(const std::string& exp) {
    std::stack<double> digital_s;
    double result = 0.0;

    for(size_t i = 0; i < exp.length(); ++i) {
        // jump the space
        if(isspace(exp[i])) {
            continue;
        }

        // catch the digit
        if(isdigit(exp[i])) {
            double num = std::stod(extractNumber(exp, i));
            digital_s.push(num);
            continue;
        }
        else {
            double b = digital_s.top(); digital_s.pop();
            double a = digital_s.top(); digital_s.pop();
            digital_s.push(compute(a, b, exp[i]));
        }
    }

    return digital_s.top();
}

double ExpressionEvaluator::computeInfix(const std::string& exp) {
    std::string postfix = infixToPostfix(exp);
    return computePostfix(postfix);
}

}
}
