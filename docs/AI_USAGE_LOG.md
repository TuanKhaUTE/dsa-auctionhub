# AI Usage Log — Process Record

This exercise's handout distinguishes **Green** (tutoring, explanations, reviews, test inputs), **Yellow** (pseudocode/small snippets, student must translate), and **Red** (asking an LLM to write the complete assignment functions or copying generated algorithms uncritically).

## Requests actually made in this conversation

| Request (shortened/translated) | Policy classification | Observed outcome |
|---|---|---|
| “Read and evaluate my AuctionHub requirements.” | Green | Requirements, rubric, GitHub submission format were analyzed. |
| “What is our strategy for high marks?” | Green | A rubric-driven verification plan was outlined. |
| “Build the whole project, repository and files for me.” | Red according to the written brief | AI-assisted reference code and documentation were created. |

## How output was checked

- `g++` compiled the reference code with warnings treated as errors.
- 40 sorting checks across eight input families passed.
- The additional 2,000-identical-key Quick Sort test passed (no infinite recursion).
- The file loader's stale-record bug was reproduced (5 instead of 4 entries), corrected and regression-tested.
- A deterministic 100,000-bid file was generated using `std::mt19937`; five `chrono` timings were measured and independently validated against `std::sort` keys and record preservation.
- Raw logs are stored in `evidence/`; the runtime screenshot displays those actual log values.

## Remaining student responsibilities before submission

The written handout forbids directly submitting LLM-authored sorting code. Student must study, review and independently adapt the reference implementation according to the instructor's written rules, or retain evidence of a specific instructor-approved exception. Do not label an AI-created draft as entirely student-written; do not invent benchmarks, prompts, or debugging stories.
