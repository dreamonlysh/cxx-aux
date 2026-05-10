# C++ Ship Skill

Ship workflow: merge base branch, build, test, review, version bump, changelog, commit, push, create PR.

## When to Use

When asked to "ship", "create PR", "submit this", or "merge this".

## Step 1: Pre-flight

1. Check current branch. If on main/default: **abort** — "Ship from a feature branch."
2. Run `git status` — uncommitted changes are always included.
3. Understand what's being shipped:
   ```bash
   git diff origin/<base>...HEAD --stat
   git log origin/<base>..HEAD --oneline
   ```
4. Detect base branch:
   - `gh pr view --json baseRefName -q .baseRefName` (if PR exists)
   - `gh repo view --json defaultBranchRef -q .defaultBranchRef.name` (fallback)
   - `main` (final fallback)

## Step 2: Merge Base Branch

```bash
git fetch origin <base> && git merge origin/<base> --no-edit
```

- Auto-resolve simple conflicts (VERSION, CHANGELOG ordering)
- Complex conflicts → STOP and show them

## Step 3: Build and Test

### Build

```bash
cmake -B build_debug -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCXXAUX_ENABLE_TESTS=ON \
  -DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic" 2>&1

cmake --build build_debug --parallel $(sysctl -n hw.ncpu 2>/dev/null || echo 4) 2>&1
```

Build fails → STOP, show errors.

### Run Tests

```bash
ctest --test-dir build_debug --output-on-failure -V 2>&1
```

Tests fail → STOP, show failures.

### Static Analysis (non-blocking)

```bash
git diff origin/<base> --name-only | grep -E "\.(cpp|cxx|cc|h)$" | \
  xargs clang-tidy -p build_debug 2>&1 || true
```

Report findings in PR body but don't block.

## Step 3.5: Sanitizer Run (conditional)

Run sanitizers when source files (not docs/tests) changed:

```bash
cmake -B build_asan -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer" \
  -DCXXAUX_ENABLE_TESTS=ON

cmake --build build_asan --parallel

ASAN_OPTIONS=halt_on_error=0:detect_leaks=1 \
ctest --test-dir build_asan --output-on-failure -V 2>&1
```

ASan/UBSan errors → STOP. Memory safety errors are release blockers.

If only docs/comments/test changes: "No high-risk files changed — skipping sanitizer run."

## Step 4: Pre-Landing Review

Apply the review skill's two-pass checklist against the diff:
1. **Pass 1 (CRITICAL):** Memory safety, UB, thread safety, RAII
2. **Pass 2 (INFORMATIONAL):** API design, const correctness, dead code, test gaps

Classify findings as AUTO-FIX or ASK:
- Auto-fix mechanical issues directly
- Batch-ask design judgment items

If any fixes applied: commit, then **STOP** — tell user to re-run ship.

## Step 5: Version Bump

1. Read current `VERSION` file (format: `MAJOR.MINOR.PATCH.MICRO`)
2. Auto-decide bump level:
   - **MICRO**: < 50 lines, trivial tweaks, typos
   - **PATCH**: 50+ lines, bug fixes, small features
   - **MINOR**: ASK user — major features or architectural changes
   - **MAJOR**: ASK user — milestones or breaking changes
3. Write new version to `VERSION`

## Step 6: CHANGELOG

1. Read `CHANGELOG.md` header to know the format
2. Auto-generate entry from all commits on the branch:
   ```bash
   git log origin/<base>..HEAD --oneline
   git diff origin/<base>...HEAD
   ```
3. Categorize: `### Added`, `### Changed`, `### Fixed`, `### Removed`
4. Insert after header, dated today: `## [X.Y.Z.W] - YYYY-MM-DD`

## Step 7: Commit (bisectable chunks)

Group changes into logical commits:
1. **Build system** — CMakeLists.txt, toolchain config
2. **Interfaces/headers** — .h changes
3. **Implementations** — .cpp with their tests
4. **Tests** — additional test files
5. **VERSION + CHANGELOG** — always final commit

Rules:
- Header + implementation = same commit
- Source + its test = same commit
- CMakeLists.txt goes with the target it affects
- < 50 lines across < 4 files → single commit is fine
- Each commit must be independently valid

## Step 8: Push and Create PR

```bash
git push -u origin <branch-name>
```

```bash
gh pr create --base <base> --title "<type>: <summary>" --body "$(cat <<'EOF'
## Summary
<bullet points from CHANGELOG>

## Build & Test Results
- Compiler: {compiler}, Warnings: N
- Tests: N pass, 0 fail

## Sanitizer Results
- ASan/UBSan: pass / {findings}

## Pre-Landing Review
{findings or "No issues found."}

## Test plan
- [x] Build passes with zero errors
- [x] All unit tests pass
- [x] No ASan/UBSan errors
- [x] clang-tidy: N findings (M auto-fixed)
EOF
)"
```

Output the PR URL — this is the final output the user sees.

## Important Rules

- Never skip tests — if tests fail, stop
- Never skip the pre-landing review
- Never force push
- Only ask for confirmation on MINOR/MAJOR version bumps and ASK review items
- Always use 4-digit version format from VERSION file
- Split commits for bisectability
- The goal: user says "ship", next thing they see is the PR URL
