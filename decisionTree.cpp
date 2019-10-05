// copyright Luca Istrate, Andrei Medar
// copyright 2018 Alina Galiceanu, Alexandra Maracine

#include "./decisionTree.h"  // NOLINT(build/include)
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <random>

using std::string;
using std::pair;
using std::vector;
using std::unordered_map;
using std::make_shared;
using std::mt19937;

// structura unui nod din decision tree
// splitIndex = dimensiunea in functie de care se imparte
// split_value = valoarea in functie de care se imparte
// is_leaf si result sunt pentru cazul in care avem un nod frunza
Node::Node() {
    is_leaf = false;
    left = nullptr;
    right = nullptr;
}

void Node::make_decision_node(const int index, const int val) {
    split_index = index;
    split_value = val;
}

// Pentru a implementa functia make_leaf am verificat daca samples au
// aceeasi clasa, in cazul in care au existat clase diferite, am folosit
// un unordered_map pentru a monitoriza frecventa fiecarei clase, apoi
// am atribuit variabilei "result" clasa cu frecventa maxima;

void Node::make_leaf(const vector<vector<int>> &samples,
                     const bool is_single_class) {
    // TODO(you)
    // Seteaza nodul ca fiind de tip frunza (modificati is_leaf si result)
    // is_single_class = true -> toate testele au aceeasi clasa (acela e result)
    // is_single_class = false -> se alege clasa care apare cel mai des
    std::unordered_map<int, int> fr;
    is_leaf = true;
    int size = samples.size();
    if (is_single_class == true) {
        result = samples[0][0];
    } else {
        for (int i = 0; i < size; ++i) {
                ++fr[samples[i][0]];
        }
        int max = 0;
        int max_class;
        for (auto x : fr) {
            if (x.second > max) {
                max = x.second;
                max_class = x.first;
            }
        }
        result = max_class;
    }
}

// Pentru a implementa functia find_best_split am folosit functii
// implementate anterior: get_entropy, get_entropy_as_indexes,
// compute_unique, get_split_as_indexes;
// Pentru a afla care este cel mai bun split am folosit procedeul
// InformationGain, valoarea maxima a IG-ului dand cel mai optim split
// value

pair<int, int> find_best_split(const vector<vector<int>> &samples,
                               const vector<int> &dimensions) {
    // TODO(you)
    // Intoarce cea mai buna dimensiune si valoare de split dintre testele
    // primite. Prin cel mai bun split (dimensiune si valoare)
    // ne referim la split-ul care maximizeaza IG
    // pair-ul intors este format din (split_index, split_value)

    int splitIndex = -1, splitValue = -1;
    float maxIG = 0;
    float parent_entropy = get_entropy(samples);
    int dimensions_size = dimensions.size();
    for (int i = 0; i < dimensions_size; ++i) {
        vector<int> new_samples = compute_unique(samples, dimensions[i]);
        int new_samples_size = new_samples.size();
        for (int j = 0; j < new_samples_size; ++j) {
            pair<vector<int>, vector<int>> child =
                get_split_as_indexes(samples, dimensions[i], new_samples[j]);
            float left_size = child.first.size();
            float right_size = child.second.size();
            if (left_size != 0 && right_size != 0) {
                float IG = parent_entropy - (left_size *
                    get_entropy_by_indexes(samples, child.first) +
                    right_size * get_entropy_by_indexes(samples, child.second))
                    / (left_size + right_size);
                if (maxIG < IG) {
                    maxIG = IG;
                    splitIndex = dimensions[i];
                    splitValue = new_samples[j];
                }
            }
        }
    }
    return pair<int, int>(splitIndex, splitValue);
}

// Folosind algoritmul ce urmeaza am implementat o metoda ce
// construieste un arbore de decizie in functie de valorile returnate
// de find_best_split si same_class urmand instructiunile date

void Node::train(const vector<vector<int>> &samples) {
    // TODO(you)
    // Antreneaza nodul curent si copii sai, daca e nevoie
    // 1) verifica daca toate testele primite au aceeasi clasa (raspuns)
    // Daca da, acest nod devine frunza, altfel continua algoritmul.
    // 2) Daca nu exista niciun split valid, acest nod devine frunza. Altfel,
    // ia cel mai bun split si continua recursiv

    vector<int> dimension = random_dimensions(samples[0].size());
    if (same_class(samples) == true) {
        make_leaf(samples, true);
    } else {
        pair<int, int> aux = find_best_split(samples, dimension);
        if (aux.first == -1 && aux.second == -1) {
            make_leaf(samples, false);
        } else {
            pair<vector<vector<int>>, vector< vector<int>>> child =
                split(samples, aux.first, aux.second);
            make_decision_node(aux.first, aux.second);
            left = make_shared<Node>(Node());
            right = make_shared<Node>(Node());
            left->train(child.first);
            right->train(child.second);
        }
    }
}

// Functia urmatoare intoarce variabila result atunci cand s-a
// ajuns la o frunza, pana se ajunge la frunza se apeleaza functia
// pe subarborele drept sau stang in functie de split_index si
// split_value.

int Node::predict(const vector<int> &image) const {
    // TODO(you)
    // Intoarce rezultatul prezis de catre decision tree
    if (is_leaf == false) {
        if (image[split_index - 1] <= split_value) {
            left -> predict(image);
        } else {
            right -> predict(image);
        }
    } else {
        return result;
    }
}

bool same_class(const vector<vector<int>> &samples) {
    // TODO(you)
    // Verifica daca testele primite ca argument au toate aceeasi
    // clasa(rezultat). Este folosit in train pentru a determina daca
    // mai are rost sa caute split-uri
    int size = samples.size();
    for (int i = 0; i < size - 1; ++i) {
        if (samples[i][0] != samples[i + 1][0]) {
            return false;
        }
    }
    return true;
}

float get_entropy(const vector<vector<int>> &samples) {
    // Intoarce entropia testelor primite
    assert(!samples.empty());
    vector<int> indexes;

    int size = samples.size();
    for (int i = 0; i < size; i++) indexes.push_back(i);

    return get_entropy_by_indexes(samples, indexes);
}

// Functia calculeaza in functie de valorile din vectorul index
// entropia subsetului dupa formula suma(pi *log2(pi), unde pi
// reprezinta probabilitatea ca o anumita clasa din setul total
// sa fie intalnita

float get_entropy_by_indexes(const vector<vector<int>> &samples,
                             const vector<int> &index) {
    // TODO(you)
    // Intoarce entropia subsetului din setul de teste total(samples)
    // Cu conditia ca subsetul sa contina testele ale caror indecsi se gasesc in
    // vectorul index (Se considera doar liniile din vectorul index)
    std::unordered_map<int, float> fr;
    float size = index.size();
    for (int i = 0; i < size; ++i) {
        ++fr[samples[index[i]][0]];
    }
    float sum = 0;
    for (auto i : fr) {
        float x = (i.second / size);
        if (x != 0) {
            sum = sum +  x * log2(x);
        }
    }
    sum = -sum;
    return sum;
}

// Am folosit un unordered_map pentru a stoca o singura data fiecare
// element monitorizandu-i frecventa apoi am adaugat valorile in vectorul
// uniqueValues ce returneaza datele cerute

vector<int> compute_unique(const vector<vector<int>> &samples, const int col) {
    // TODO(you)
    // Intoarce toate valorile (se elimina duplicatele)
    // care apar in setul de teste, pe coloana col
    vector<int> uniqueValues;
    std::unordered_map<int, int> fr;
    int size = samples.size();
    for (int i = 0; i < size; ++i) {
        ++fr[samples[i][col]];
    }
    for (auto i : fr) {
        uniqueValues.push_back(i.first);
    }

    return uniqueValues;
}

pair<vector<vector<int>>, vector<vector<int>>> split(
    const vector<vector<int>> &samples, const int split_index,
    const int split_value) {
    // Intoarce cele 2 subseturi de teste obtinute in urma separarii
    // In functie de split_index si split_value
    vector<vector<int>> left, right;

    auto p = get_split_as_indexes(samples, split_index, split_value);
    for (const auto &i : p.first) left.push_back(samples[i]);
    for (const auto &i : p.second) right.push_back(samples[i]);

    return pair<vector<vector<int>>, vector<vector<int>>>(left, right);
}

// In functie de valoarea lui samples[i][split_index] si split_value
// setul de samples dat este divizat in doua subseturi

pair<vector<int>, vector<int>> get_split_as_indexes(
    const vector<vector<int>> &samples, const int split_index,
    const int split_value) {
    // TODO(you)
    // Intoarce indecsii sample-urilor din cele 2 subseturi obtinute in urma
    // separarii in functie de split_index si split_value
    vector<int> left, right;
    int size = samples.size();
    for (int i = 0; i < size ; ++i) {
        if (samples[i][split_index] <= split_value) {
            left.push_back(i);
        } else {
            right.push_back(i);
        }
    }
    return make_pair(left, right);
}

// Pentru generarea de numere random am folosit utilitara mt19937
// impreuna cu random_device si uniform_int_distribution.
// Pentru restul implementarii am folosit un vector de frecventa
// pentru a ne asigura ca valorile returnate sunt unice.

vector<int> random_dimensions(const int size) {
    // TODO(you)
    // Intoarce sqrt(size) dimensiuni diferite pe care sa caute splitul maxim
    // Precizare: Dimensiunile gasite sunt > 0 si < size
    vector<int> rez;
    int dim = floor(sqrt(size));
    vector<int> fr(size, 0);
    int minSize = 1;
    int maxSize = size - 1;
    static std::random_device numbers;
    static mt19937 randGen(numbers());
    std::uniform_int_distribution<int> num(minSize, maxSize);
    for (int i = 0; i < dim; ++i) {
        int x = num(numbers);
        if (fr[x] == 0) {
            rez.push_back(x);
            ++fr[x];
        } else {
            --i;
        }
    }
    return rez;
}
