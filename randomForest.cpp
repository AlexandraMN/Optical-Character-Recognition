// copyright Luca Istrate, Andrei Medar
// copyright 2018 Alina Galiceanu, Alexandra Maracine

#include "randomForest.h"
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include "decisionTree.h"
#include <unordered_map>

using std::vector;
using std::pair;
using std::string;
using std::mt19937;

// Pentru a genera valori random am folosit functia rand_r, iar pentru
// a garanta valorile unice din random samples am folosit unordered_map

vector<vector<int>> get_random_samples(const vector<vector<int>> &samples,
                                       int num_to_return) {
    // TODO(you)
    // Intoarce un vector de marime num_to_return cu elemente random,
    // diferite din samples
    vector<vector<int>> ret(num_to_return);
    std::unordered_map<int, int> visited;
    unsigned int aux = 1;
    int i = 0;
    int size = samples.size();
    while (visited.size() != num_to_return) {
        int x = rand_r(&aux) % size;
        ++aux;
        ++visited[x];
    }
    for (auto a : visited) {
        ret[i] = samples[a.first];
        ++i;
    }
    return ret;
}

RandomForest::RandomForest(int num_trees, const vector<vector<int>> &samples)
    : num_trees(num_trees), images(samples) {}

void RandomForest::build() {
    // Aloca pentru fiecare Tree cate n / num_trees
    // Unde n e numarul total de teste de training
    // Apoi antreneaza fiecare tree cu testele alese
    assert(!images.empty());
    vector<vector<int>> random_samples;

    int data_size = images.size() / num_trees;

    for (int i = 0; i < num_trees; i++) {
        // cout << "Creating Tree nr: " << i << endl;
        random_samples = get_random_samples(images, data_size);

        // Construieste un Tree nou si il antreneaza
        trees.push_back(Node());
        trees[trees.size() - 1].train(random_samples);
    }
}

// Pentru a genera un raspuns cat mai precis am apelat "predict"
// pentru fiecare tree din trees si am returnat valoarea cu frecventa
// maxima

int RandomForest::predict(const vector<int> &image) {
    // TODO(you)
    // Va intoarce cea mai probabila prezicere pentru testul din argument
    // se va interoga fiecare Tree si se va considera raspunsul final ca
    // fiind cel majoritar

    std::unordered_map<int, int> fr;
    for (int i = 0; i < num_trees; ++i) {
        ++fr[trees[i].predict(image)];
    }
    int max = 0;
    int maxpos = 0;
    for (auto i : fr) {
        if (i.second > max) {
            max = i.second;
            maxpos = i.first;
        }
    }
    return maxpos;
}
