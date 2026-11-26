# AuctionHub — Five Sorting Algorithms in C++

A reproducible DSA project demonstrating five real-world bid-sorting modules, correctness checks, and a **100,000-record Quick Sort benchmark**.

## Project contents

```text
auctionhub.cpp
REFLECTION.md
test_data/small.txt
test_data/large.txt
screenshot_runtime.png
README.md
docs/TEST_RESULTS.md
docs/AI_USAGE_LOG.md
docs/DEFENSE_QA.md
evidence/self_test_output.txt
evidence/demo_output.txt
evidence/benchmark_output.txt
evidence/loader_bug_before_after.txt
```

## Build & run

Requirements: C++17 compiler (verified with GCC/g++ 14.2.0 on Linux).

```bash
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror auctionhub.cpp -o auctionhub
./auctionhub --self-test
./auctionhub --demo test_data/small.txt
./auctionhub --generate 100000 test_data/large.txt
./auctionhub --benchmark test_data/large.txt
```

The `--generate` command **overwrites** the named data file with 100,000 deterministic bids using `std::mt19937` (seed `20261126`). Each bid contains `bidderId amount timestamp auctionId`, one record per line. The benchmark measures only sorting, not file-loading or validation, with `std::chrono::steady_clock` over five independent copies.

## Task-to-module mapping

| Task | AuctionHub module | Algorithm | Sort order |
|---|---|---|---|
| A | Live Auction Monitor | Insertion Sort | amount ASC, timestamp ASC on ties |
| B | Top-3 Bid Finder | Selection Sort | amount DESC, in-place |
| C | Price Anomaly Validator | Interchange Sort | amount ASC, prints swap count |
| D | Stabilization Detector | Bubble Sort (early stop) | amount ASC, true only if first pass has no swaps |
| E | Historical Sorter | Lomuto Quick Sort (median-of-three) | amount ASC, timestamp ASC on ties |

**Task D interpretation:** the provided rubric defines `STABLE` as *already sorted on the first pass*, not that the numeric price has stopped changing over time. An initially unsorted array returns `false` after being fully sorted. 

**Task E duplicate robustness:** Lomuto partitions can be very unbalanced for equal keys; the implementation recurses on the smaller partition and iterates over the larger partition, so large groups of identical keys still terminate without excessive recursion stack growth. This does **not** change the quadratic theoretical worst case.

## Measured results (this environment)

- Automated tests: **42/42 PASS** across 5 algorithms and a file-loader regression case.
- 100,000-bid Quick Sort average (5 runs): **30.890 ms**; fastest: **27.458 ms**.
- Source of truth: [`evidence/benchmark_output.txt`](evidence/benchmark_output.txt). Times vary by machine and load.

## Evidence and study materials

- [`REFLECTION.md`](REFLECTION.md) — brief, rubric-mapped answers
- [`docs/TEST_RESULTS.md`](docs/TEST_RESULTS.md) — edge cases, commands, expected outcomes
- [`docs/AI_USAGE_LOG.md`](docs/AI_USAGE_LOG.md) — actual AI interactions and validation boundaries
- [`docs/DEFENSE_QA.md`](docs/DEFENSE_QA.md) — questions for a live defense
- [`screenshot_runtime.png`](screenshot_runtime.png) — console-style image rendered directly from actual saved runtime output

## AI transparency

The initial reference implementation and documentation were developed with AI assistance. The written assignment restricts submitting LLM-authored sorting implementations verbatim. Before academic submission, the student should review and independently adapt the code in accordance with the instructor's explicit rules or document a specific instructor-approved exception. All timings and test evidence in this repository are from actual executions, not invented values.
