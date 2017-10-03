#include <string>
#include <vector>

#include "apps/analysys/prop.h"

namespace analysys {

using std::string;
using std::vector;

struct RawExp {
    using PropType = CompactProp::PropType;
    using PropValInt = CompactProp::PropValInt;
    using PropValFloat = CompactProp::PropValFloat;

    enum {
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
            return RawExp::ExeEq(a, b);
        case Ge:
            return RawExp::ExeGe(a, b);
        case Le:
            return RawExp::ExeLe(a, b);
        case Gt:
            return RawExp::ExeGt(a, b);
        case Lt:
            return RawExp::ExeLt(a, b);
        default:
            return false;
        }
    }

    template <typename iteratable>
    inline bool Match(iteratable props) const {
        for (auto it = props.begin(); it != props.end(); ++it) {
            int name_match = strncmp(it->Name(), name.c_str(), it->NameLen());
            if (name_match > 0)
                break;
            if (name_match != 0)
                continue;
            if (type != it->Type())
                return false;
            if (it->Type() == PropString)
                return strncmp(it->String(), val_string.c_str(), it->ValLen()) == 0;
            else if (it->Type() == PropInt)
                return RawExp::Cmp(it->Int(), cmp_op, val_int);
            else if (it->Type() == PropFloat)
                return RawExp::Cmp(it->Float(), cmp_op, val_float);
            break;
        }
        return false;
    }

    inline RawExp(const char *name_, CmpOp cmp_op_, const char *val_) :
        cmp_op(cmp_op_), name(name_), type(PropString), val_string(val_) {}

    inline RawExp(const char *name_, CmpOp cmp_op_, PropValInt val_) :
        cmp_op(cmp_op_), name(name_), type(PropInt), val_int(val_) {}

    inline RawExp(const char *name_, CmpOp cmp_op_, PropValFloat val_) :
        cmp_op(cmp_op_), name(name_), type(PropFloat), val_float(val_) {}
};

// Usage exp.And("brand", Expression::Eq, "Apple");
struct RawExps : public vector<RawExp> {
    using Self = vector<RawExp>;

    using CmpOp = RawExp::CmpOp;

    using PropValInt = RawExp::PropValInt;
    using PropValFloat = RawExp::PropValFloat;

    inline RawExps & And(const char *name, CmpOp cmp, const char *val) {
        if (cmp != RawExp::Eq)
            throw ErrWrongUsage("onlys support 'eq' compare on strings");
        Self::emplace_back(name, cmp, val);
        return *this;
    }

    inline RawExps & And(const char *name, CmpOp cmp, PropValInt val) {
        Self::emplace_back(name, cmp, val);
        return *this;
    }

    inline RawExps & And(const char *name, CmpOp cmp, PropValFloat val) {
        Self::emplace_back(name, cmp, val);
        return *this;
    }

    template <typename iteratable>
    inline bool Match(iteratable props) const {
        if (Self::empty())
            return false;
        for (auto it = Self::begin(); it != Self::end(); ++it)
            if (!it->Match(props))
                return false;
        return true;
    }
};

}
