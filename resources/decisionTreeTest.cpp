#include "decisionTreeTest.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include "../decisionTree.h"

using namespace std;

int test_same_class() {
    int num_correct = 0;
    const int num_tests = 5;
    std::vector<int> wrong_answers;

    const vector<string> tests_in = {
        "tests/same_class/test1.in", "tests/same_class/test2.in",
        "tests/same_class/test3.in", "tests/same_class/test4.in",
        "tests/same_class/test5.in"};

    const vector<string> tests_ref = {
        "tests/same_class/test1.ref", "tests/same_class/test2.ref",
        "tests/same_class/test3.ref", "tests/same_class/test4.ref",
        "tests/same_class/test5.ref"};

    for (int i = 0; i < num_tests; i++) {
        ifstream in(tests_in[i]);
        ifstream ref(tests_ref[i]);

        // Format teste: prima linie n, m
        // Pe fiecare din urm n linii: cate m elemente

        int n, m, elem;
        in >> n >> m;
        std::vector<std::vector<int>> samples;
        std::vector<int> temp;

        for (int j = 0; j < n; j++) {
            temp.clear();
            for (int k = 0; k < m; k++) {
                in >> elem;
                temp.push_back(elem);
            }
            samples.push_back(temp);
        }

        // read the ref file
        bool ref_result;
        ref >> ref_result;

        if (same_class(samples) == ref_result) {
            num_correct++;
        } else {
            wrong_answers.push_back(i);
        }
    }

    if (num_correct != num_tests) {
        std::cerr << "Failed: ";
        for (size_t i = 0; i < wrong_answers.size(); i++)
            std::cerr << wrong_answers[i] + 1 << " ";
        std::cerr << std::endl;
    }
    return num_correct;
}

int test_random_dimensions() {
    int num_correct = 0;
    const int num_tests = 5;
    std::vector<int> wrong_answers;

    const std::vector<string> tests_in = {
        "tests/random_dimensions/test1.in", "tests/random_dimensions/test2.in",
        "tests/random_dimensions/test3.in", "tests/random_dimensions/test4.in",
        "tests/random_dimensions/test5.in",
    };

    for (int i = 0; i < num_tests; i++) {
        ifstream in(tests_in[i]);
        int size;
        bool ok = 1;
        std::vector<int> result;

        // format: un singur intreg, size
        in >> size;
        result = random_dimensions(size);
        if (result.size() != (unsigned int)sqrt(size)) ok = 0;
        std::unordered_map<int, int> map;
        for (size_t i = 0; i < result.size(); i++) {
            if (result[i] <= 0 || result[i] >= size) ok = 0;
            if (map[result[i]] != 0)
                ok = 0;
            else
                map[result[i]] = 1;
        }
        if (ok)
            num_correct++;
        else
            wrong_answers.push_back(i);
    }

    if (num_correct != num_tests) {
        std::cerr << "Failed: ";
        for (size_t i = 0; i < wrong_answers.size(); i++)
            std::cerr << wrong_answers[i] + 1 << " ";
        std::cerr << std::endl;
    }
    return num_correct;
}

int test_compute_unique() {
    int num_correct = 0;
    const int num_tests = 5;
    std::vector<int> wrong_answers;

    const vector<string> tests_in = {
        "tests/compute_unique/test1.in", "tests/compute_unique/test2.in",
        "tests/compute_unique/test3.in", "tests/compute_unique/test4.in",
        "tests/compute_unique/test5.in"};

    const vector<string> tests_ref = {
        "tests/compute_unique/test1.ref", "tests/compute_unique/test2.ref",
        "tests/compute_unique/test3.ref", "tests/compute_unique/test4.ref",
        "tests/compute_unique/test5.ref"};

    for (int i = 0; i < num_tests; i++) {
        ifstream in(tests_in[i]);
        ifstream ref(tests_ref[i]);

        // Format teste: prima linie n, m, col
        // Pe fiecare din urm n linii: cate m elemente

        int n, m, elem, col, ref_n;
        in >> n >> m >> col;
        std::vector<std::vector<int>> samples;
        std::vector<int> temp;

        for (int j = 0; j < n; j++) {
            temp.clear();
            for (int k = 0; k < m; k++) {
                in >> elem;
                temp.push_back(elem);
            }
            samples.push_back(temp);
        }

        // read the ref file
        vector<int> ref_result;
        ref >> ref_n;
        for (int j = 0; j < ref_n; j++) {
            ref >> elem;
            ref_result.push_back(elem);
        }

        vector<int> result = compute_unique(samples, col);

        if (result.size() != ref_result.size()) {
            wrong_answers.push_back(i);
            continue;
        }

        bool ok = 1;
        sort(result.begin(), result.end());
        for (size_t j = 0; j < ref_result.size(); j++) {
            if (result[j] != ref_result[j]) {
                ok = 0;
                break;
            }
        }

        if (ok)
            num_correct++;
        else
            wrong_answers.push_back(i);
    }

    if (num_correct != num_tests) {
        std::cerr << "Failed: ";
        for (size_t i = 0; i < wrong_answers.size(); i++)
            std::cerr << wrong_answers[i] + 1 << " ";
        std::cerr << std::endl;
    }
    return num_correct;
}

int test_get_split_as_indexes() {
    int num_correct = 0;
    const int num_tests = 5;
    std::vector<int> wrong_answers;

    const std::vector<string> tests_in = {
        "tests/get_split_as_indexes/test1.in",
        "tests/get_split_as_indexes/test2.in",
        "tests/get_split_as_indexes/test3.in",
        "tests/get_split_as_indexes/test4.in",
        "tests/get_split_as_indexes/test5.in",
    };

    const std::vector<string> tests_ref = {
        "tests/get_split_as_indexes/test1.ref",
        "tests/get_split_as_indexes/test2.ref",
        "tests/get_split_as_indexes/test3.ref",
        "tests/get_split_as_indexes/test4.ref",
        "tests/get_split_as_indexes/test5.ref",
    };

    for (int i = 0; i < num_tests; i++) {
        ifstream in(tests_in[i]);
        ifstream ref(tests_ref[i]);

        // format input m n split_index, split_value
        // + n linii a cate m elemente
        int n, m, split_index, split_value, elem;
        in >> n >> m >> split_index >> split_value;
        std::vector<std::vector<int>> samples;
        pair<vector<int>, vector<int>> result;
        std::vector<int> temp, left, right;
        for (int j = 0; j < n; j++) {
            temp.clear();
            for (int k = 0; k < m; k++) {
                in >> elem;
                temp.push_back(elem);
            }
            samples.push_back(temp);
        }

        result = get_split_as_indexes(samples, split_index, split_value);

        // format output : n + n elemente, m + m elemente (ambele sortate)
        // reprezinta copiii formati

        int x, y;
        ref >> x;
        for (int i = 0; i < x; i++) {
            ref >> elem;
            left.push_back(elem);
        }
        ref >> y;
        for (int i = 0; i < y; i++) {
            ref >> elem;
            right.push_back(elem);
        }
        std::sort(result.first.begin(), result.first.end());
        std::sort(result.second.begin(), result.second.end());

        if (left != result.first || right != result.second)
            wrong_answers.push_back(i);
        else
            num_correct++;
    }

    if (num_correct != num_tests) {
        std::cerr << "Failed: ";
        for (size_t i = 0; i < wrong_answers.size(); i++)
            std::cerr << wrong_answers[i] + 1 << " ";
        std::cerr << std::endl;
    }
    return num_correct;
}
