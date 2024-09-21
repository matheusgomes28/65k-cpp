#!/usr/bin/env bash

set -euo pipefail

: ' This script will find run lcov to generate
    a report and then push it to codecov.
'

THIS_DIR=$(cd "$(dirname "$0")" && pwd)
PROJECT_DIR="$(cd "${THIS_DIR}/.." && pwd)"

echo "Project root dir: ${PROJECT_DIR}"
lcov --capture --directory . --output-file lcov.info \
     --rc geninfo_adjust_src_path="${PROJECT_DIR}/build/unix-deb-ninja \
     => ${PROJECT_DIR}" \
     --gcov-tool "${PROJECT_DIR}/scripts/gcov_for_clang.sh" \
     --ignore-errors inconsistent \
     --ignore-errors unused


SYSTEM_DIR="/usr/"
CPM_DIR="${HOME}/.cpm/"

# Remove the system & test dirs
lcov --ignore-errors inconsistent --remove lcov.info "${SYSTEM_DIR}*" --output-file lcov.info
lcov --ignore-errors inconsistent --remove lcov.info "${PROJECT_DIR}/tests*" --output-file lcov.info
lcov --ignore-errors inconsistent --remove lcov.info "${CPM_DIR}*" --output-file lcov.info

# Generate HTML report into coverage/
genhtml lcov.info --ignore-errors inconsistent --ignore-errors corrupt --output-directory coverage > coverage_stats.txt
LINES_COV="$(grep lines < coverage_stats.txt | grep -Eo "[0-9]+\.[0-9]+%" | cut -d '.' -f1)"
FUNCTIONS_COV="$(grep functions < coverage_stats.txt | grep -Eo "[0-9]+\.[0-9]+%" | cut -d '.' -f1)"

# Make sure we fail the pipeline if coverage is less than something
LINES_COV_THRESH="98"
if [ "$LINES_COV" -lt "$LINES_COV_THRESH" ]; then
    echo "Line coverage is less than threshold $LINES_COV_THRESH"
    exit 1
fi

FUNCTIONS_COV_THRESH="100"
if [ "$FUNCTIONS_COV" -lt "$FUNCTIONS_COV_THRESH" ]; then
    echo "Functions coverage is less than threshold $FUNCTIONS_COV_THRESH"
    exit 1
fi
