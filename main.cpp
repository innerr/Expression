#include <iostream>
#include <iomanip>
#include "rapidjson/document.h"
#include "expression.h"

struct Dict {
public:
    using Json = rapidjson::Document;

    Json &doc;

    explicit Dict(Json &doc_) : doc(doc_) {}

    struct iterator {
        using It = rapidjson::Value::ConstMemberIterator;

        It it;

        explicit iterator(const It &it_) : it(it_) {}

        bool operator != (const iterator &x) {
            return it != x.it;
        }
        void operator ++ () {
            ++it;
        }
        iterator* operator -> () {
            return this;
        }
        Expression::PropType Type() {
            auto type = it->value.GetType();
            if (type == rapidjson::kStringType)
                return Expression::PropString;
            if (type == rapidjson::kNumberType && it->value.IsDouble())
                return Expression::PropFloat;
            if (type == rapidjson::kNumberType)
                return Expression::PropInt;
            return Expression::PropNone;
        }
        const char * Name() {
            return it->name.GetString();
        }
        const char * String() {
            return it->value.GetString();
        }
        Expression::PropValInt Int() {
            return (Expression::PropValInt)it->value.GetInt();
        }
        Expression::PropValFloat Float() {
            return (Expression::PropValFloat)it->value.GetDouble();
        }
    };

    iterator begin() {
        return iterator(doc.MemberBegin());
    }

    iterator end() {
        return iterator(doc.MemberEnd());
    }
};

int main() {
    const char* expression = "(brand = 'Apple' & price > 6000) | (brand = 'HW' | price > 5000)";
    const char *data = R"({"brand": "Apple", "price": 8888.8})";

    Expressions exp;
    Expressions::hashMap<unsigned long, Expression> val = exp.Parse(expression);
    std::cout << exp << std::endl;

    rapidjson::Document row;
    row.Parse(data);

    Dict d(row);

    time_t start = clock(), end;

    int testCases = 1000000;
//    bool matched = false;
    for(int T = 0; T < testCases; T ++) {
        bool matched = exp.Matched(d, val);
        printf("%d\n", matched);
    }

    end = clock();
    std::cerr << std::fixed << std::setprecision(3) << (end - start + 0.0) / CLOCKS_PER_SEC << "s " << std::endl;

    return 0;
}
