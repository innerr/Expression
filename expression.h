#pragma once

#include "rapidjson/document.h"

namespace ptio {

struct Expression {
    using Dict = const rapidjson::Document;

    Expression(const char *in) {
    }

    bool Matched(const Dict &dict) {
        return false;
    }
};

}
