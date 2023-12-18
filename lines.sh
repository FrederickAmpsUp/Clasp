# ugh, you're a monster
find ./src/** ./include/clasp/** -type f \( -name "*.c" -o -name "*.h" \) -exec cat {} \; | wc -l