#!/usr/bin/env bash
set -euo pipefail

BIN_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BIN="$BIN_DIR/philo"
TIMEOUT=${TIMEOUT:-5}

if [ ! -x "$BIN" ]; then
	if command -v make >/dev/null 2>&1; then
		echo "Building philo binary..."
		make -C "$BIN_DIR" >/dev/null
	else
		echo "philo binary not found and make unavailable" >&2
		exit 1
	fi
fi

LAST_OUTPUT=""
LAST_EXIT=0
CHECK_FAIL_REASON=""

run_command() {
	local output
	if output=$(timeout "$TIMEOUT" "$@" 2>&1); then
		LAST_EXIT=0
	else
		LAST_EXIT=$?
	fi
	LAST_OUTPUT="$output"
}

check_monotonic() {
	local prev=-1
	local seen=0
	while IFS= read -r line; do
		if [[ $line =~ ^([0-9]+)\ ms\ ([0-9]+)\  ]]; then
			local ts=${BASH_REMATCH[1]}
			if (( ts < prev )); then
				CHECK_FAIL_REASON="timestamp regression: $ts < $prev"
				return 1
			fi
			prev=$ts
			seen=1
		fi
	done <<< "$LAST_OUTPUT"
	if [ "$seen" -eq 0 ]; then
		CHECK_FAIL_REASON="no timestamped lines found"
		return 1
	fi
	return 0
}

check_single_philo_death() {
	local timestamp_lines=0
	local last_line=""
	while IFS= read -r line; do
		last_line="$line"
		if [[ $line =~ ^([0-9]+)\ ms\ ([0-9]+)\ (.*)$ ]]; then
			timestamp_lines=$((timestamp_lines + 1))
			local id=${BASH_REMATCH[2]}
			local msg=${BASH_REMATCH[3]}
			if [ "$id" -ne 1 ]; then
				CHECK_FAIL_REASON="unexpected philosopher id $id"
				return 1
			fi
			if [ "$msg" != "is dead" ]; then
				CHECK_FAIL_REASON="unexpected message '$msg'"
				return 1
			fi
		fi
	done <<< "$LAST_OUTPUT"
	if [ "$timestamp_lines" -ne 1 ]; then
		CHECK_FAIL_REASON="expected exactly one timestamped line, got $timestamp_lines"
		return 1
	fi
	if [ "$last_line" != "All philosophers are collected" ]; then
		CHECK_FAIL_REASON="summary line missing or misplaced"
		return 1
	fi
	return 0
}

print_fail() {
	local name="$1"
	local reason="$2"
	printf "%-22s FAIL (%s)\n" "$name" "$reason"
	printf "  output: %s\n" "$LAST_OUTPUT"
}

print_ok() {
	printf "%-22s OK\n" "$1"
}

total=0
passed=0

execute_test() {
	local name="$1"
	local expected_exit="$2"
	local expected_text="$3"
	local checker="$4"
	shift 4

	total=$((total + 1))

	run_command "$@"
	local fail_reason=""
	if [ "$LAST_EXIT" -ne "$expected_exit" ]; then
		fail_reason="exit $LAST_EXIT != $expected_exit"
	fi
	if [ -z "$fail_reason" ] && [ "$expected_text" != "-" ] \
		&& ! grep -qF "$expected_text" <<<"$LAST_OUTPUT"; then
		fail_reason="missing '$expected_text'"
	fi
	if [ -z "$fail_reason" ] && [ "$checker" != "-" ]; then
		CHECK_FAIL_REASON=""
		if ! "$checker"; then
			fail_reason=${CHECK_FAIL_REASON:-"custom check failed"}
		fi
	fi
	if [ -n "$fail_reason" ]; then
		print_fail "$name" "$fail_reason"
		return
	fi
	print_ok "$name"
	passed=$((passed + 1))
}

# Argument validation tests
execute_test "missing_args" 1 "Arguments expected" - "$BIN"
execute_test "non_digit" 1 "time_to_eat must be a positive int" - "$BIN" 2 800 bad 200
execute_test "negative_philos" 1 "number_philo must be a positive int < 100000" - "$BIN" -2 800 200 200
execute_test "zero_philos" 1 "number_philo must be a positive int < 100000" - "$BIN" 0 800 200 200
execute_test "trailing_chars" 1 "number_philo must be a positive int < 100000" - "$BIN" 2a 800 200 200

# Overflow and large values
execute_test "overflow_philos" 1 "number_philo must be a positive int < 100000" - "$BIN" 2147483648 800 200 200
execute_test "overflow_die" 1 "time_to_die must be a positive int" - "$BIN" 2 2147483648 200 200

# Behavioural checks
execute_test "single_death" 0 "All philosophers are collected" check_single_philo_death "$BIN" 1 200 100 100
execute_test "monotonic_order" 0 "All philosophers are collected" check_monotonic "$BIN" 2 200 100 100 1

# Valid scenarios (should terminate quickly)
execute_test "valid_single" 0 "All philosophers are collected" - "$BIN" 1 800 200 200
execute_test "valid_with_meals" 0 "All philosophers are collected" - "$BIN" 2 200 100 100 1
execute_test "leading_spaces" 0 "All philosophers are collected" - "$BIN" "   3" 200 100 100 1

printf "\nPassed %d/%d tests\n" "$passed" "$total"

if [ "$passed" -ne "$total" ]; then
	exit 1
fi
