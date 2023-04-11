# Complexity of Two Sorting Algorithms

- **Insertion Sort**

My insertion sort algorithm is basically the same as a normal insertion sorting algorithm. It first loops over all the elements from the start to the end. For each element, it will compare its value with the previous element and if the current node is less than it's previous, then a swap is done. Then, it will check the previous of the previous and do another swap if necessary. It will continuously check with previous elements until no swap occurs.

  - **Worst Case**

In the worst case, the list is sorted in opposite order, and all the elements need to be reordered. An example is when the list needs to be sorted in ascending order, but the list is already in descending order. In this case, for each element in the list, it will have to check over all the previous elements in the list in order to find its appropriate position. This results in an exponential curve with a complexity of (O(n^2)).

  - **Best Case**

In the best case, the list is already sorted, and no reordering of elements needs to be done. In this case, the algorithm has to iterate over all the elements at least once to check if they are in sorted order, so the complexity is O(N).

  - **Average Case**

In the average case, the list is completely randomized, so the complexity of the algorithm can be a range between the worst case (O(N^2)) and the best case (O(N)).

- **Merge Sort**

The way the merge sort works is that it first finds the midpoint of a range and splits the single range into two equally sized ranges. Then, those two ranges get split into 4, then 8, then 16 and so on until the ranges cannot be split any more. Then, those many smaller ranges will get merged with each other to form the sorted list. The 16 ranges get merged into 8, then 4, then 2, and finally 1.

My merge sort works a bit differently from the usual merge sort algorithm, in that finding the midpoint for a range is not O(1). This is because in a Doubly Linked List, there is no way to randomly index the list to find the middle. Instead, all the elements have to be iterated in order for the midpoint to be found, so finding the midpoint for a single range is (O(N)), and finding the midpoints over the course of the entire algorithm is O(Nlog(N)).

Another difference is that when a merge on a range occurs, all the elements are first copied to separate arrays and then reinserted into the range via those arrays. This makes the merge sort algorithm an out-of-place algorithm. This results in a complexity of O(Nlog(N)) for copying the elements to the arrays, and then another O(Nlog(N)) to put them back.

  - **Worst Case**

In the worst case, the list is sorted in opposite order, and all the elements need to be reordered. An example is when the list needs to be sorted in ascending order, but the list is already in descending order. However, whether the elements are sorted or not doesn't change much, because the list will have all of its elements split and merged anyway. The complexity can be calculated by adding the following steps:

1. The complexity for calculating the midpoints when splitting: O(NLog(N))
2. The complexity of copying the elements to separate arrays when merging: O(NLog(N))
3. The complexity of copying the elements back to the range to complete the merge: O(NLog(N)).

Summing these steps results in a complexity of O(3NLog(N)).

  - **Best Case**

In the best case, the list is already sorted, and no reordering of elements needs to be done. However, just like in the worst-case, it doesn't matter which order the elements are in. The merge sort algorithm will split and merge all the elements anyway. So, just like with the worst-case, the complexity is O(3NLog(N)).

  - **Average Case**

In the average case, the list is completely randomized. Since the complexity if the worst and best cases are equal, the average case complexity is O(3NLog(N)).

# Data Set Analysis

**Insertion Sort – (Release/x64)**

10 Nodes - 0.0000006 Seconds

100 Nodes - 0.0000076 Seconds

1000 Nodes - 0.0005107 Seconds

10000 Nodes - 0.0540055 Seconds

100000 Nodes - 6.47657 Seconds

1000000 Nodes - 2172.35 Seconds

![Insertion Sort - Release](https://user-images.githubusercontent.com/12601671/231075027-609ff1f1-2c8d-491e-89e0-8d1f106f1014.png)

**Merge Sort – (Release/x64)**

10 Nodes – 0.0000093 Seconds

100 Nodes – 0.0000644 Seconds

1000 Nodes – 0.0006519 Seconds

10000 Nodes - 0.0077402 Seconds

100000 Nodes – 0.0860594 Seconds

1000000 Nodes – 1.0357 Seconds

![Merge Sort - Release](https://user-images.githubusercontent.com/12601671/231075024-37cadd4d-9de3-4d81-8ff5-4f128835eecd.png)

# Worst Case Analysis

For the worst-case test, the algorithms will need to sort a list that is already in descending order, and sort it into an ascending order. This will result in almost all the values getting significantly rearranged, which should show how the algorithms operate at the worst case.

**Insertion Sort – (Debug/x64)**

10 Nodes – 0.0000214 Seconds

100 Nodes – 0.0017748 Seconds

1000 Nodes – 0.176771 Seconds

10000 Nodes – 17.7715 Seconds

![Insertion Sort - Debug](https://user-images.githubusercontent.com/12601671/231075020-6493f396-174d-4f9e-9efa-130a401d9884.png)

**Merge Sort – (Debug/x64)**

10 Nodes – 0.0002228 Seconds

100 Nodes – 0.0032135 Seconds

1000 Nodes – 0.0426664 Seconds

10000 Nodes – 0.537071 Seconds

![Merge Sort - Debug](https://user-images.githubusercontent.com/12601671/231075022-502a9bef-dbb5-4ef1-b31f-b74855460867.png)

# Conclusion

In conclusion, there are two things that can be noticed when comparing the two algorithms. The first is that merge sort is way faster when sorting large amounts of elements. When sorting 1000000 elements, merge sort is a whopping 2,097% faster than insertion sort, because insertion sort has a more exponential complexity O(N^2), whereas merge sort has a more logarithmic complexity O(NLog(N)).

A second observation is that insertion sort is faster than merge sort when sorting much smaller lists. This is probably because insertion sort is less sophisticated than merge sort (recursion, array copying, and splitting), allowing it to run faster when the list size is small.