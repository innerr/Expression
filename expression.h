#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

struct Expression {
    using PropType = int;
    using PropValInt = uint32_t;
    using PropValFloat = float;

    enum {
        PropNone = -1,
        PropString = 0,
        PropInt = 1,
        PropFloat = 2,
    };

    using CmpOp = int;

    enum {
        Eq = 0,
        Ge = 1,
        Le = 2,
        Gt = 3,
        Lt = 4,
    };

    CmpOp cmp_op;

    string name;
    PropType type;

    PropValInt val_int;
    PropValFloat val_float;
    string val_string;

    template <typename T>
    inline static bool ExeEq(const T &a, const T &b) {
        return a == b;
    }
    template <typename T>
    inline static bool ExeGe(const T &a, const T &b) {
        return a >= b;
    }
    template <typename T>
    inline static bool ExeLe(const T &a, const T &b) {
        return a <= b;
    }
    template <typename T>
    inline static bool ExeGt(const T &a, const T &b) {
        return a > b;
    }
    template <typename T>
    inline static bool ExeLt(const T &a, const T &b) {
        return a < b;
    }
    template <typename T>
    inline static bool Cmp(const T &a, CmpOp op, const T &b) {
        switch (op) {
        case Eq:
            return Expression::ExeEq(a, b);
        case Ge:
            return Expression::ExeGe(a, b);
        case Le:
            return Expression::ExeLe(a, b);
        case Gt:
            return Expression::ExeGt(a, b);
        case Lt:
            return Expression::ExeLt(a, b);
        default:
            return false;
        }
    }

    template <typename iteratable>
    inline bool Matched(iteratable props) {
        for (auto it = props.begin(); it != props.end(); ++it) {
            if (type != it->Type() || name != it->Name())
                continue;
            if (it->Type() == PropString)
                return Expression::Cmp(val_string, cmp_op, string(it->String()));
            else if (it->Type() == PropInt)
                return Expression::Cmp(val_int, cmp_op, it->Int());
            else if (it->Type() == PropFloat)
                return Expression::Cmp(val_float, cmp_op, it->Float());
            else
                return false;
        }
        return true;
    }

    inline Expression(const char *name_, CmpOp cmp_op_, const char *val_) :
        cmp_op(cmp_op_), name(name_), type(PropString), val_string(val_) {}

    inline Expression(const char *name_, CmpOp cmp_op_, PropValInt val_) :
        cmp_op(cmp_op_), name(name_), type(PropInt), val_int(val_) {}

    inline Expression(const char *name_, CmpOp cmp_op_, PropValFloat val_) :
        cmp_op(cmp_op_), name(name_), type(PropFloat), val_float(val_) {}
};

struct Expressions : public vector<Expression> {
    using Self = vector<Expression>;

    using CmpOp = Expression::CmpOp;

    using PropValInt = Expression::PropValInt;
    using PropValFloat = Expression::PropValFloat;

    inline Expressions & And(const char *name, CmpOp cmp, const char *val) {
        Self::emplace_back(name, cmp, val);
        return *this;
    }

    inline Expressions & And(const char *name, CmpOp cmp, PropValInt val) {
        Self::emplace_back(name, cmp, val);
        return *this;
    }

    inline Expressions & And(const char *name, CmpOp cmp, PropValFloat val) {
        Self::emplace_back(name, cmp, val);
        return *this;
    }

    template <typename iteratable>
    inline bool Matched(iteratable props) {
        for (auto it = Self::begin(); it != Self::end(); ++it)
            if (!it->Matched(props))
                return false;
        return true;
    }

    inline void Parse(const char *in) {
        // TODO: parse
        // And("brand", Eq, "Apple").And("price", Lt, 3000);
    }
};
