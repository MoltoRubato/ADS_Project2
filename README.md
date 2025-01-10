# ADS_Project2: Dictionary with Linked List and Patricia Tree

This project involves implementing a dictionary in C using two data structures: a linked list and a Patricia (radix) tree. The dictionary supports search functionality, including handling exact matches, similar matches, and no-match scenarios, with a focus on efficiency and complexity analysis.

## Features

1. **Linked List Dictionary**
   - Stores data records as a linked list.
   - Counts binary, string comparisons, and node accesses during search operations.
   - Outputs metrics for key lookups.

2. **Patricia Tree Dictionary**
   - Implements a radix tree for efficient key lookup.
   - Includes spellchecking functionality to suggest the closest match based on edit distance.
   - Records metrics such as comparisons and node accesses during queries.

3. **Complexity Analysis**
   - Compares time and memory complexities of the linked list and Patricia tree implementations.
   - Analyzes theoretical and practical efficiency across various test cases.

## Implementation Highlights

- **Modular Design:** Separate `.c` and `.h` files for dictionary operations.
- **Space-Efficient Storage:** Optimized memory allocation for strings.
- **Single File Read:** Input data is processed in one pass.
- **Comparison Metrics:** Tracks binary, string, and node comparison counts.

## Setup Instructions

- Use the provided `Makefile`:
  - `make dict3`: Builds the linked list implementation.
  - `make dict4`: Builds the Patricia tree implementation.

## Tools used  
- Programming Language: Written in C with modular design principles. 

## Contributors
- Kerui Huang
- University of Melbourne COMP20003 Teaching Team (Question and Dataset Provider)

## License
This project is for academic purposes under the University of Melbourne's COMP20003 course.

## Note
While the dictionary works, there are some unaddressed memory leaks. Due to this project being a timed assignment, this wasn't able to be fixed during the time constraints.
