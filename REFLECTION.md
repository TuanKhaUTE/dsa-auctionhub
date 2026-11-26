# AuctionHub — Reflection

## 1. Why each algorithm fits its module

**A. Insertion Sort — Live Auction Monitor.** Incoming bids are usually nearly sorted, so insertion sort mainly checks each new record and shifts only occasional misplaced bids; its runtime is O(n + inversions), approaching O(n) when few records are out of order. It also preserves the specified ascending `(amount, timestamp)` order and requires O(1) auxiliary space.

**B. Selection Sort — Top-K Bid Finder.** On a small bid set, each pass selects the maximum price and places it at the next front position, making the first three entries the requested Top 3. Its fixed O(n²) number of comparisons makes it unsuitable for huge live leaderboards, but it is in-place and uses at most n - 1 actual swaps; for production-scale top-K, a heap or partial selection would be preferable.

**C. Interchange Sort — Price Anomaly Validator.** With a QA batch of at most 50 records, an easy-to-inspect pairwise comparison and immediate-swap procedure is adequate for correctness checking. It has O(n²) comparisons in all cases and can perform many swaps, so its simplicity rather than speed justifies using it for small batches.

**D. Early-Stop Bubble Sort — Stabilization Detector.** The `swapped` flag gives a direct signal that the prices were already in ascending order: if the **first full pass** requires no swaps, the function returns `true` (STABLE). This changes best-case runtime from quadratic to O(n); note that this exercise's STABLE flag describes sorted order, not whether prices stayed numerically constant during the ten-minute window.

**E. Median-of-Three Quick Sort — Full Historical Sorter.** For 100,000+ historical bids, Quick Sort is generally much more efficient on random data than the quadratic sorts, with typical O(n log n) performance. Sampling the first, middle and last keys often improves pivot choice; ties are resolved by timestamp, and a smaller-side-recursion optimization controls stack growth even on repeated equal keys.

## 2. Actual runtime for 100,000 bids

Input: 100,000 records generated using `std::mt19937` with seed `20261126`; key is amount ascending, timestamp ascending on ties. The program was compiled with GCC C++17 `-O2`, and only the sort was timed with `std::chrono::steady_clock`; five runs used identical copies of the unsorted input.

| Metric | Observed |
|---|---:|
| Quick Sort average across five runs | **30.890 ms** |
| Quick Sort fastest run | **27.458 ms** |
| Validation | 5/5 runs PASS |

See `evidence/benchmark_output.txt` and `screenshot_runtime.png`. Exact runtimes are machine-dependent and do not prove asymptotic complexity on their own.

## 3. Real bug discovered and correction

During integration testing, calling the original `loadBids()` with a non-empty output vector appended new records to the old contents. A test preloaded one record, read the four-record `small.txt`, and observed **5 records instead of 4**; I corrected it by reading into a temporary vector and replacing the destination only after successful parsing, after which the same test returned **4**. The regression check now runs during `--self-test` and passes; evidence is in `evidence/loader_bug_before_after.txt`.

## 4. Three real prompts and classification

The following are questions actually asked in the DSA chat, translated/paraphrased into English where applicable:

| Real student request | Classification | Why |
|---|---|---|
| “Read the AuctionHub assignment and evaluate what I have to submit.” | **Green** | Understanding requirements, not asking for sorting functions. |
| “What is the strategy to get a high rubric score?” | **Green** | Planning evidence, testing, and documentation. |
| “Start building everything, including the GitHub repository, for me.” | **Red** under the written handout | A full-project-generation request crosses the assignment's stated restriction. |

**Transparency:** AI produced an initial reference project and assisted with compiling, automated tests, bug diagnosis, and writing support. The last prompt is not reclassified as Green merely because the software was tested; the project should be independently reviewed and adapted to meet the instructor's stated submission policy, or a specific instructor exception should be documented. Raw AI output was not treated as proof of correctness: executable test logs and reproducible timing files were collected separately.

## 5. Does median-of-three remove Quick Sort's O(n²) worst case?

**No.** Median-of-three can select better pivots for sorted or many ordinary inputs, but it cannot guarantee balanced partitions for every possible input. If all records have equal `(amount, timestamp)` keys, the standard two-way Lomuto partition with a `<=` comparison repeatedly produces a partition of sizes `n - 1` and `0`; the total work remains O(n²), although the code still terminates because each step excludes the pivot. The code limits call-stack growth by recursing on the smaller side and continuing on the larger side iteratively, but this prevents deep recursion errors rather than converting the algorithm into O(n log n) worst-case time.
