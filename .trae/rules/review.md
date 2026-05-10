# C++ Code Review Skill

Pre-landing code review for C++ structural issues that tests don't catch: memory safety, undefined behavior, thread safety, and API design problems.

## When to Use

When asked to "review", "code review", "check this code", or before merging changes.

## Step 1: Detect Scope

1. Get current branch: `git branch --show-current`
2. If on main/default branch with no changes: "Nothing to review." Stop.
3. Detect base branch:
   - `gh pr view --json baseRefName -q .baseRefName` (if PR exists)
   - `gh repo view --json defaultBranchRef -q .defaultBranchRef.name` (fallback)
   - `main` (final fallback)
4. Get diff: `git fetch origin <base> --quiet && git diff origin/<base> --stat`

## Step 2: Two-Pass Review

### Pass 1 — CRITICAL (memory safety, UB, thread safety)

**Memory Safety:**
- Raw owning pointers in new code (use `unique_ptr`/`shared_ptr`)
- `delete`/`delete[]` outside RAII destructors
- Buffer access with no bounds check
- `reinterpret_cast` without alignment/aliasing justification
- `memcpy`/`memset` on non-trivially-copyable types
- Raw pointer to temporary or stack object
- `std::move` on const object (silently copies)
- Missing destructor calls for placement-new'd objects in containers

**Undefined Behavior:**
- Signed integer overflow (use unsigned or `__builtin_add_overflow`)
- Left-shifting into sign bit
- Dereferencing pointer to deleted/expired object
- Accessing union member other than last-written (outside `std::variant`)
- Returning reference/pointer to local variable
- Virtual function calls in constructor/destructor
- `const` methods that modify state (even through `mutable` if semantically wrong)

**Thread Safety:**
- Shared mutable data without synchronization
- `std::shared_ptr` copy without lock (ref count is atomic, pointed-to object is NOT)
- Condition variable without loop (spurious wakeup)
- `std::async` return value discarded (destructor blocks)

**cxx-aux Specific Checks:**
- `flat_vector`/`flat_string` — are elements properly destroyed on pop/erase/clear/destruct?
- `const` methods must not write to storage (e.g., `data() const` / `c_str() const` must be pure readers)
- `#include <immintrin.h>` without platform guards
- `layout_bit_mapping` — alignment and bit manipulation correctness
- `ObjectPool` — thread safety of allocation/deallocation
- `StringTable` — iterator invalidation after insertion
- `small_vector` — union member lifetime management (placement new + `std::destroy_at`)
- `MemoryPoolAllocator::deallocate` — must destroy objects before freeing memory

### Pass 2 — INFORMATIONAL (API design, code quality)

**API Design & Const Correctness:**
- Missing `noexcept` on non-throwing functions (especially move ops, destructors, swap)
- Missing `constexpr` on compile-time-evaluable functions
- Inconsistent parameter ordering across similar functions
- Public API taking `char*` without length (use `std::string_view`)
- Missing SFINAE/concepts constraints on template parameters
- `swap` marked `noexcept` but calls potentially-throwing operations

**Magic Numbers & Coupling:**
- Hard-coded sizes, offsets, or constants without named constants
- Tight coupling between modules that should be independent

**Dead Code & Consistency:**
- Unused `#include` directives
- Redundant `std::min`/`std::max` calls
- Inconsistent error handling (mix of assert/throw/error codes)

**Test Gaps:**
- New public API without corresponding test
- Error paths not exercised by tests
- Edge cases: empty input, max-size input, null/zero values

**Enum & Switch Completeness:**
- When diff introduces new enum value or variant: grep for all switch/if-chain references to sibling values and check if new value is handled

## Step 3: Fix-First Review

**Every finding gets action — not just critical ones.**

### Classify each finding

- **AUTO-FIX**: Mechanical fixes (missing `const`, missing `noexcept`, dead code, redundant computation)
- **ASK**: Design judgment needed (naming, error strategy, ownership model, API changes)

### Auto-fix all AUTO-FIX items

Apply directly. Output one line per fix:
`[AUTO-FIXED] [file:line] Problem → what you did`

### Batch-ask about ASK items

Present all ASK items in one question:
- Number, severity, problem, recommended fix
- Per-item options: A) Fix, B) Skip
- Include overall RECOMMENDATION

### Apply user-approved fixes

Output what was fixed.

## Step 4: Documentation Cross-Reference

Check if code changes affect features described in README.md, API_GUIDE.md, or example/ files. Flag stale documentation as informational finding.

## Output Format

```
Code Review: N issues (X critical, Y informational)
[CRITICAL] file:line — Problem. Fix: recommendation
[INFORMATIONAL] file:line — Problem. Fix: recommendation
...
Summary: Top 3 things to fix
```

## Important Rules

- Read the FULL diff before commenting — don't flag issues already addressed
- Fix-first, not read-only — AUTO-FIX items are applied directly
- Be terse — one line problem, one line fix
- Only flag real problems — skip anything that's fine
- Never commit, push, or create PRs — that's the ship skill's job
