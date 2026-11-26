# AuctionHub — Defense Q&A

1. **Why is insertion sort suited to nearly-sorted input?** Work is proportional to n plus the number of inversions; few out-of-order bids require few shifts. It is stable under the `(amount,timestamp)` comparison.
2. **How are ties handled in Tasks A and E?** Compare amount first; if amounts match, compare timestamp ascending. If both keys match, either order is acceptable for Quick Sort.
3. **What makes selection sort different from interchange sort?** Selection sort scans for one maximum and swaps at most once per pass; interchange sort swaps immediately whenever it sees a smaller later value.
4. **Why can selection sort serve Top 3 but not a massive real-time leaderboard?** First passes move maximum values to the front, but its full sort is O(n²); a heap or partial select is better for large n.
5. **What does interchange's swap counter mean?** It increments only on real pair exchanges; it can be zero for sorted input and is generally input-order dependent.
6. **When does bubbleSortEarlyStop return true?** Only if no swaps occur on the FIRST full pass. It may sort an initially unsorted array but must still return false.
7. **Is a STABLE flag evidence that price values have stopped changing over time?** No. Here it measures ascending order of a window, not actual volatility or temporal stationarity.
8. **What is median-of-three?** Compare first, middle and last keys; move their median into the `high` position; apply Lomuto partition to that subarray.
9. **Why can identical keys still cause O(n²) Quick Sort?** Standard Lomuto `<= pivot` places all equal keys to one side, so the next problem is size n−1.
10. **Why does this implementation avoid a stack overflow on duplicates?** It recurses only into the smaller partition and loops over the larger one, guaranteeing shallow call-stack depth even when runtime work is quadratic.
11. **Why does Quick Sort use a wrapper?** It handles empty vectors and converts `size()` safely to inclusive index bounds for the required `quickSort(v,lo,hi)` API.
12. **What did the benchmark prove?** Five runs on 100,000 randomly generated records were sorted correctly; it measured runtime in this environment, not a universal time bound.
13. **What real bug was found?** `loadBids` initially appended into a non-empty vector, yielding five entries from a four-line file; it now replaces contents only after successful parsing.
14. **What happens if the C++ program receives zero bids?** All five sorting functions return safely; `bubbleSortEarlyStop` returns true and the wrapper avoids indexing `size()-1`.
15. **What is the biggest integrity risk?** The written brief prohibits copying full LLM-generated sorting functions; document actual AI assistance and perform required independent student review/adaptation.
