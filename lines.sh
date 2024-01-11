# ugh, you're a monster
find ./target/** ./src/** ./include/clasp/** -not -path "./src/extern/*" -type f \( -name "*.c" -o -name "*.h" \) -exec cat {} \; | wc -l