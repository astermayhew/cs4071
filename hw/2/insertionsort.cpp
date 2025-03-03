#include <forward_list>
#include <utility>

void do_sort(std::forward_list<int>& list, std::forward_list<int>::iterator last_sorted) {
  auto unsorted = last_sorted;
  ++unsorted;

  if (unsorted == list.end()) {
    return;
  }

  auto current = list.begin();
  // if insertion spot is past first element
  if (*unsorted < *current) {
    list.push_front(*unsorted);
  } else {
    for (auto prev = current++; current != unsorted; ++current) {
      if (*unsorted < *current) {
        list.insert_after(prev, *unsorted);
        break;
      }
    }

    if (current == unsorted) {
      return;
    }
  }

  auto next = ++unsorted;
  list.erase_after(last_sorted);
  do_sort(list, next);
}

void insertion_sort(std::forward_list<int>& list) {
  if (!list.empty()) {
    do_sort(list, list.begin());
  }
}


std::forward_list<int> list;
