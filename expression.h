#pragma once

#include <iostream>
#include <string>
#include <vector>

using std::vector;
using std::ostream;
using std::abs;

struct Expression {
    using PropType = uint8_t;
    using PropValInt = int32_t;
    using PropValFloat = float;

    enum {
        PropString = 0,
        PropInt = 1,
        PropFloat = 2,
        PropBool = 3,
        PropParameter = 4,
        PropOp = 5,
        PropLeftBracket = 6,
        PropNone = 255,
    };

    enum CmpOp {Eq, Ge, Le, Gt, Lt, Or, And};

    enum ReturnType {
        Undefined,
        False,
        True,
    };

    struct Bool {
        ReturnType ans;

        inline explicit Bool(const ReturnType &x) : ans(x) {}
        inline explicit Bool(const bool &x) : ans(x ? True : False) {}

        inline Bool operator && (const Bool &rhs) const {
            if (ans == Undefined)
                return rhs;
            return (rhs.ans == Undefined) ? *this : Bool(ans == True && rhs.ans == True);
        }
        inline Bool operator || (const Bool &rhs) const {
            if (ans == Undefined)
                return rhs;
            return (rhs.ans == Undefined) ? *this : Bool(ans == True || rhs.ans == True);
        }
        inline Bool operator == (const Bool &rhs) const {
            return (ans == Undefined || rhs.ans == Undefined) ? Bool(True) : Bool(ans == rhs.ans);
        }
        inline Bool operator >= (const Bool &rhs) const {
            return (ans == Undefined || rhs.ans == Undefined) ? Bool(True) : Bool(ans >= rhs.ans);
        }
        inline Bool operator <= (const Bool &Rhs) const {
            return (ans == Undefined || Rhs.ans == Undefined) ? Bool(True) : Bool(ans <= Rhs.ans);
        }
        inline Bool operator > (const Bool &Rhs) const {
            return (ans == Undefined || Rhs.ans == Undefined) ? Bool(True) : Bool(ans > Rhs.ans);
        }
        inline Bool operator < (const Bool &Rhs) const {
            return (ans == Undefined || Rhs.ans == Undefined) ? Bool(True) : Bool(ans < Rhs.ans);
        }
    };

    using HashCode = uint32_t;

    PropType type;
    CmpOp cmp_op;
    HashCode name;

    HashCode val_string;
    PropValInt val_int;
    PropValFloat val_float;
    Bool val_bool;

    inline Expression(): type(PropBool), val_bool(Undefined) {}

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
    inline Expression & Assign(const HashCode &hashcode) {
        type = PropString;
        val_string = hashcode;
        return *this;
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

    inline Expression & AssignParameter(const HashCode &hashcode) {
        type = PropParameter;
        name = hashcode;
        return *this;
    }

    inline Expression & AssignExpOp(const char &op) {
        type = PropOp;
        if (op == '>')
            cmp_op = Ge;
        else if (op == '<')
            cmp_op = Le;
        else if (op == '=')
            cmp_op = Eq;
        else
            assert(false);
        return *this;
    }

    inline Expression & AssignOp(const char &op) {
        type = PropOp;
        if (op == '&')
            cmp_op = And;
        else if (op == '|')
            cmp_op = Or;
        else if (op == '=')
            cmp_op = Eq;
        else if (op == '>')
            cmp_op = Gt;
        else if (op == '<')
            cmp_op = Lt;
        else
            assert(false);
        return *this;
    }

    inline Expression LeftBracket() {
        type = PropLeftBracket;
        return *this;
    }

    inline friend ostream & operator << (ostream &w, const Expression &exp) {
        static const std::string bls[] = {"Undefined", "False", "True"};
        static const std::string ops[] = {"=", ">=", "<=", ">", "<", "|", "&"};
        switch (exp.type) {
            case PropInt:
                return w << exp.val_int << " ";
            case PropFloat:
                return w << exp.val_float << " ";
            case PropString:
                return w << "\'" << exp.val_string << "\' ";
            case PropBool:
                assert(exp.val_bool.ans >= 0 && exp.val_bool.ans < 3);
                return w << bls[exp.val_bool.ans] << " ";
            case PropParameter:
                return w << exp.name << " ";
            case PropOp:
                assert(exp.cmp_op >= 0 && exp.cmp_op < 7);
                return w << ops[exp.cmp_op] << " ";
            case PropLeftBracket:
                return w << '(' << " ";
            default:
                break;
        }
        assert(false);
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
    template <typename T>
    inline Expression & CalcAnd(const T &a, const T &b) {
        return AssignBool(a && b);
    }
    template <typename T>
    inline Expression & CalcOr(const T &a, const T &b) {
        return AssignBool(a || b);
    }
    inline Expression & CalcAnd(const Bool &a, const Bool &b) {
        return AssignBool(a && b);
    }
    inline Expression & CalcOr(const Bool &a, const Bool &b) {
        return AssignBool(a || b);
    }

    template <class T>
    inline Expression & Exec(const T &a, const T &b, const CmpOp &op) {
        switch (op) {
            case Or:
                return CalcOr(a, b);
            case And:
                return CalcAnd(a, b);
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
            default:
                assert(false);
        }
        return *this;
    }

    inline Expression & Calc(const CmpOp &op, const Expression &rhs) {
        if (type == PropBool)
            return Exec(val_bool, rhs.val_bool, op);
        if (type == PropString)
            return Exec(val_string, rhs.val_string, op);
        if (type == PropFloat || rhs.type == PropFloat)
            return Exec(val_float, rhs.val_float, op);
        return Exec(val_int, rhs.val_int, op);
    }
};

class Expressions: public vector<Expression> {
    using Self = vector<Expression>;

    using CmpOp = Expression::CmpOp;
    using HashCode = Expression::HashCode;

    using PropValInt = Expression::PropValInt;
    using PropValFloat = Expression::PropValFloat;

    template<typename T>
    class Stack {
        size_t capacity;
        T *tail;
        T *content;

    public:
        inline explicit Stack(size_t capacity_ = 8) : capacity(capacity_) {
            content = new T[capacity];
            tail = content;
        }
        inline ~Stack() {
            delete[] content;
        }

        inline size_t Size() {
            return tail - content;
        }
        inline bool Empty() {
            return tail == content;
        }

        inline const T Top() {
            if (Empty()) {
                assert(false);
                return T();
            }
            return *(tail - 1);
        }
        inline const T Pop() {
            if (Empty()) {
                assert(false);
                return T();
            }
            return *(--tail);
        }
        inline void Push(const T &x) {
            if (Size() == capacity) {
                auto *tmp = new T[capacity << 1];
                memcpy(content, tmp, sizeof(T) * capacity);
                delete[] content;
                content = tmp;
                tail = content + capacity;
                capacity <<= 1;
            }
            *tail = x;
            ++tail;
        }

        inline friend ostream & operator << (ostream &w, Stack &x) {
            if (!x.Empty()) {
                for (auto p = x.tail - 1; ; --p) {
                    w << *p << " ";
                    if (p == x.content) {
                        break;
                    }
                }
            }
            return w;
        }
    };

    class Pair {
    public:
        HashCode name;
        Expression exp;
        inline Pair(): name(0), exp() {}
        inline Pair(const HashCode &name_, const Expression &exp_): name(name_), exp(exp_) {}

        inline void set(const HashCode &name_, const Expression &exp_) {
            name = name_;
            exp = exp_;
        }
        inline bool operator < (const Pair& rhs) const {
            return name < rhs.name;
        }
        inline bool operator == (const Pair& rhs) const {
            return name == rhs.name;
        }
    };

    inline bool IsW(const char &c) {
        return isalpha(c) || isdigit(c) || c == '_';
    }
    inline bool IsD(const char &c) {
        return isdigit(c) || c == '.';
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

    inline Expression get(Pair *src_begin, Pair *src_end, const HashCode &hashcode) {
        Expression exp;
        Pair p(hashcode, exp.AssignBool());
        Pair *ret = std::lower_bound(src_begin, src_end, p);

        if (ret != src_end && ret->name == hashcode) {
            return ret->exp;
        } else {
            return exp;
        }
    }

public:

    inline friend ostream & operator << (ostream &w, const Expressions &exps) {
        for (const Expression &exp: exps) {
            w << exp << " ";
        }
        return w;
    }

    void Parse(const char *in) {
        Self::clear();
        Stack<Expression> stack;
        Expression ret;
        char g = 0;
        auto len = (int)strlen(in);
        for (int i = 0; i < len; ) {
            g = in[i];
            if (isblank(g)) {
                ++i;
                continue;
            }

            if (isalpha(g)) {
                HashCode hashcode = 0;
                while (IsW(in[i]))
                    hashcode = hashcode * 131U + in[i ++];
                Self::emplace_back(ret.AssignParameter(hashcode));
            } else if (isdigit(g) || g == '-') {
                PropValInt ans = 0, fac = 1;
                PropValFloat ansFloat = 0;
                if (g == '-') {
                    fac = -1;
                    ++i;
                }
                int cnt = 0;
                float d = 1;
                while (IsD(g = in[i])) {
                    if (g == '.') {
                        ansFloat = ans;
                        ++cnt;
                    } else if (cnt == 0) {
                        ans = ans * 10 + g - 48;
                    } else {
                        ansFloat = ansFloat * 10 + (g - 48);
                        d *= 0.1;
                    }
                    ++i;
                }
                if (cnt == 1)
                    Self::emplace_back(ret.Assign(fac * ansFloat * d));
                else if (cnt == 0)
                    Self::emplace_back(ret.Assign(fac * ans));
                else
                    assert(false);
            } else if (g == '\'') {
                // TODO: solve complex case
                ++i;
                int j = 0;
                HashCode hashcode = 0;
                while ((g = in[i++]) != '\'')
                    hashcode = hashcode * 131U + g;
                Self::emplace_back(ret.Assign(hashcode));
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
                while (!stack.Empty() && stack.Top().type != Expression::PropLeftBracket)
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
    }

    template <typename iterable>
    inline bool Match(const iterable &props) {
        Expression exp;
        int cnt = 0, tot = props.size();
        Pair used[tot];
        for (auto it = props.begin(); cnt < tot; ++it, ++cnt) {
            auto type = it->Type();
            HashCode hashcode = 0;
            const char *p = it->Name();
            int len = it->NameLen();
            for (int i = 0; i < len; ++i) {
                hashcode = hashcode * 131U + p[i];
            }
            if (type == Expression::PropString) {
                HashCode hashcode2 = 0;
                const char *q = it->String();
                len = it->ValLen();
                for (int i = 0; i < len; ++i) {
                    hashcode2 = hashcode2 * 131U + q[i];
                }
                used[cnt].set(hashcode, exp.Assign(hashcode2));
            } else if (type == Expression::PropInt) {
                used[cnt].set(hashcode, exp.Assign(it->Int()));
            } else if (type == Expression::PropFloat) {
                used[cnt].set(hashcode, exp.Assign(it->Float()));
            } else {
                assert(false);
            }
        }
        std::sort(used, used + cnt);

        Stack<Expression> stack;
        for (const Expression &e: *this) {
            if (e.type == Expression::PropParameter) {
                stack.Push(get(used, used + cnt, e.name));
            } else if (e.type != Expression::PropOp) {
                stack.Push(e);
            } else {
                Expression t1 = stack.Pop();
                Expression t2 = stack.Pop();
                stack.Push(t2.Calc(e.cmp_op, t1));
            }
        }

        // assert(stack.Size() == 1);
        return stack.Top().val_bool.ans != Expression::False;
    }
};
