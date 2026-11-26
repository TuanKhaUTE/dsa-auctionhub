#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;
using namespace std::chrono;

struct Bid {
    string bidderId;
    double amount;
    long long timestamp;
    int auctionId;
};

// Sorting key for Tasks A and E: amount ascending; timestamp ascending on ties.
bool lessByAmountTime(const Bid& left, const Bid& right) {
    if (left.amount != right.amount) return left.amount < right.amount;
    return left.timestamp < right.timestamp;
}

void printBids(const vector<Bid>& v, size_t limit = 10) {
    for (size_t i = 0; i < min(v.size(), limit); ++i) {
        cout << "[" << v[i].bidderId << "] $" << fixed << setprecision(2)
             << v[i].amount << " @ " << v[i].timestamp << "\n";
    }
    if (v.size() > limit) cout << "... (" << v.size() << " total)\n";
}

bool loadBids(const string& path, vector<Bid>& out) {
    ifstream f(path);
    if (!f) return false;
    vector<Bid> loaded;
    Bid b;
    while (f >> b.bidderId >> b.amount >> b.timestamp >> b.auctionId) {
        loaded.push_back(b);
    }
    if (!f.eof()) return false;  // Do not replace valid data on a malformed file.
    out = move(loaded);           // Replace the previous contents, do not append.
    return true;
}

// Task A: stable insertion sort; shift records with strictly greater keys.
void insertionSort(vector<Bid>& v) {
    for (size_t i = 1; i < v.size(); ++i) {
        Bid key = v[i];
        size_t j = i;
        while (j > 0 && lessByAmountTime(key, v[j - 1])) {
            v[j] = v[j - 1];
            --j;
        }
        v[j] = key;
    }
}

// Task B: in-place selection sort by price descending (at most n-1 swaps).
void selectionSort(vector<Bid>& v) {
    for (size_t i = 0; i < v.size(); ++i) {
        size_t maxIndex = i;
        for (size_t j = i + 1; j < v.size(); ++j) {
            if (v[j].amount > v[maxIndex].amount) maxIndex = j;
        }
        if (maxIndex != i) swap(v[i], v[maxIndex]);
    }
}

// Task C: interchange sort (immediate swaps, unlike selection sort).
// The function reports the number of swaps itself, as requested.
void interchangeSort(vector<Bid>& v) {
    unsigned long long swaps = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        for (size_t j = i + 1; j < v.size(); ++j) {
            if (v[j].amount < v[i].amount) {
                swap(v[i], v[j]);
                ++swaps;
            }
        }
    }
    cout << "Interchange Sort: total swaps = " << swaps << '\n';
}

// Task D: return TRUE iff the FIRST full pass made no swaps.
// Do not confuse this with stopping later after an initially unsorted input.
bool bubbleSortEarlyStop(vector<Bid>& v) {
    if (v.size() < 2) return true;
    bool initiallySorted = false;
    for (size_t i = 0; i + 1 < v.size(); ++i) {
        bool swapped = false;
        for (size_t j = 0; j + 1 < v.size() - i; ++j) {
            if (v[j].amount > v[j + 1].amount) {
                swap(v[j], v[j + 1]);
                swapped = true;
            }
        }
        if (i == 0) initiallySorted = !swapped;
        if (!swapped) break;
    }
    return initiallySorted;
}

// Task E: median-of-three selection among low/middle/high; standard Lomuto.
int medianOfThreeIndex(const vector<Bid>& v, int low, int mid, int high) {
    const Bid& a = v[static_cast<size_t>(low)];
    const Bid& b = v[static_cast<size_t>(mid)];
    const Bid& c = v[static_cast<size_t>(high)];
    if (lessByAmountTime(a, b)) {
        if (lessByAmountTime(b, c)) return mid;
        return lessByAmountTime(a, c) ? high : low;
    }
    if (lessByAmountTime(a, c)) return low;
    return lessByAmountTime(b, c) ? high : mid;
}

int partitionLomutoMedian(vector<Bid>& v, int low, int high) {
    int mid = low + (high - low) / 2;
    int median = medianOfThreeIndex(v, low, mid, high);
    swap(v[static_cast<size_t>(median)], v[static_cast<size_t>(high)]);
    const Bid pivot = v[static_cast<size_t>(high)];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        // Lomuto uses <= pivot: move elements <= pivot to the left.
        if (!lessByAmountTime(pivot, v[static_cast<size_t>(j)])) {
            ++i;
            swap(v[static_cast<size_t>(i)], v[static_cast<size_t>(j)]);
        }
    }
    swap(v[static_cast<size_t>(i + 1)], v[static_cast<size_t>(high)]);
    return i + 1;
}

void quickSort(vector<Bid>& v, int lo, int hi) {
    // Recurse on the smaller side, iterate over the larger side.
    // This bounds stack depth even for many duplicate pivot values.
    while (lo < hi) {
        int pivotIndex = partitionLomutoMedian(v, lo, hi);
        int leftSize = pivotIndex - lo;
        int rightSize = hi - pivotIndex;
        if (leftSize < rightSize) {
            if (leftSize > 1) quickSort(v, lo, pivotIndex - 1);
            lo = pivotIndex + 1;
        } else {
            if (rightSize > 1) quickSort(v, pivotIndex + 1, hi);
            hi = pivotIndex - 1;
        }
    }
}

void quickSortWrapper(vector<Bid>& v) {
    if (v.size() > 1) {
        if (v.size() > static_cast<size_t>(numeric_limits<int>::max())) {
            throw invalid_argument("Too many records for int indices");
        }
        quickSort(v, 0, static_cast<int>(v.size()) - 1);
    }
}

bool isSortedAsc(const vector<Bid>& v, bool useTimestamp) {
    for (size_t i = 1; i < v.size(); ++i) {
        bool wrong = useTimestamp ? lessByAmountTime(v[i], v[i - 1])
                                  : v[i].amount < v[i - 1].amount;
        if (wrong) return false;
    }
    return true;
}

bool isSortedDesc(const vector<Bid>& v) {
    for (size_t i = 1; i < v.size(); ++i) {
        if (v[i].amount > v[i - 1].amount) return false;
    }
    return true;
}

bool sameRecords(const vector<Bid>& first, const vector<Bid>& second) {
    if (first.size() != second.size()) return false;
    vector<string> a, b;
    a.reserve(first.size());
    b.reserve(second.size());
    for (const Bid& item : first) a.push_back(item.bidderId + "#" + to_string(item.auctionId)
                                                  + "#" + to_string(item.timestamp)
                                                  + "#" + to_string(item.amount));
    for (const Bid& item : second) b.push_back(item.bidderId + "#" + to_string(item.auctionId)
                                                   + "#" + to_string(item.timestamp)
                                                   + "#" + to_string(item.amount));
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    return a == b;
}

vector<Bid> makeBids(const vector<double>& prices, const vector<long long>& times = {}) {
    vector<Bid> result;
    for (size_t i = 0; i < prices.size(); ++i) {
        result.push_back({"test_" + to_string(i), prices[i],
                          times.empty() ? static_cast<long long>(1000 + i) : times[i], 1});
    }
    return result;
}

bool runEdgeTests() {
    cout << "=== SELF-TEST: ALL FIVE ALGORITHMS ===\n";
    struct Test {
        string name;
        vector<Bid> bids;
    };
    vector<Test> tests = {
        {"empty", makeBids({})},
        {"single", makeBids({42})},
        {"already sorted", makeBids({1, 2, 3, 4, 5, 6})},
        {"reverse sorted", makeBids({6, 5, 4, 3, 2, 1})},
        {"all equal price and timestamp", makeBids({7, 7, 7, 7, 7, 7}, {1000, 1000, 1000, 1000, 1000, 1000})},
        {"duplicate prices/timestamp ties", makeBids({5, 2, 5, 2, 5, 2}, {105, 102, 101, 106, 100, 103})},
        {"near sorted", makeBids({100, 102.5, 101, 103})},
        {"negative and zero", makeBids({0, -2, 3, -2, 0, 1})}
    };
    int passed = 0;
    int total = 0;
    for (const Test& test : tests) {
        vector<Bid> a = test.bids;
        insertionSort(a);
        bool okA = isSortedAsc(a, true) && sameRecords(test.bids, a);
        vector<Bid> b = test.bids;
        selectionSort(b);
        bool okB = isSortedDesc(b) && sameRecords(test.bids, b);
        vector<Bid> c = test.bids;
        interchangeSort(c);
        bool okC = isSortedAsc(c, false) && sameRecords(test.bids, c);
        vector<Bid> d = test.bids;
        bool observedStable = bubbleSortEarlyStop(d);
        bool originallySorted = isSortedAsc(test.bids, false);
        bool okD = isSortedAsc(d, false) && observedStable == originallySorted
                   && sameRecords(test.bids, d);
        vector<Bid> e = test.bids;
        quickSortWrapper(e);
        bool okE = isSortedAsc(e, true) && sameRecords(test.bids, e);
        bool results[] = {okA, okB, okC, okD, okE};
        const char* labels[] = {"Insertion", "Selection", "Interchange", "Bubble", "Quick"};
        for (int index = 0; index < 5; ++index) {
            ++total;
            if (results[index]) ++passed;
            cout << "[" << (results[index] ? "PASS" : "FAIL") << "] "
                 << setw(11) << left << labels[index] << " | " << test.name << '\n';
        }
    }
    // Stress test specifically catches non-shrinking recursion and equal pivots.
    vector<Bid> equalLarge = makeBids(vector<double>(2000, 7.0), vector<long long>(2000, 1000));
    vector<Bid> stress = equalLarge;
    quickSortWrapper(stress);
    bool stressOk = isSortedAsc(stress, true) && sameRecords(equalLarge, stress);
    ++total;
    if (stressOk) ++passed;
    cout << "[" << (stressOk ? "PASS" : "FAIL")
         << "] Quick Sort  | 2,000 equal bids (termination stress test)\n";
    // Regression test for a real integration bug: loading into a nonempty
    // vector used to append records instead of replacing the old contents.
    vector<Bid> existing = makeBids({999});
    bool loaded = loadBids("test_data/small.txt", existing);
    bool loaderOk = loaded && existing.size() == 4 && existing[0].bidderId == "alice";
    ++total;
    if (loaderOk) ++passed;
    cout << "[" << (loaderOk ? "PASS" : "FAIL")
         << "] File Loader | replace existing records (regression test)\n";
    cout << "TOTAL: " << passed << "/" << total << " tests passed\n";
    return passed == total;
}

vector<Bid> generateBids(size_t count, unsigned int seed = 20261126) {
    mt19937 rng(seed);
    uniform_int_distribution<int> amountCents(10000, 1000000);
    uniform_int_distribution<int> auctionId(1, 1000);
    vector<Bid> bids;
    bids.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        bids.push_back({"bidder_" + to_string(i),
                        amountCents(rng) / 100.0,
                        1700000000000LL + static_cast<long long>(i), auctionId(rng)});
    }
    return bids;
}

bool saveBids(const string& path, const vector<Bid>& bids) {
    ofstream out(path);
    if (!out) return false;
    out << fixed << setprecision(2);
    for (const Bid& bid : bids) {
        out << bid.bidderId << ' ' << bid.amount << ' ' << bid.timestamp
            << ' ' << bid.auctionId << '\n';
    }
    return static_cast<bool>(out);
}

bool benchmarkQuickSort(const vector<Bid>& original, int trials) {
    cout << "=== QUICK SORT BENCHMARK ===\n";
    cout << "Records: " << original.size() << " | pivot: median-of-three | trials: "
         << trials << '\n';
    if (!original.empty() && original.size() > static_cast<size_t>(numeric_limits<int>::max())) {
        cerr << "Array too large for int indices\n";
        return false;
    }
    double total = 0;
    double fastest = numeric_limits<double>::max();
    vector<Bid> reference = original;
    sort(reference.begin(), reference.end(), lessByAmountTime);
    for (int run = 1; run <= trials; ++run) {
        vector<Bid> copy = original;
        const auto start = steady_clock::now();
        quickSortWrapper(copy);
        const auto finish = steady_clock::now();
        const double milliseconds = duration<double, milli>(finish - start).count();
        bool okay = isSortedAsc(copy, true) && sameRecords(original, copy);
        // For matching (amount,timestamp) keys, exact ID order is unspecified.
        for (size_t i = 0; okay && i < copy.size(); ++i) {
            okay = copy[i].amount == reference[i].amount
                && copy[i].timestamp == reference[i].timestamp;
        }
        cout << fixed << setprecision(3) << "Run " << run << ": "
             << milliseconds << " ms | " << (okay ? "PASS" : "FAIL") << '\n';
        if (!okay) return false;
        fastest = min(fastest, milliseconds);
        total += milliseconds;
    }
    cout << fixed << setprecision(3) << "Average: " << total / trials
         << " ms | Fastest: " << fastest << " ms\n";
    cout << "Validation: sorted ascending by amount, timestamp ties; records preserved\n";
    return true;
}

bool runDemo(const string& path) {
    vector<Bid> bids;
    if (!loadBids(path, bids)) {
        cerr << "Cannot load " << path << '\n';
        return false;
    }
    cout << "=== AUCTIONHUB SMALL DEMO: " << path << " ===\n";
    cout << "Loaded " << bids.size() << " bids\n";
    auto a = bids; insertionSort(a);
    cout << "Task A - Live Auction Monitor (ascending price, timestamp tie):\n";
    printBids(a);
    auto b = bids; selectionSort(b);
    cout << "Task B - Top 3 bids (descending price):\n";
    printBids(b, 3);
    auto c = bids; interchangeSort(c);
    cout << "Task C - Price Anomaly Validator (ascending price):\n";
    printBids(c);
    auto d = bids; const bool stable = bubbleSortEarlyStop(d);
    cout << "Task D - Price window: " << (stable ? "STABLE" : "UNSTABLE") << '\n';
    auto e = bids; quickSortWrapper(e);
    cout << "Task E - Historical sorter (ascending price, timestamp tie):\n";
    printBids(e);
    bool ok = isSortedAsc(a, true) && isSortedDesc(b) && isSortedAsc(c, false)
        && isSortedAsc(d, false) && isSortedAsc(e, true);
    cout << "DEMO VALIDATION: " << (ok ? "PASS" : "FAIL") << '\n';
    return ok;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage:\n"
             << "  ./auctionhub --self-test\n"
             << "  ./auctionhub --demo test_data/small.txt\n"
             << "  ./auctionhub --generate 100000 test_data/large.txt\n"
             << "  ./auctionhub --benchmark test_data/large.txt\n";
        return 1;
    }
    const string command = argv[1];
    if (command == "--self-test" && argc == 2) return runEdgeTests() ? 0 : 2;
    if (command == "--demo" && argc == 3) return runDemo(argv[2]) ? 0 : 2;
    if (command == "--generate" && argc == 4) {
        try {
            size_t count = stoull(argv[2]);
            if (count > 1000000) throw invalid_argument("Refusing > 1M generated records");
            bool okay = saveBids(argv[3], generateBids(count));
            cout << (okay ? "GENERATED " : "GENERATION FAILED ") << count
                 << " bids -> " << argv[3] << '\n';
            return okay ? 0 : 2;
        } catch (const exception& ex) {
            cerr << "Error: " << ex.what() << '\n';
            return 2;
        }
    }
    if (command == "--benchmark" && argc == 3) {
        vector<Bid> bids;
        if (!loadBids(argv[2], bids)) {
            cerr << "Cannot load " << argv[2] << '\n';
            return 2;
        }
        return benchmarkQuickSort(bids, 5) ? 0 : 2;
    }
    cerr << "Unknown command or arguments\n";
    return 1;
}
