#include "randomForestTest.h"
#include "decisionTreeTest.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv) {
    if (argc != 2) {
        int sum = 0;
        sum += test_same_class();
        sum += test_get_random_samples();
        sum += test_random_dimensions();
        sum += test_compute_unique();
        sum += test_get_split_as_indexes();

        sum *= 2;

        cerr << "total: " << sum << "/50" << endl;
        cout << sum;
        return 0;
    }

    int result;
    string arg = string(argv[1]);
    if (arg == "same_class") {
        result = test_same_class() * 2;
        cerr << "same_class: " << result << "/10" << endl;
        cout << result << endl;;
    } else if (arg == "get_random_samples") {
        result = test_get_random_samples() * 2;
        cerr << "get_random_samples: " << result << "/10" << endl;
        cout << result << endl;;
    } else if (arg == "random_dimensions") {
        result = test_random_dimensions() * 2;
        cerr << "random_dimensions: " << result << "/10" << endl;
        cout << result << endl;;
    } else if (arg == "compute_unique") {
        result = test_compute_unique() * 2;
        cerr << "compute_unique: " << result << "/10" << endl;
        cout << result << endl;;
    } else if (arg == "get_split_as_indexes") {
        result = test_get_split_as_indexes() * 2;
        cerr << "test_get_split_as_indexes: " << result << "/10" << endl;
        cout << result << endl;;

    } else {
        cerr << "unknown command line argument" << endl;
    }

    return 0;
}
