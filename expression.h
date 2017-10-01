#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

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
    char *name;

    char *val_string;
    PropValInt val_int;
    PropValFloat val_float;
    Bool val_bool;

    inline Expression(): type(PropNone), cmp_op(Nd), val_int(0), val_float(0), val_bool(Undefined), name(0), val_string(0) {}

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
    inline Expression & Assign(const char *str) {
        type = PropString;
        for(int i = 0; str[i]; i ++) {
            val_string[i] = str[i];
        }
        return *this;
    }
//    inline Expression & Assign(const char *str) {
//        return Assign(string(str));
//    }
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

    inline Expression & AssignParameter(char *str) {
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
                static const std::string bls[] = {"True", "False", "Undefined"};
                return w << bls[exp.val_bool.ans] << " ";
            case PropParameter:
                return w << exp.name << " ";
            case PropOp:
                static const std::string ops[] = {"=", ">=", "<=", ">", "<", "&", "|"};
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
    inline Expression & CalcAnd(const char *a, const char *b) {
        return AssignBool(strlen(a) && strlen(b));
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
    inline Expression & CalcOr(const char *a, const char *b) {
        return AssignBool(strlen(a) || strlen(b));
    }
    inline Expression & CalcOr(const float &a, const float &b) {
        return AssignBool(abs(a) > 0.0001 || abs(b) > 0.0001);
    }
    inline Expression & CalcOr(const int &a, const int &b) {
        return AssignBool(a || b);
    }

    inline const char *ToString() const {
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
            return Bool(strlen(val_string) > 0);
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
        T *tail;
        T *content;

    public:
        inline explicit Stack(size_t capacity_ = 256) : capacity(capacity_) {
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
            if(Empty()) {
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
            return *(-- tail);
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
            ++ tail;
        }

        inline friend ostream & operator << (ostream &w, Stack &x) {
            if(!x.Empty()) {
                for (auto p = x.tail - 1; p != x.content; --p)
                    w << *p << " ";
            }
            return w;
        }
    };

    template<typename K, typename V>
    class hashMap {
        size_t capacity;
        struct node {
            K key;
            V val;
            node *next;
            node(): next(nullptr) {}
            node(const K &key_, const V &val_): key(key_), val(val_), next(nullptr) {}
        };
        node *tail;
#define HASH_SIZE 233
        node *content[HASH_SIZE];
        node *pool;

    public:
        inline explicit hashMap(size_t capacity_ = 256) : capacity(capacity_) {
            pool = new node[capacity];
            for(auto &g: content) {
                g = nullptr;
            }
            tail = pool;
        }

        inline size_t Size() {
            return tail - pool;
        }
        inline bool Empty() {
            return tail == pool;
        }

        //please ensure that you used find first
        inline V get(const K &key) {
            size_t hashcode = std::hash<K>()(key) % HASH_SIZE;
            node *p = content[hashcode];
            for(; p; p = p->next) {
                if(p->key == key) {
                    return p->val;
                }
            }
            assert(false);
            return V();
        }
        inline bool find(K &key) {
            size_t hashcode = std::hash<K>()(key) % HASH_SIZE;
            node *p = content[hashcode];
            if(p == nullptr) {
                return false;
            }
            for(; p; p = p->next) {
                if(p->key == key) {
                    return true;
                }
            }
            return false;
        }

        inline void insert(const K &key, const V &val) {
            std::cout << key << std::endl;
            size_t hashcode = std::hash<K>()(key) % HASH_SIZE;
            std::cout << key << std::endl;
            node *p = content[hashcode];
            if(p == nullptr) {
                tail->key = key;
                tail->val = val;
                tail->next = nullptr;
                content[hashcode] = tail;
                ++ tail;
                return;
            }
            node *prev = p;
            for(; p; p = p->next) {
                if(p->key == key) {
                    p->val = val;
                    return;
                }
                prev = p;
            }
            // TODO: to support dynamic allocation
            prev->next = tail;
            tail->key = key;
            tail->val = val;
            tail->next = nullptr;
            ++ tail;
        }

        inline friend ostream & operator << (ostream &w, hashMap &x) {
            if(!x.Empty()) {
                for (auto p = x.tail - 1; p != x.pool; --p)
                    w << "(" << p->key << ", " << p->val << ") ";
            }
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

    inline hashMap<const char *, Expression> Parse(const char *in){
        Self::clear();
        Stack<Expression> stack;
        hashMap<const char *, Expression> dict;
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
                char ans[64];
                int j = 0;
                while (IsW(g = in[i++]))
                    ans[j ++] = (char) tolower(g);
                ans[j] = 0;
                -- i;
//                const_cast<>(ans);
                dict.insert(ans, ret.AssignBool());
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
                char ans[64];
                ++ i;
                int j = 0;
                while ((g = in[i ++]) != '\'')
                    ans[j ++] = g;
                ans[j] = 0;
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
        return dict;
    }

    template <typename iterable>
    inline bool Matched(iterable props, hashMap<const char *, Expression> val) {
        // TODO: Assign => constructor
        Expression exp;
        for (auto it = props.begin(); it != props.end(); ++ it) {
            auto type = it->Type();
            const char *name = it->Name();
            if (!val.find(name)) {
                continue;
            }
            if (type == Expression::PropString)
                val.insert(name, exp.Assign(it->String()));
            else if (type == Expression::PropInt)
                val.insert(name, exp.Assign(it->Int()));
            else if (type == Expression::PropFloat)
                val.insert(name, exp.Assign(it->Float()));
            else
                assert(false);
        }

        Stack<Expression> stack;
        for (auto &e: *this) {
            if (e.type == Expression::PropParameter) {
                stack.Push(val.get(e.name));
            } else if (e.type != Expression::PropOp) {
                stack.Push(e);
            } else {
                Expression t1 = stack.Pop();
                Expression t2 = stack.Pop();
                 std::cerr << t2 << exp << t1;
                stack.Push(t2.Calc(e.cmp_op, t1));
                 std::cerr << t2 << std::endl;
            }
        }

        // assert(stack.Size() == 1);
        return stack.Top().val_bool.ans != Expression::False;
    }
};
