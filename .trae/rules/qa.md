# C++ QA Skill

Systematically QA test a C++ project: build, run tests, run static analysis, run memory checkers, then iteratively fix bugs. Produces before/after health scores and a ship-readiness summary.

## When to Use

When asked to "qa", "QA", "test this", "find bugs", "test and fix", or "fix what's broken".

## Parameters

| Parameter | Default | Override |
|-----------|---------|----------|
| Tier | Standard | `--quick` (critical+high only), `--exhaustive` (+cosmetic) |
| Scope | Full project | `Focus on <module>` |
| Sanitizers | ASan+UBSan | `--no-sanitizers`, `--tsan` |

## Step 0: Pre-flight

1. **Check clean working tree:**
   ```bash
   [ -n "$(git status --porcelain)" ] && echo "ERROR: Dirty working tree. Commit or stash first." && exit 1
   ```

2. **Check toolchain:**
   ```bash
   cmake --version >/dev/null 2>&1 || echo "CMAKE:MISSING"
   clang++ --version >/dev/null 2>&1 || g++ --version >/dev/null 2>&1 || echo "CXX:MISSING"
   ctest --version >/dev/null 2>&1 || echo "CTEST:MISSING"
   clang-tidy --version >/dev/null 2>&1 || echo "CLANG_TIDY:MISSING"
   ```

3. **Create output directory:**
   ```bash
   mkdir -p .qa-reports
   ```

## Phase 1: Build

```bash
cmake -B build_debug -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCXXAUX_ENABLE_TESTS=ON \
  -DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic" 2>&1 | tee .qa-reports/cmake.log

cmake --build build_debug --parallel $(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4) 2>&1 | tee .qa-reports/build.log
```

**Parse build output for:**
- Compilation errors → STOP, no point running tests
- Warnings: `-Wunused-*`, `-Wshadow`, `-Wconversion`, `-Wnull-dereference`, `-Wformat-security`

## Phase 2: Unit Tests

```bash
ctest --test-dir build_debug --output-on-failure -V 2>&1 | tee .qa-reports/test.log
```

**Parse for:**
- Pass/fail counts
- FAILED test names and assertion messages
- Segfault or signal-based failures (memory safety red flag)
- Timeout failures (possible hang/infinite loop)

## Phase 3: Static Analysis

```bash
# clang-tidy (if compile_commands.json available)
if [ -f build_debug/compile_commands.json ]; then
  find include src -name "*.h" -o -name "*.cpp" | \
    head -50 | xargs clang-tidy -p build_debug 2>&1 | tee .qa-reports/clang-tidy.log
fi
```

**Flag these clang-tidy checks:**
- `bugprone-*` → critical/high
- `cppcoreguidelines-*` → high
- `clang-analyzer-*` → high
- `performance-*` → medium
- `modernize-*` → informational
- `readability-*` → low

## Phase 4: Memory Analysis

```bash
# AddressSanitizer build
cmake -B build_asan -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer" \
  -DCMAKE_C_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer" \
  -DCXXAUX_ENABLE_TESTS=ON 2>&1

cmake --build build_asan --parallel $(sysctl -n hw.ncpu 2>/dev/null || echo 4) 2>&1

ASAN_OPTIONS=halt_on_error=0:detect_leaks=1 \
UBSAN_OPTIONS=halt_on_error=0:print_stacktrace=1 \
ctest --test-dir build_asan --output-on-failure -V 2>&1 | tee .qa-reports/asan.log
```

**Flag these ASan/UBSan findings:**
- Heap use-after-free → critical
- Buffer overflow/underflow → critical
- Memory leaks (definitely lost) → critical
- Memory leaks (reachable) → high
- Uninitialized value reads → high
- Invalid free / double-free → critical

**If ThreadSanitizer requested:**
```bash
cmake -B build_tsan -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=thread -fno-omit-frame-pointer" \
  -DCXXAUX_ENABLE_TESTS=ON
cmake --build build_tsan --parallel
ctest --test-dir build_tsan --output-on-failure -V 2>&1 | tee .qa-reports/tsan.log
```

## Phase 5: Health Score

| Category | Weight | Scoring |
|----------|--------|---------|
| Build | 25% | 0 errors 0 warnings=100, 1-5 warnings=80, 6-20=60, 20+=40, any error=0 |
| Tests | 35% | All pass=100, 1-2 fail=60, 3-10 fail=30, 10+ or segfault=0 |
| Static Analysis | 20% | 0 findings=100, 1-3=70, 4-10=40, 10+=10 |
| Memory Safety | 20% | Clean=100, reachable leaks=70, definite leaks/UAF=20, overflow/double-free=0 |

`score = Σ (category_score × weight)`

## Phase 6: Fix Loop

For each fixable issue, in severity order:

1. **Locate source** — find the responsible file(s), ONLY modify directly related files
2. **Fix** — minimal fix, smallest change that resolves the issue
3. **Commit** — `git commit -m "fix(qa): ISSUE-NNN — short description"` (one commit per fix)
4. **Re-test** — rebuild affected target, re-run relevant tests, re-run sanitizers
5. **Classify** — verified / best-effort / reverted

### Regression Test

For each verified fix:
- Read 2-3 existing test files to match project conventions (GoogleTest)
- Write a regression test that exercises the exact precondition that triggered the bug
- Include attribution comment: `// Regression: ISSUE-NNN — {what broke}`
- Run the new test, commit if passing

### Self-Regulation

Every 5 fixes, compute WTF-likelihood:
- Start at 0%, each revert +15%, each fix touching >3 files +5%, after fix 15: +1% per fix
- If WTF > 20%: STOP and show progress
- Hard cap: 50 fixes

## Phase 7: Report

Write `.qa-reports/qa-report-{YYYY-MM-DD}.md`:

```
# QA Report — cxx-aux — {date}

## Health Score: {baseline} → {final}

## Build
- Compiler: {compiler}
- Errors: N, Warnings: M

## Tests
- Pass: N, Fail: M

## Static Analysis
- clang-tidy findings: N

## Memory Safety
- ASan errors: N, UBSan errors: N

## Issues Found: N (X critical, Y high, Z medium, W low)

## Fixes Applied: M (verified: A, best-effort: B, reverted: C)

## Top 3 Things to Fix
1. ...
2. ...
3. ...
```

## cxx-aux Specific QA Checks

- **flat_vector/flat_string**: Test with non-trivially-destructible types (e.g., `std::string`) — verify no leaks
- **small_vector**: Benchmark `std::visit` overhead vs direct branching
- **ObjectPool**: Test concurrent allocation/deallocation
- **StringTable**: Test iterator stability after insertion
- **ranges views**: Test with empty ranges, single-element ranges, and move-only types
- **ELF parser**: Test with malformed ELF files (corrupt headers, truncated sections)
- **Platform guards**: Verify `<immintrin.h>` is conditionally included

## Important Rules

1. Build errors block everything — if it doesn't compile, nothing else matters
2. Memory errors are critical — any ASan/valgrind error is high or critical
3. Never skip static analysis — clang-tidy catches real bugs
4. Test failures are bug reports — a failing test is never "just a test issue"
5. One commit per fix — never bundle multiple fixes
6. Revert on regression — if a fix makes things worse, `git revert HEAD` immediately
7. Self-regulate — follow the WTF-likelihood heuristic, stop and ask when in doubt
