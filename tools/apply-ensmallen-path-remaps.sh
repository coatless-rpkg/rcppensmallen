#!/usr/bin/env bash
#
# Apply permanent path remaps to the bundled 'ensmallen' headers so every file
# path in the built source tarball stays within R's 100-byte "portable file
# names" limit.
#
# The "Update Ensmallen" workflow calls this right after extracting a new
# upstream release (which wipes and re-extracts inst/include/ensmallen_bits
# verbatim). It is also safe to run locally and is idempotent.
#
# Run from the package root:  bash tools/apply-ensmallen-path-remaps.sh
#
# Manifest: tools/ensmallen-path-remaps.tsv  (old<TAB>new, relative to inst/include/)
#
set -euo pipefail

INCLUDE_DIR="inst/include"
MANIFEST="tools/ensmallen-path-remaps.tsv"
PKG_PREFIX="RcppEnsmallen"   # tarball top-level dir; paths ship as PKG_PREFIX/<relpath>
MAX_BYTES=100

if [ ! -f "$MANIFEST" ]; then
  echo "ERROR: manifest not found: $MANIFEST (run from the package root)" >&2
  exit 1
fi

# --- Apply each remap -------------------------------------------------------
while IFS=$'\t' read -r old new _rest; do
  # Skip blank lines and comments.
  case "${old:-}" in
    ''|\#*) continue ;;
  esac
  if [ -z "${new:-}" ]; then
    echo "ERROR: manifest entry has no target: '$old'" >&2
    exit 1
  fi

  old_path="$INCLUDE_DIR/$old"
  new_path="$INCLUDE_DIR/$new"
  old_base="$(basename "$old")"
  new_base="$(basename "$new")"

  if [ -f "$new_path" ] && [ ! -f "$old_path" ]; then
    echo "= already remapped: $old_base -> $new_base"
  elif [ -f "$old_path" ]; then
    mkdir -p "$(dirname "$new_path")"
    mv "$old_path" "$new_path"
    echo "> renamed: $old -> $new"
  else
    echo "ERROR: source header not found (upstream layout changed?): $old_path" >&2
    exit 1
  fi

  # Rewrite #include references (and the @file doc comment) by basename.
  # ensmallen header basenames are globally unique, so a basename substitution
  # is safe and catches both relative and root-relative include paths.
  if [ "$old_base" != "$new_base" ]; then
    files="$(grep -rlF --include='*.hpp' "$old_base" "$INCLUDE_DIR" 2>/dev/null || true)"
    if [ -n "$files" ]; then
      pat="${old_base//./\\.}"   # escape dots for the sed pattern
      printf '%s\n' "$files" | while IFS= read -r f; do
        [ -n "$f" ] || continue
        sed -i.bak "s/${pat}/${new_base}/g" "$f"
        rm -f "$f.bak"
        echo "  patched include(s) in: ${f#"$INCLUDE_DIR"/}"
      done
    fi
  fi
done < "$MANIFEST"

# --- Guard: no shipped path may exceed MAX_BYTES ----------------------------
echo "checking inst/ paths are <= $MAX_BYTES bytes ..."
violations=0
while IFS= read -r f; do
  full="$PKG_PREFIX/$f"
  n="$(printf '%s' "$full" | wc -c | tr -d '[:space:]')"
  if [ "$n" -gt "$MAX_BYTES" ]; then
    echo "TOO LONG ($n bytes): $full" >&2
    violations=$((violations + 1))
  fi
done < <(find inst -type f)

if [ "$violations" -gt 0 ]; then
  echo "ERROR: $violations path(s) exceed $MAX_BYTES bytes." >&2
  echo "       Add a shortened mapping for each to $MANIFEST and re-run." >&2
  exit 1
fi
echo "OK: all inst/ paths are within $MAX_BYTES bytes."
