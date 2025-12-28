#!/bin/bash

# Get the current working directory
REPO_ROOT=$(git rev-parse --show-toplevel)
if [ $? -ne 0 ]; then
  exit 1
fi
echo "Current directory: $(pwd)"
echo "Repository root: ${REPO_ROOT}"

GIT_COMMIT_TEMPLATE="${REPO_ROOT}/script/git/git-commit-template"
if [ -f "${GIT_COMMIT_TEMPLATE}" ]; then
  git config commit.template ${GIT_COMMIT_TEMPLATE}
  echo "Succes to config commit template: ${GIT_COMMIT_TEMPLATE}"
else
  echo "warn: ${GIT_COMMIT_TEMPLATE} not found."
fi

GIT_HOOKS_DIR="${REPO_ROOT}/script/git/hooks"
if [ -d "${GIT_HOOKS_DIR}" ]; then
  git config core.hooksPath ${GIT_HOOKS_DIR}
  echo "Success to config git hooks path: ${GIT_HOOKS_DIR}"
else
  echo "warn: ${GIT_HOOKS_DIR} not found."
fi
