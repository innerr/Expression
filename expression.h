#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;
using std::ostream;
using std::unordered_map;
using std::abs;

struct Expression {
    using PropValInt = int;
    using PropValFloat = float;

    enum PropType {
        PropNone,
        PropString,
        PropInt,
        PropFloat,
        PropBool,
        PropParameter,
        PropOp,
        PropLeftBracket
    };

    enum CmpOp {Eq, Ge, Le, Gt, Lt, Or, And, Nd};

    enum ReturnType {
        Undefined = -1,
        False = 0,
        True = 1,
    };

    struct Bool {
        ReturnType ans;

        inline explicit Bool(ReturnType x) : ans(x) {}
        inline explicit Bool(bool x) : ans(x ? True : False) {}

        inline Bool operator && (const Bool &rhs) const {
            if (ans == Undefined)
                return (rhs.ans == Undefined) ? Bool(Undefined) : rhs;
            return (rhs.ans == Undefined) ? *this : Bool(ans == True && rhs.ans == True);
        }
        inline Bool operator || (const Bool &rhs) const {
            if (ans == Undefined)
                return (rhs.ans == Undefined) ? Bool(Undefined) : rhs;
            return (rhs.ans == Undefined) ? *this : Bool(ans == True || rhs.ans == True);
        }
        inline Bool operator == (const Bool &rhs) const {
            return (ans == Undefined || rhs.ans == Undefined) ? Bool(true) : Bool(ans == rhs.ans);
        }
        inline Bool operator >= (const Bool &rhs) const {
            return (ans == Undefined || rhs.ans == Undefined) ? Bool(true) : Bool(ans >= rhs.ans);
        }
        inline Bool operator <= (const Bool &Rhs) const {
            return (ans == Undefined || Rhs.ans == Undefined) ? Bool(true) : Bool(ans <= Rhs.ans);
        }
        inline Bool operator > (const Bool &Rhs) const {
            return (ans == Undefined || Rhs.ans == Undefined) ? Bool(true) : Bool(ans > Rhs.ans);
        }
        inline Bool operator < (const Bool &Rhs) const {
            return (ans == Undefined || Rhs.ans == Undefined) ? Bool(true) : Bool(ans < Rhs.ans);
        }
    };

    PropType type;
    CmpOp cmp_op;
    string name;

    string val_string;
    PropValInt val_int;
    PropValFloat val_float;
    Bool val_bool;

    inline Expression(): type(PropNone), cmp_op(Nd), val_int(0), val_float(0), val_bool(Undefined) {}

    inline Expression & Assign(const PropValInt &propValInt) {
        type = PropInt;
        val_int = propValInt;
        val_float = propValInt;
        return *this;
    }
    inline Expression & Assign(const PropValFloat &propValFloat) {
        type = PropFloat;
        val_float = propValFloat;
        return *this;
    }
    inline Expression & Assign(const string &str) {
        type = PropString;
        val_string = str;
        return *this;
    }
    inline Expression & Assign(const char *str) {
        return Assign(string(str));
    }
    inline Expression & AssignBool() {
        type = PropBool;
        val_bool = Bool(Undefined);
        return *this;
    }
    inline Expression & AssignBool(const bool &b) {
        type = PropBool;
        val_bool = Bool(b);
        return *this;
    }
    inline Expression & AssignBool(const Bool &b) {
        type = PropBool;
        val_bool = b;
        return *this;
    }

    inline Expression & AssignParameter(const string &str) {
        type = PropParameter;
        name = str;
        return *this;
    }

    inline Expression & AssignExpOp(const char &op) {
        type = PropOp;
        if (op == '=')
            cmp_op = Eq;
        else if (op == '>')
            cmp_op = Ge;
        else if (op == '<')
            cmp_op = Le;
        else
            assert(false);
        return *this;
    }

    inline Expression & AssignOp(const char &op) {
        type = PropOp;
        if(op == '=')
            cmp_op = Eq;
        else if(op == '>')
            cmp_op = Gt;
        else if(op == '<')
            cmp_op = Lt;
        else if(op == '&')
            cmp_op = And;
        else if(op == '|')
            cmp_op = Or;
        else
            assert(false);
        return *this;
    }

    inline Expression LeftBracket() {
        type = PropLeftBracket;
        return *this;
    }

    inline friend ostream & operator << (ostream &w, const Expression &exp) {
        switch (exp.type) {
            case PropInt:
                return w << exp.val_int << " ";
            case PropFloat:
                return w << exp.val_float << " ";
            case PropString:
                return w << "\'" << exp.val_string << "\' ";
            case PropBool:
                static const string bls[] = {"True", "False", "Undefined"};
                return w << bls[exp.val_bool.ans] << " ";
            case PropParameter:
                return w << exp.name << " ";
            case PropOp:
                static const string ops[] = {"=", ">=", "<=", ">", "<", "&", "|"};
                return w << ops[exp.cmp_op] << " ";
            case PropLeftBracket:
                return w << '(' << " ";
            default:
                break;
        }
        return w << "??? ";
    }

    template <typename T>
    inline Expression & CalcEq(const T &a, const T &b) {
        return AssignBool(a == b);
    }
    template <typename T>
    inline Expression & CalcGe(const T &a, const T &b) {
        return AssignBool(a >= b);
    }
    template <typename T>
    inline Expression & CalcGt(const T &a, const T &b) {
        return AssignBool(a > b);
    }
    inline Expression & CalcAnd(const Bool &a, const Bool &b) {
        return AssignBool(a && b);
    }
    inline Expression & CalcAnd(const string &a, const string &b) {
        return AssignBool(a.length() && b.length());
    }
    inline Expression & CalcAnd(const float &a, const float &b) {
        return AssignBool(abs(a) > 0.0001 && abs(b) > 0.0001);
    }
    inline Expression & CalcAnd(const int &a, const int &b) {
        return AssignBool(a && b);
    }
    inline Expression & CalcOr(const Bool &a, const Bool &b) {
        return AssignBool(a || b);
    }
    inline Expression & CalcOr(const string &a, const string &b) {
        return AssignBool(a.length() || b.length());
    }
    inline Expression & CalcOr(const float &a, const float &b) {
        return AssignBool(abs(a) > 0.0001 || abs(b) > 0.0001);
    }
    inline Expression & CalcOr(const int &a, const int &b) {
        return AssignBool(a || b);
    }

    inline string ToString() const {
        assert(type == PropString);
        return val_string;
    }
    inline float ToFloat() const {
        return val_float;
    }
    inline Bool ToBool() const {
        if(type == PropBool)
            return val_bool;
        if (type == PropString)
            return Bool(val_string.length() > 0);
        if (type == PropFloat)
            return Bool(abs(val_float) > 0.0001);
        if (type == PropInt)
            return Bool(val_int != 0);
        assert(false);
        return Bool(Undefined);
    }

    template <class T>
    inline Expression & Exec(const T &a, const T &b, const CmpOp &op) {
        switch (op) {
            case Eq:
                return CalcEq(a, b);
            case Ge:
                return CalcGe(a, b);
            case Le:
                return CalcGe(b, a);
            case Gt:
                return CalcGt(a, b);
            case Lt:
                return CalcGt(b, a);
            case Or:
                return CalcOr(a, b);
            case And:
                return CalcAnd(a, b);
            default:
                assert(false);
        }
        return *this;
    }

    inline Expression & Calc(const CmpOp &op, const Expression &rhs) {
        if (type == PropBool || rhs.type == PropBool)
            return Exec(ToBool(), rhs.ToBool(), op);
        if (type == PropString || rhs.type == PropString)
            return Exec(ToString(), rhs.ToString(), op);
        if (type == PropFloat || rhs.type == PropFloat)
            return Exec(ToFloat(), rhs.ToFloat(), op);
        return Exec(val_int, rhs.val_int, op);
    }
};

struct Expressions : public vector<Expression> {
    template<typename T>
    class Stack {
        size_t capacity;
        // TODO: use pointer, not offset, can be faster.
        size_t tail;
        T *content;

    public:
        inline Stack(size_t capacity_ = 256) : tail(0), capacity(capacity_) {
            content = new T[capacity];
        }
        inline ~Stack() {
            delete[] content;
        }

        inline size_t Size() {
            return tail;
        }
        inline bool Empty() {
            return !tail;
        }

        inline const T Top() {
            if(tail == 0) {
                assert(false);
                return T();
            }
            return content[tail - 1];
        }
        inline const T Pop() {
            if (tail == 0) {
                assert(false);
                return T();
            }
            return content[--tail];
        }
        inline void Push(const T &x) {
            if (tail == capacity) {
                auto *tmp = new T[capacity << 1];
                memcpy(content, tmp, sizeof(T) * capacity);
                delete[] content;
                content = tmp;
                capacity <<= 1;
            }
            content[tail++] = x;
        }

        inline friend ostream & operator << (ostream &w, const Stack &x) {
            for (int i = (int)(x.tail - 1); i >= 0; --i)
                w << x.content[i];
            return w;
        }
    };

    using Self = vector<Expression>;

    using CmpOp = Expression::CmpOp;

    using PropValInt = Expression::PropValInt;
    using PropValFloat = Expression::PropValFloat;

    inline bool IsW(const char &c) {
        return isalpha(c) || isdigit(c) || c == '_';
    }
    inline bool IsD(const char &c) {
        return isdigit(c) || c == '.';
    }

    inline friend ostream & operator << (ostream &w, const Expressions &exps) {
        for (const Expression &exp: exps)
            w << exp;
        return w;
    }

    inline static int Prior(const Expression &exp) {
        if (exp.type != Expression::PropOp)
            return 100;
        if (exp.type == Expression::PropLeftBracket)
            return 2;
        if (exp.cmp_op == Expression::And || exp.cmp_op == Expression::Or)
            return 0;
        return 1;
    }

    inline void Parse(const char *in){
        Self::clear();
        Stack<Expression> stack;
        char g = 0;
        auto len = (int)strlen(in);

        for (int i = 0; i < len; ) {
            g = in[i];
            if (isblank(g)) {
                ++i;
                continue;
            }
            Expression ret;
            if (isalpha(g)) {
                string ans;
                while (IsW(g = in[i++]))
                    ans += (char) tolower(g);
                --i;
                Self::emplace_back(ret.AssignParameter(ans));
            } else if (isdigit(g) || g == '-') {
                PropValInt ans = 0, fac = 1;
                PropValFloat ansFloat = 0;
                if (g == '-') {
                    fac = -1;
                    ++i;
                }
                int cnt = 0;
                float d = 1;
                while (IsD(g = in[i ++])) {
                    if (g == '.') {
                        ansFloat = ans;
                        ++cnt;
                    }
                    else if (cnt == 0) {
                        ans = (ans << 3) + (ans << 1) + g - 48;
                    } else {
                        ansFloat += d * (g - 48);
                        d *= 0.1;
                    }
                }
                --i;
                if (cnt == 1)
                    Self::emplace_back(ret.Assign(fac * ansFloat));
                else if (cnt == 0)
                    Self::emplace_back(ret.Assign(fac * ans));
                else
                    assert(false);
            } else if(g == '\'') {
                // TODO: solve complex case
                string ans;
                ++i;
                while ((g = in[i ++]) != '\'')
                    ans += g;
                Self::emplace_back(ret.Assign(ans));
            } else if (g == '(') {
                ++i;
                stack.Push(ret.LeftBracket());
            } else if (g == ')') {
                ++i;
                while(!stack.Empty() && stack.Top().type != Expression::PropLeftBracket)
                    Self::emplace_back(stack.Pop());
                stack.Pop();
            } else if (g == '&' || g == '|') {
                ++i;
                // TODO: 'Expression::Prior(stack.Top()) >= 0' always true
                while (!stack.Empty() && stack.Top().type != Expression::PropLeftBracket && Prior(stack.Top()) >= 0)
                    Self::emplace_back(stack.Pop());
                stack.Push(ret.AssignOp(g));
            } else if (g == '=' || g == '<' || g == '>') {
                while (!stack.Empty() && stack.Top().type != Expression::PropLeftBracket && Prior(stack.Top()) >= 1)
                    Self::emplace_back(stack.Pop());
                ++i;
                if (in[i] == '=') {
                    ++i;
                    stack.Push(ret.AssignExpOp(g));
                } else {
                    stack.Push(ret.AssignOp(g));
                }
            } else {
                assert(false);
            }
        }

        while (!stack.Empty())
            Self::emplace_back(stack.Pop());
        // std::cout << *this << std::endl;
    }

    template <typename iterable>
    inline bool Matched(iterable props) {
        unordered_map<string, Expression> val;
        // TODO: Assign => constructor
        Expression exp;
        for (auto it = props.begin(); it != props.end(); ++ it) {
            auto type = it->Type();
            string name(it->Name());
            if (type == Expression::PropString)
                val.insert(make_pair(name, exp.Assign(it->String())));
            else if (type == Expression::PropInt)
                val.insert(make_pair(name, exp.Assign(it->Int())));
            else if (type == Expression::PropFloat)
                val.insert(make_pair(name, exp.Assign((float)(it->Float()))));
            else
                assert(false);
        }

        Stack<Expression> stack;
        for (auto &exp: *this) {
            if (exp.type == Expression::PropParameter) {
                stack.Push((val.find(exp.name) == val.end()) ? exp.AssignBool() : val[exp.name]);
                continue;
            }
            if (exp.type != Expression::PropOp) {
                stack.Push(exp);
                continue;
            }
            Expression t1 = stack.Pop();
            Expression t2 = stack.Pop();
            // std::cerr << t2 << exp << t1;
            stack.Push(t2.Calc(exp.cmp_op, t1));
            // std::cerr << t2 << std::endl;
        }

        // assert(stack.Size() == 1);
        return stack.Top().val_bool.ans != Expression::False;
    }
};
