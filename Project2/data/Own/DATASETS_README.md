# Delivery Truck Pallet Packing Optimization - Test Datasets

This document describes the test datasets provided for the Delivery Truck Pallet Packing Optimization problem. Each dataset is designed to test specific aspects of your algorithms and highlight the trade-offs between different approaches.

Notice that the datasets (as well as this document ((except this phrase))) where created using AI, so there might be a few things wrong.

## Dataset Characteristics

### Dataset 01 - Basic Test Case
- **Truck Capacity:** 100
- **Number of Pallets:** 9
- **Characteristics:** Small dataset suitable for initial testing and algorithm development.
- **Purpose:** Provides a simple baseline for verifying correctness of all algorithm implementations.
- **Not an edge case:** This is a standard, well-balanced test case.

### Dataset 02 - Medium Size
- **Truck Capacity:** 500
- **Number of Pallets:** 20
- **Characteristics:** Medium-sized dataset with varied weight-profit relationships.
- **Purpose:** Tests basic performance differences between exhaustive and more efficient approaches.
- **Not an edge case:** This is a standard test case with moderate complexity.

### Dataset 03 - Large Size
- **Truck Capacity:** 1000
- **Number of Pallets:** 50
- **Characteristics:** Large dataset that should still be manageable with dynamic programming.
- **Purpose:** Demonstrates the performance limits of brute-force approaches and the advantages of dynamic programming.
- **Not an edge case:** Standard large test case.

### Dataset 04 - Very Large Size
- **Truck Capacity:** 5000
- **Number of Pallets:** 100
- **Characteristics:** Very large dataset impractical for brute force approaches.
- **Purpose:** Demonstrates the necessity of efficient algorithms like dynamic programming and approximation methods.
- **Edge case:** Pushes the limits of what dynamic programming can handle efficiently due to the number of pallets and large capacity.

### Dataset 05 - Same Weight-to-Profit Ratio
- **Truck Capacity:** 400
- **Number of Pallets:** 15
- **Characteristics:** All pallets have the exact same weight-to-profit ratio (2:1).
- **Purpose:** Tests greedy algorithms that rely on weight-to-profit ratio sorting.
- **Edge case:** Since all items have identical profit-to-weight ratios, greedy algorithms that sort by this metric will struggle to make optimal decisions, as there's no clear preference between items. For this specific case, the optimal solution might involve taking the maximum number of lightest items or heaviest items depending on how the capacity aligns.

### Dataset 06 - Tight Capacity Constraint
- **Truck Capacity:** 100
- **Number of Pallets:** 10
- **Characteristics:** Total weight of all pallets (275) significantly exceeds truck capacity.
- **Purpose:** Tests how algorithms handle scenarios with tight constraints where only a small subset of items can be selected.
- **Edge case:** The tight capacity means that the algorithm must be very selective, potentially leading to situations where a suboptimal greedy choice (selecting smaller high-profit items) could outperform a typically optimal approach.

### Dataset 07 - Many Small Items
- **Truck Capacity:** 1000
- **Number of Pallets:** 30
- **Characteristics:** All pallets have small weights (≤40 units).
- **Purpose:** Tests how algorithms handle cases where many items can fit, leading to a large number of possible combinations.
- **Edge case:** With many small items that can fit within the capacity, the solution space becomes very large. This challenges exhaustive search algorithms and can also test how well approximation algorithms handle highly combinatorial problems.

### Dataset 08 - One Dominant High-Value Item
- **Truck Capacity:** 200
- **Number of Pallets:** 10
- **Characteristics:** Contains one extremely valuable pallet (weight: 180, profit: 1000) that takes up 90% of capacity.
- **Purpose:** Tests if algorithms correctly identify that the single high-value item is better than any combination of other items.
- **Edge case:** This tests whether algorithms can identify the "obvious" choice when there's one item that dominates all others in terms of profit, even though it consumes most of the capacity. Some approximation algorithms might mistakenly prefer combinations of smaller items.

### Dataset 09 - Random Weight-Profit Relationships
- **Truck Capacity:** 300
- **Number of Pallets:** 12
- **Characteristics:** Features pallets with counter-intuitive weight-profit relationships (heavy items with low profit, light items with high profit).
- **Purpose:** Challenges sorting-based heuristics and tests how well algorithms can find optimal combinations.
- **Edge case:** The unpredictable relationship between weight and profit makes it difficult for greedy algorithms to make good choices based on simple heuristics, requiring more careful optimization.

### Dataset 10 - Extreme Size
- **Truck Capacity:** 10000
- **Number of Pallets:** 200
- **Characteristics:** Extremely large dataset with high computational requirements.
- **Purpose:** Tests the limits of approximation algorithms when exact solutions are practically impossible.
- **Edge case:** This is an extreme case that would be impossible to solve optimally with exact methods on most computers, requiring efficient approximation algorithms.

## Algorithm Performance Expectations

### Exhaustive Search (Brute Force)
- **Suitable for:** Datasets 01, 02 (possibly 03 with optimizations)
- **Expected behavior:** Will find optimal solutions but becomes prohibitively slow for larger datasets.
- **Key challenges:** Dataset 03 and larger will likely cause timeouts or excessive runtime.

### Dynamic Programming
- **Suitable for:** Datasets 01-04
- **Expected behavior:** Will find optimal solutions with polynomial time complexity relative to capacity and number of items.
- **Key challenges:** Dataset 04 will test memory efficiency of the implementation. Dataset 10 will likely be too large for standard dynamic programming.

### Greedy Approximation
- **Suitable for:** All datasets
- **Expected behavior:** Will run quickly even on very large datasets, but may not find optimal solutions.
- **Key challenges:** Will likely produce suboptimal solutions for Datasets 05, 08, and 09 due to their special characteristics.

### Integer Linear Programming
- **Suitable for:** Datasets 01-04
- **Expected behavior:** Will find optimal solutions if implemented correctly.
- **Key challenges:** May struggle with Dataset 04 and larger due to computational complexity.

## Testing Methodology

When evaluating your algorithms with these datasets, consider:

1. **Correctness:** For smaller datasets where you can compute the optimal solution, compare the results of your approximation algorithms against the optimal value.

2. **Running time:** Measure and compare the execution time of different algorithms on the same dataset.

3. **Memory usage:** Monitor memory consumption, especially for dynamic programming on larger datasets.

4. **Approximation ratio:** For greedy algorithms, calculate how close the approximate solution is to the optimal solution (when available).

By systematically testing your algorithms against these diverse datasets, you'll gain valuable insights into the trade-offs between optimality, efficiency, and feasibility in solving the Delivery Truck Pallet Packing Optimization problem.