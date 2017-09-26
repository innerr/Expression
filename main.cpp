#include <iostream>
#include "rapidjson/document.h"
#include "expression.h"

using namespace ptio;

int main() {
    const char *expression = "(brand = 'Apple' & price > 6000) | (brand = 'HW' & price > 5000)";
    const char *data = "{\"brand\": \"Apple\", \"price\": 8888.8.8}";

    ptio::Expression exp(expression);

    rapidjson::Document row;
    row.Parse(data);

    bool matched = exp.Matched(row);
    std::cout << "{" << data << "} " << (matched ? "" : "not") << " matched {" << expression << "}" << std::endl;

    return 0;
}
