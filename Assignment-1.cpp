#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cmath>
#include "rapidjson/document.h" 

using namespace std;
using namespace rapidjson;
int convertToDecimal(const string& value, int base) {
    int result = 0;
    int power = 1;
    for (int i = value.length() - 1; i >= 0; --i) {
        int digit = (isdigit(value[i])) ? (value[i] - '0') : (toupper(value[i]) - 'A' + 10);
        result += digit * power;
        power *= base;
    }
    return result;
}


double lagrangeInterpolation(const vector<pair<int, int>>& points) {
    double result = 0.0;
    int n = points.size();

    for (int i = 0; i < n; ++i) {
        double term = points[i].second;
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                term *= -points[j].first / double(points[i].first - points[j].first);
            }
        }
        result += term;
    }
    return result;
}

int main() {
    
    const char* jsonInput = R"({
        "keys": {
            "n": 4,
            "k": 3
        },
        "1": {
            "base": "10",
            "value": "4"
        },
        "2": {
            "base": "2",
            "value": "111"
        },
        "3": {
            "base": "10",
            "value": "12"
        },
        "6": {
            "base": "4",
            "value": "213"
        }
    })";

    
    Document document;
    document.Parse(jsonInput);

    if (!document.IsObject()) {
        cerr << "Invalid JSON format." << endl;
        return 1;
    }

    
    const Value& keys = document["keys"];
    int n = keys["n"].GetInt();
    int k = keys["k"].GetInt();

    vector<pair<int, int>> points;

    
    for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
        string key = itr->name.GetString();
        if (key == "keys") continue;  

        int x = stoi(key);  
        int base = itr->value["base"].GetInt();
        string valueStr = itr->value["value"].GetString();
        int y = convertToDecimal(valueStr, base);  

        points.emplace_back(x, y);
    }

    
    if (points.size() < k) {
        cerr << "Insufficient points provided for polynomial interpolation." << endl;
        return 1;
    }
    double constantTerm = lagrangeInterpolation(points);

    cout << "The constant term c is: " << constantTerm << endl;

    return 0;
}
