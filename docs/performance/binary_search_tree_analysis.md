The following are some benchmarks on how fast the algorithm can insert and delete nodes from a list. This is testing both the balanced variant and non-balanced variant. The nodes have been inserted in numerical order (1,2,3,4,5,6,…). This is to test the worst-case scenario of both list types.

# Insertion – Without Balancing

100 Nodes - 0.00005389009 Seconds

1000 Nodes - 0.0025065 Seconds

10000 Nodes - 0.270226 Seconds

100000 Nodes - 66.5651 Seconds

![Insertion - Without Balancing](https://user-images.githubusercontent.com/12601671/231076212-e97789cd-7386-4fa0-b62e-6bad68af998d.png)

# Insertion – With Balancing

100 Nodes - 0.0001328 Seconds

1000 Nodes - 0.0009814 Seconds

10000 Nodes - 0.0143087 Seconds

100000 Nodes - 0.204116 Seconds

![Insertion - With Balancing](https://user-images.githubusercontent.com/12601671/231076210-62acc976-2fae-4851-b7f3-1d474b522532.png)

Deletion will be tested in a similar manner. All items will be inserted in sorted order (1,2,3,4,5,6,…), but they will be deleted in reverse order (100,99,98,97,…). This will test the worst-case scenario for deleting elements in both list types.

# Deletion – Without Balancing

100 Nodes - 0.00019689831 Seconds

1000 Nodes - 0.0022997 Seconds

10000 Nodes - 0.259401 Seconds

100000 Nodes - 63.909 Seconds

![Deletion - Without Balacing](https://user-images.githubusercontent.com/12601671/231076208-1abeae8c-3420-47d5-83d6-cf6ef5eb9cfc.png)

# Deletion – With Balancing

100 Nodes - 0.00003312274 Seconds

1000 Nodes - 0.0011137 Seconds

10000 Nodes - 0.0157855 Seconds

100000 Nodes - 0.244405 Seconds

![Deletion - With Balancing](https://user-images.githubusercontent.com/12601671/231076206-e885ceb9-3020-43b5-b511-3313ec283934.png)

# Conclusion

In conclusion, it is pretty clear that balancing the tree when inserting and deleting nodes makes the tree much faster. By balancing the tree, insertions can be 300x faster and deletions can be 250x faster. It also seems as if the insertions and deletions without balance follow an exponential curve. I find this to be odd, since after analyzing my own algorithm, it should be identical to a normal BST algorithm, which is O(N) in the worst-case scenario. There may be some overhead when running the algorithm in Debug mode, but I'm not sure.