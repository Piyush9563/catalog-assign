#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <cmath>
#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;


unsigned long long convertToDecimal(const string& value, int base) {
    unsigned long long result = 0;
    unsigned long long power = 1;
    for (int i = value.length() - 1; i >= 0; --i) {
        int digit = (isdigit(value[i])) ? (value[i] - '0') : (toupper(value[i]) - 'A' + 10);
        result += digit * power;
        power *= base;
    }
    return result;
}


double lagrangeInterpolation(const vector<pair<int, unsigned long long>>& points) {
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


void processTestCase(const string& filename) {
    
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file " << filename << endl;
        return;
    }

    
    string jsonInput((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    
    Document document;
    document.Parse(jsonInput.c_str());

    if (!document.IsObject()) {
        cerr << "Invalid JSON format." << endl;
        return;
    }

    
    const Value& keys = document["keys"];
    int n = keys["n"].GetInt();
    int k = keys["k"].GetInt();

    vector<pair<int, unsigned long long>> points;

    
    for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
        string key = itr->name.GetString();
        if (key == "keys") continue;  

        int x = stoi(key);  
        int base = itr->value["base"].GetInt();
        string valueStr = itr->value["value"].GetString();
        unsigned long long y = convertToDecimal(valueStr, base); 

        points.emplace_back(x, y);
    }


    if (points.size() < k) {
        cerr << "Insufficient points provided for polynomial interpolation." << endl;
        return;
    }
    double constantTerm = lagrangeInterpolation(points);

    cout << "The constant term c for " << filename << " is: " << constantTerm << endl;
}

int main() {
    
    processTestCase("testcase1.json");

    return 0;
}
