include="./include/clasp"
source="./src/ ./target/"

head_data=$(find $include -name "*.h" -type f -exec wc -l {} +)
source_data=$(find $source -name "*.c" -type f -exec wc -l {} +)

head_max=$(echo "$head_data" | awk '$2 != "total" { print $1, $2 }' | sort -n -k1 | tail -n 1)
head_min=$(echo "$head_data" | awk '$2 != "total" { print $1, $2 }' | sort -r -n -k1 | tail -n 1)

source_max=$(echo "$source_data" | awk '$2 != "total" { print $1, $2 }' | sort -n -k1 | tail -n 1)
source_min=$(echo "$source_data" | awk '$2 != "total" { print $1, $2 }' | sort -r -n -k1 | tail -n 1)

head_total_lines=$(echo "$head_data" | awk '$2 == "total" { print $1 }')
source_total_lines=$(echo "$source_data" | awk '$2 == "total" { print $1 }')

overall_max=$(echo "$head_max
$source_max" | sort -n -k1 | tail -n 1)

overall_min=$(echo "$head_min
$source_min" | sort -r -n -k1 | tail -n 1)


echo "Headers:"
echo "  Total: $(echo "$head_data" | awk '$2 == "total" { print $1 }')"
echo "  Max: $(echo $head_max | awk ' { print $1 } ') ($(basename $(echo $head_max | awk ' { print $2 } ')))"
echo "  Min: $(echo $head_min | awk ' { print $1 } ') ($(basename $(echo $head_min | awk ' { print $2 } ')))"


echo ""
echo "Source:"
echo "  Total: $(echo "$source_data" | awk '$2 == "total" { print $1 }')"
echo "  Max: $(echo $source_max | awk ' { print $1 } ') ($(basename $(echo $source_max | awk ' { print $2 } ')))"
echo "  Min: $(echo $source_min | awk ' { print $1 } ') ($(basename $(echo $source_min | awk ' { print $2 } ')))"

echo ""
echo "Overall:"
echo "  Max: $(echo $overall_max | awk ' { print $1 } ') ($(basename $(echo $overall_max | awk ' { print $2 } ')))"
echo "  Min: $(echo $overall_min | awk ' { print $1 } ') ($(basename $(echo $overall_min | awk ' { print $2 } ')))"
echo "  Total: $((head_total_lines + source_total_lines))"