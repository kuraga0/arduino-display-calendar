#!/usr/bin/env bash
set -euo pipefail

SRC_FILE="src/main.cpp"
BACKUP="$(mktemp)"

cp "$SRC_FILE" "$BACKUP"
trap 'cp "$BACKUP" "$SRC_FILE"; rm -f "$BACKUP"' EXIT

sec=$(date +%-S)
min=$(date +%-M)
hour=$(date +%-H)
dow=$(date +%u)   # 1=Mon..7=Sun (virtuabotixRTC ожидает свой формат — проверь под либу)
dom=$(date +%-d)
mon=$(date +%-m)
year=$(date +%Y)

sed -i \
	-e "s/#define CURRENT_SECONDS .*/#define CURRENT_SECONDS $sec/" \
	-e "s/#define CURRENT_MINUTES .*/#define CURRENT_MINUTES $min/" \
	-e "s/#define CURRENT_HOURS .*/#define CURRENT_HOURS $hour/" \
	-e "s/#define CURRENT_DAY_OF_WEEK .*/#define CURRENT_DAY_OF_WEEK $dow/" \
	-e "s/#define CURRENT_DAY_OF_MONTH .*/#define CURRENT_DAY_OF_MONTH $dom/" \
	-e "s/#define CURRENT_MONTH .*/#define CURRENT_MONTH $mon/" \
	-e "s/#define CURRENT_YEAR .*/#define CURRENT_YEAR $year/" \
	"$SRC_FILE"

pio run -t upload
