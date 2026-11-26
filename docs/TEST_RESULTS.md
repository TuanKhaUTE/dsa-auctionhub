# AuctionHub — Test Results and Reproduction

## Environment

- GCC/g++ 14.2.0, `-std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror`
- `std::chrono::steady_clock` for milliseconds, five runs
- Source: `auctionhub.cpp`, input: `test_data/large.txt`

## Mandatory scenario checks

| Task | Check | Expected | Verified |
|---|---|---|---|
| A | `alice 100, bob 102.5, carol 101, dave 103` | alice, carol, bob, dave | PASS |
| A/E | amount ties with different timestamps | timestamp ASC | PASS |
| B | Same sample: Top 3 | dave, bob, carol | PASS |
| B | Selection implementation | in-place, no auxiliary vector | source review |
| C | Four-record sample | ascending; prints swap count 1 | PASS |
| D | Already sorted | returns true, STABLE | PASS |
| D | Reverse-sorted | returns false, fully sorted | PASS |
| D | Initially unsorted but sorted after later pass | returns false | PASS |
| E | Median-of-three pivot & wrapper | sorted by amount/time; `quickSortWrapper` | PASS |
| E | 100,000 random bids | all records present, sorted | PASS 5/5 |
| E | 2,000 records with identical amounts AND timestamps | terminates; sorted | PASS |

## Comprehensive automated matrix

Eight input families × five sorting functions = **40/40** checks, plus 2,000-equal-key Quick Sort stress check and file loader regression check = **42/42 PASS**.

Inputs: empty, singleton, sorted, reversed, all-equal `(amount,timestamp)`, price ties with different timestamps, near-sorted, and negative/zero synthetic values. The last category is a correctness stress test, not a claim that negative bids are valid on the platform.

The sorter test checks both sortedness and multiset record preservation using all Bid fields, not only the number of records. For Task D it separately checks whether the return value matches the sortedness of the **original** input.

## Actual benchmark

```text
=== QUICK SORT BENCHMARK ===
Records: 100000 | pivot: median-of-three | trials: 5
Run 1: 30.735 ms | PASS
Run 2: 27.458 ms | PASS
Run 3: 29.248 ms | PASS
Run 4: 30.314 ms | PASS
Run 5: 36.697 ms | PASS
Average: 30.890 ms | Fastest: 27.458 ms
Validation: sorted ascending by amount, timestamp ties; records preserved
```

Timing excludes file loading, validation, and copying and includes only the Quick Sort call. The five runs each start from a fresh copy of the **same unsorted data**. The screenshot is a console-style image generated directly from these actual saved outputs; raw logs are included independently.

## Reproduce

```bash
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror auctionhub.cpp -o auctionhub
./auctionhub --self-test
./auctionhub --demo test_data/small.txt
./auctionhub --benchmark test_data/large.txt
```
