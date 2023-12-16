# ugh, you're a monster
find ./** -type f \( -name "*.c" -o -name "*.h" \) -not -path "./CMakeFiles/*" -not -path "./build/*" -exec cat {} \; | wc -l