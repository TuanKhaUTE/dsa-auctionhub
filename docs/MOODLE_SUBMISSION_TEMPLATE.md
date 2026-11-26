# AuctionHub — DSA Practical Submission

**Public GitHub Repository:** https://github.com/TuanKhaUTE/dsa-auctionhub

## 1. Project Overview

The AuctionHub project implements five sorting algorithms in C++17 using the `Bid` structure supplied in the assignment. The repository contains `auctionhub.cpp`, `REFLECTION.md`, `test_data/small.txt`, `test_data/large.txt` (100,000 generated records), and `screenshot_runtime.png`, plus supporting documentation and raw execution logs.

## 2. Implementation by Task

**Task A — Insertion Sort (Live Auction Monitor):** Sorts ascending by bid amount and then by timestamp when amounts tie. The code shifts larger records to the right, making it a practical fit for nearly sorted arrivals.

**Task B — Selection Sort (Top-K Bid Finder):** Sorts in place by amount descending and displays the first three records. For the provided small input, the Top 3 bidders are dave, bob, and carol.

**Task C — Interchange Sort (Price Anomaly Validator):** Sorts a small batch ascending by amount and prints the count of actual swaps. It exchanges elements immediately when an out-of-order pair is found.

**Task D — Bubble Sort with Early Stop (Stabilization Detector):** Uses a `swapped` flag and sorts ascending by amount. It returns `STABLE` only when no swaps occur during the FIRST full pass, even if a later pass allows the sorting process to finish early.

**Task E — Median-of-Three Quick Sort (Full Historical Sorter):** Chooses the median of the first, middle, and last records, moves it into the high position and uses Lomuto partitioning. The wrapper handles an empty vector, and sorting uses amount and timestamp tie-breaking.

## 3. Test Evidence

The automated harness passed **42/42 checks**. These cover empty, one-element, sorted, reversed, all-equal-key, duplicate-price, near-sorted, and negative/zero synthetic inputs across all five sorting functions. A separate stress test with 2,000 identical `(amount,timestamp)` keys verified Quick Sort termination; another regression test confirmed corrected file-loading behavior.

The demo on `test_data/small.txt` produced the expected ascending order for Task A and Top 3 order for Task B. Test details are available in `docs/TEST_RESULTS.md`, `evidence/self_test_output.txt`, and `evidence/demo_output.txt`.

## 4. Performance Benchmark

The 100,000-bid benchmark uses `std::mt19937` with a fixed seed and measures Quick Sort using `std::chrono::steady_clock`. Five fresh copies of the same unsorted input were sorted, with timing excluding file I/O and validation.

| Metric | Result |
|---|---:|
| Dataset size | 100,000 bids |
| Benchmark runs | 5 |
| Average runtime | **30.890 ms** |
| Fastest runtime | **27.458 ms** |
| Validation | **5/5 PASS** |

These measurements were obtained in the test environment using GCC C++17 `-O2`; timings will vary with the machine. The raw log is in `evidence/benchmark_output.txt`, and the console-style rendering of those actual results is in `screenshot_runtime.png`.

## 5. Complexity and Reflection

Insertion Sort has O(n) best-case and O(n²) average/worst-case time. Selection Sort and Interchange Sort have O(n²) time in all cases. Early-stop Bubble Sort improves its already-sorted best case to O(n), while Quick Sort has typical O(n log n) time but can still degrade to O(n²) with unbalanced Lomuto partitions, including identical keys.

The detailed justification for all five modules, an actual file-loader bug and its correction, the AI interaction record, and the Quick Sort worst-case explanation are documented in `REFLECTION.md` and `docs/AI_USAGE_LOG.md`.

## 6. Reproduction

Build with:

```bash
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror auctionhub.cpp -o auctionhub
```

Run `./auctionhub --self-test`, `./auctionhub --demo test_data/small.txt`, and `./auctionhub --benchmark test_data/large.txt` from the repository root.

**Submission note:** Review/adapt AI-assisted implementation to meet the instructor's written LLM-use requirements, or document an instructor-approved exception before declaring this work ready for submission.
