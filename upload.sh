#!/usr/bin/env bash

# ============================================ #
# defines current time from os, uplooads it,   #
# then removes line that sets clock time so it #
# won't reset when boards is not powered       #
# ============================================ #

set -euo pipefail

FILE="${1:-src/main.cpp}"
BACKUP="$(mktemp)"

cp "$FILE" "$BACKUP"
trap 'cp "$BACKUP" "$FILE"; rm -f "$BACKUP"' EXIT

SEC=$(( $(date +%-S) + 10 ))
MIN=$(date +%-M)
HOUR=$(date +%-H)
DOW=$(date +%u) # 1=Mon..7=Sun
DOM=$(date +%-d)
MONTH=$(date +%-m)
YEAR=$(date +%Y)

sed -i \
	-e "s/#define CURRENT_SECONDS .*/#define CURRENT_SECONDS ${SEC}/" \
	-e "s/#define CURRENT_MINUTES .*/#define CURRENT_MINUTES ${MIN}/" \
	-e "s/#define CURRENT_HOURS .*/#define CURRENT_HOURS ${HOUR}/" \
	-e "s/#define CURRENT_DAY_OF_WEEK .*/#define CURRENT_DAY_OF_WEEK ${DOW}/" \
	-e "s/#define CURRENT_DAY_OF_MONTH .*/#define CURRENT_DAY_OF_MONTH ${DOM}/" \
	-e "s/#define CURRENT_MONTH .*/#define CURRENT_MONTH ${MONTH}/" \
	-e "s/#define CURRENT_YEAR .*/#define CURRENT_YEAR ${YEAR}/" \
	"$FILE"

pio run -t upload

# Remove set time function
perl -0777 -pi -e 's/(^\h*RTC\.setDS1302Time\([^;]*;\n)/\/*\n\1*\/\n/m' "$FILE"

pio run -t upload
