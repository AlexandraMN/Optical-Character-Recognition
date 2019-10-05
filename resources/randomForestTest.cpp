#include "randomForestTest.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int test_get_random_samples() {
    int num_correct = 0;
    const int num_tests = 5;

    std::vector<int> wrong_answers;
    const std::vector<string> tests_in = {
        "tests/get_random_samples/test1.in",
        "tests/get_random_samples/test2.in",
        "tests/get_random_samples/test3.in",
        "tests/get_random_samples/test4.in",
        "tests/get_random_samples/test5.in",
    };

    for (int i = 0; i < num_tests; i++) {
        ifstream in(tests_in[i]);

        // formatul testelor: n m num_to_return
        // n linii a cate m elemente

        int n, m, num_to_return, elem;
        in >> n >> m >> num_to_return;
        std::vector<std::vector<int>> samples, result;
        std::vector<int> temp;
        for (int j = 0; j < n; j++) {
            temp.clear();
            for (int k = 0; k < m; k++) {
                in >> elem;
                temp.push_back(elem);
            }
            samples.push_back(temp);
        }

        result = get_random_samples(samples, num_to_return);
        bool ok = 1;
        if (result.size() != (unsigned int)num_to_return) {
            wrong_answers.push_back(i);
            continue;
        }
        for (size_t s = 0; s < result.size() - 1; s++)
            for (size_t r = s + 1; r < result.size(); r++)
                if (result[s] == result[r]) ok = 0;
        if (ok)
            num_correct++;
        else
            wrong_answers.push_back(i);
    }

    if (num_correct != num_tests) {
        std::cerr << "Tests failed: ";
        for (size_t i = 0; i < wrong_answers.size(); i++)
            std::cerr << wrong_answers[i] + 1 << " ";
        std::cerr << std::endl;
    }
    return num_correct;
}
