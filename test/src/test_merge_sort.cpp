#include <gtest/gtest.h>
#include <merge_sort.h>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>

// Helper function to check if a container is sorted
template<typename Container>
bool is_sorted(const Container& container) {
    if (container.empty()) return true;
    auto it = container.begin();
    auto next_it = std::next(it);
    while (next_it != container.end()) {
        if (*it > *next_it) return false;
        ++it;
        ++next_it;
    }
    return true;
}

// Test with std::vector
TEST(MergeSortTest, VectorSort) {
    std::vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    sorting_impl::merge_sort(vec);
    EXPECT_TRUE(is_sorted(vec));
}

// Test with std::list
TEST(MergeSortTest, ListSort) {
    std::list<int> lst = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    sorting_impl::merge_sort(lst);
    EXPECT_TRUE(is_sorted(lst));
}

// Test with std::deque
TEST(MergeSortTest, DequeSort) {
    std::deque<int> deq = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    sorting_impl::merge_sort(deq);
    EXPECT_TRUE(is_sorted(deq));
}

// Test with a custom container
class CustomContainer {
public:
    using iterator = typename std::vector<int>::iterator;
    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }
    void push_back(int value) { data.push_back(value); }
    size_t size() const { return data.size(); }
private:
    std::vector<int> data;
};

TEST(MergeSortTest, CustomContainerSort) {
    CustomContainer custom;
    custom.push_back(3);
    custom.push_back(1);
    custom.push_back(4);
    custom.push_back(1);
    custom.push_back(5);
    custom.push_back(9);
    custom.push_back(2);
    custom.push_back(6);
    custom.push_back(5);
    custom.push_back(3);
    custom.push_back(5);

    sorting_impl::merge_sort(custom);
    EXPECT_TRUE(is_sorted(custom));
}

// Test with a user-defined comparer
TEST(MergeSortTest, UserDefinedComparer) {
    std::vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    auto comparer = [](const int& a, const int& b) { return a < b; };
    sorting_impl::merge_sort(vec, comparer);
    EXPECT_TRUE(is_sorted(vec));
}

// Test with a user-defined swapper
TEST(MergeSortTest, UserDefinedSwapper) {
    std::vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    auto swapper = [](int& a, int& b) { std::swap(a, b); };
    sorting_impl::merge_sort(vec, std::less<int>(), swapper);
    EXPECT_TRUE(is_sorted(vec));
}
