//#pragma once

#include <string>
#include <vector>
#include <regex>
#include <cassert>
#include <unordered_map>
#include "stack.h"

using std::string;
using std::vector;

const string ops[] = {"=", ">=", "<=", ">", "<", "&", "|"};
const string bls[] = {"True", "False", "Undefined"};

struct Expression {
    using PropValInt = int;
    using PropValFloat = float;

    enum PropType{
        PropNone,
        PropString,
        PropInt,
        PropFloat,
        PropBool,
        PropParameter,
        PropOp,
        PropLeftBracket
    };

    enum CmpOp {
        Eq,
        Ge,
        Le,
        Gt,
        Lt,
        Ad,
        Or,
        Nd
    };

    enum returnType {
        True,
        False,
        Undefined,
    };

    struct Bool {
        returnType ans;

        explicit Bool() {
            ans = Undefined;
        }

        explicit Bool(bool x) {
            if(x) {
                ans = True;
            } else {
                ans = False;
            }
        }

        inline Bool operator && (const Bool& Rhs) const {
            if(ans == Undefined) {
                if(Rhs.ans == Undefined) {
                    return Bool();
                } else {
                    return Rhs;
                }
            } else if(Rhs.ans == Undefined) {
                return *this;
            } else {
                return Bool(ans == True && Rhs.ans == True);
            }
        }

        inline Bool operator || (const Bool& Rhs) const& {
            if(ans == Undefined) {
                if(Rhs.ans == Undefined) {
                    return Bool();
                } else {
                    return Rhs;
                }
            } else if(Rhs.ans == Undefined) {
                return *this;
            } else {
                return Bool(ans != Rhs.ans);
            }
        }

        inline Bool operator == (const Bool& Rhs) const& {
            if(ans == Undefined || Rhs.ans == Undefined) {
                return Bool(true);
            } else {
                return Bool(ans == Rhs.ans);
            }
        }

        inline Bool operator >= (const Bool& Rhs) const& {
            if(ans == Undefined || Rhs.ans == Undefined) {
                return Bool(true);
            } else {
                return Bool(ans >= Rhs.ans);
            }
        }

        inline Bool operator <= (const Bool& Rhs) const& {
            if(ans == Undefined || Rhs.ans == Undefined) {
                return Bool(true);
            } else {
                return Bool(ans <= Rhs.ans);
            }
        }

        inline Bool operator > (const Bool& Rhs) const& {
            if(ans == Undefined || Rhs.ans == Undefined) {
                return Bool(true);
            } else {
                return Bool(ans > Rhs.ans);
            }
        }

        inline Bool operator < (const Bool& Rhs) const& {
            if(ans == Undefined || Rhs.ans == Undefined) {
                return Bool(true);
            } else {
                return Bool(ans < Rhs.ans);
            }
        }

    };

    PropType type;

    CmpOp cmp_op;

    string name;
    string val_string;

    PropValInt val_int;
    PropValFloat val_float;
    Bool val_bool;

    Expression(): type(PropNone), cmp_op(Nd), val_int(0), val_float(0) {}

    inline Expression assign(const PropValInt& propValInt) {
        type = PropInt;
        val_int = propValInt;
        val_float = propValInt;
        return *this;
    }

    inline Expression assign(const PropValFloat& propValFloat) {
        type = PropFloat;
        val_float = propValFloat;
        return *this;
    }

    inline Expression assignParameter(const string& str) {
        type = PropParameter;
        name = str;
        return *this;
    }

    inline Expression assign(const string& str) {
        type = PropString;
        val_string = str;
        return *this;
    }

    inline Expression assign(const char* str) {
        string s(str);
        return assign(s);
    }

    inline Expression assignBool() {
        type = PropBool;
        val_bool = Bool();
        return *this;
    }

    inline Expression assignBool(const bool& b) {
        type = PropBool;
        val_bool = Bool(b);
        return *this;
    }

    inline Expression assignBool(const Bool& b) {
        type = PropBool;
        val_bool = b;
        return *this;
    }

    inline Expression assignEOp(const char& op) {
        type = PropOp;
        if(op == '=') {
            cmp_op = Eq;
        } else if(op == '>') {
            cmp_op = Ge;
        } else if(op == '<') {
            cmp_op = Le;
        } else {
            assert(false);
        }
        return *this;
    }

    inline Expression assignOp(const char& op) {
        type = PropOp;
        if(op == '=') {
            cmp_op = Eq;
        } else if(op == '>') {
            cmp_op = Gt;
        } else if(op == '<') {
            cmp_op = Lt;
        } else if(op == '&') {
            cmp_op = Ad;
        } else if(op == '|') {
            cmp_op = Or;

        } else {
            assert(false);
        }
        return *this;
    }

    inline Expression lBracket() {
        type = PropLeftBracket;
        return *this;
    }

    inline void print() {
        switch(type) {
            case PropInt:
                std::cout << val_int << " ";
                break;
            case PropFloat:
                std::cout << val_float << " ";
                break;
            case PropString:
                std::cout << "\'" << val_string << "\' ";
                break;
            case PropBool:
                std::cout << bls[val_bool.ans] << std::endl;
                break;
            case PropParameter:
                std::cout << name << " ";
                break;
            case PropOp:
                std::cout << ops[cmp_op] << " ";
                break;
            case PropLeftBracket:
                std::cout << '(' << " ";
                break;
            default:
                std::cout << "??? ";
                break;
        }
    }

    template <typename T>
    inline Expression CalcEq(const T& a, const T& b) {
        assignBool(a == b);
        return *this;
    }

    template <typename T>
    inline Expression CalcGe(const T& a, const T& b) {
        assignBool(a >= b);
        return *this;
    }

    template <typename T>
    inline Expression CalcGt(const T& a, const T& b) {
        assignBool(a > b);
        return *this;
    }

    inline Expression CalcAd(const Bool& a, const Bool& b) {
        assignBool(a && b);
        return *this;
    }

    inline Expression CalcAd(const string& a, const string& b) {
        assignBool(a.length() && b.length());
        return *this;
    }

    inline Expression CalcAd(const float& a, const float& b) {
        assignBool(std::abs(a) > 0.0001 && std::abs(b) > 0.0001);
        return *this;
    }

    inline Expression CalcAd(const int& a, const int& b) {
        assignBool(a && b);
        return *this;
    }

    inline Expression CalcOr(const Bool& a, const Bool& b) {
        assignBool(a || b);
        return *this;
    }

    inline Expression CalcOr(const string& a, const string& b) {
        assignBool(a.length() || b.length());
        return *this;
    }

    inline Expression CalcOr(const float& a, const float& b) {
        assignBool(std::abs(a) > 0.0001 || std::abs(b) > 0.0001);
        return *this;
    }

    inline Expression CalcOr(const int& a, const int& b) {
        assignBool(a || b);
        return *this;
    }

    inline string toString() const {
        if(type == PropString) {
            return val_string;
        } else {
            //TODO
            string s("data type to string not supported");
            assert(printf("%s\n", s.c_str()) && false);
            return "";
        }
    }

    inline float toFloat() const {
        return val_float;
    }

    template <class T>
    inline Expression Exec(const T& A, const T& B, const CmpOp& op) {
        switch(op) {
            case Eq: return CalcEq(A, B);
            case Ge: return CalcGe(A, B);
            case Le: return CalcGe(B, A);
            case Gt: return CalcGt(A, B);
            case Lt: return CalcGt(B, A);
            case Ad: return CalcAd(A, B);
            case Or: return CalcOr(A, B);
            default: assert(false); return Expression();
        }
    }

    inline Expression Calc(const CmpOp& op, const Expression& b) {
        if(type == PropBool || b.type == PropBool) {
            auto A = val_bool;
            auto B = b.val_bool;
            return Exec(A, B, op);
        }
        if(type == PropString || b.type == PropString) {
            auto A = toString();
            auto B = b.toString();
            return Exec(A, B, op);
        } else if(type == PropFloat || b.type == PropFloat) {
            auto A = toFloat();
            auto B = b.toFloat();
            return Exec(A, B, op);
        } else {
            auto A = val_int;
            auto B = b.val_int;
            return Exec(A, B, op);
        }
    }
};

struct Expressions : public vector<Expression> {
    using Self = vector<Expression>;

    using CmpOp = Expression::CmpOp;

    using PropValInt = Expression::PropValInt;
    using PropValFloat = Expression::PropValFloat;

    inline bool isW(const char& s) {
        return isalpha(s) || isdigit(s) || s == '_';
    }

    inline bool isD(const char& s) {
        return isdigit(s) || s == '.';
    }

    inline void print() {
        for(Expression e: *this) {
            e.print();
        }
        std::cout << std::endl;
    }

    int Prior(const Expression& e) {
        if(e.type != Expression::PropOp) {
            return 100;
        } else if(e.type == Expression::PropLeftBracket) {
            return 2;
        } else if(e.cmp_op == Expression::Ad || e.cmp_op == Expression::Or) {
            return 0;
        } else {
            return 1;
        }
    }

    inline void Parse(const char* A){
        stack<Expression> s;
        Self::clear();
        char g;
        auto len = (int) strlen(A);
        for(int i = 0; i < len; ) {
            g = A[i];
            if(isblank(g)) {
                i ++;
                continue;
            }
            Expression ret;
            if(isalpha(g)) {
                string ans;
                while(isW(g = A[i ++])) {
                    ans += (char) tolower(g);
                }
                -- i;
                Self::emplace_back(ret.assignParameter(ans));
            } else if(isdigit(g) || g == '-') {
                PropValInt ans = 0, fac = 1;
                PropValFloat ansFloat = 0;
                if(g == '-') {
                    fac = -1;
                    ++ i;
                }
                int cnt = 0;
                float d = 1;
                while(isD(g = A[i ++])) {
                    if(g == '.') {
                        ansFloat = ans;
                        cnt ++;
                    }
                    else if(cnt == 0) {
                        ans = (ans << 3) + (ans << 1) + g - 48;
                    } else {
                        ansFloat += d * (g - 48);
                        d *= 0.1;
                    }
                }
                -- i;
                if(cnt == 1) {
                    Self::emplace_back(ret.assign(fac * ansFloat));
                } else if(cnt == 0) {
                    Self::emplace_back(ret.assign(fac * ans));
                } else {
                    assert(false);
                }
            } else if(g == '\'') {
                // TODO: solve complex case
                string ans;
                i ++;
                while((g = A[i ++]) != '\'') {
                    ans += g;
                }
                Self::emplace_back(ret.assign(ans));
            } else if(g == '(') {
                i ++;
                s.push(ret.lBracket());
            } else if(g == ')') {
                i ++;
                while(!s.empty() && s.top().type != Expression::PropLeftBracket) {
                    Self::emplace_back(s.pop());
                }
                s.pop();
            } else if(g == '&' || g == '|') {
                i ++;
                while(!s.empty() && s.top().type != Expression::PropLeftBracket && Prior(s.top()) >= 0) {
                    Self::emplace_back(s.pop());
                }

                s.push(ret.assignOp(g));
            } else if(g == '=' || g == '<' || g == '>') {
                while(!s.empty() && s.top().type != Expression::PropLeftBracket && Prior(s.top()) >= 1) {
                    Self::emplace_back(s.pop());
                }
                i ++;
                if(A[i] == '=') {
                    i ++;
                    s.push(ret.assignEOp(g));
                } else {
                    s.push(ret.assignOp(g));
                }
            } else {
                assert(false);
            }
        }
        while(!s.empty()){
            Self::emplace_back(s.pop());
        }
        print();
    }

    template <typename iterable>
    inline bool Matched(iterable props) {
        std::unordered_map<string, Expression> val;
        Expression ex;
        val.clear();
        for(auto it = props.begin(); it != props.end(); ++ it) {
            auto type = it->Type();
            string name(it->Name());
            if(type == Expression::PropString) {
                val.insert(make_pair(name, ex.assign(it->String())));
            } else if(type == Expression::PropInt) {
                val.insert(make_pair(name, ex.assign(it->Int())));
            } else if(type == Expression::PropFloat) {
                val.insert(make_pair(name, ex.assign((float)(it->Float()))));
            } else {
                assert(false);
            }
        }
        stack<Expression> s;
        while(!s.empty()) s.pop();
        for(auto e: *this) {
            if(e.type == Expression::PropParameter) {
                if(val.find(e.name) == val.end()) {
                    s.push(ex.assignBool());
                } else {
                    s.push(val[e.name]);
                }
            } else if(e.type == Expression::PropOp) {
                Expression t1 = s.pop();
                Expression t2 = s.pop();
//                t2.print(); e.print(); t1.print();
                s.push(t2.Calc(e.cmp_op, t1));
//                t2.Calc(e.cmp_op, t1).print();
//                std::cout << std::endl;
            } else if(e.type == Expression::PropInt ||
                    e.type == Expression::PropFloat ||
                    e.type == Expression::PropString ||
                    e.type == Expression::PropBool) {
                s.push(e);
            } else {
                assert(false);
            }
        }
        return s.top().val_bool.ans != Expression::False;
    }

};
