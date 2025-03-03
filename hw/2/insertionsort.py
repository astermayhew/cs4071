class Node:
    def __init__(self, data, next):
        self.data = data
        self.next = next

    data = 0
    next = None


class List:
    def __init__(self, node):
        self.head = node

    head = None


def insertion_sort(list):
    if list.head is None:
        return

    do_sort(list, list.head)


def do_sort(list, last_sorted):
    first_unsorted = last_sorted.next
    if first_unsorted is None:
        return

    previous = list.head

    if first_unsorted.data < previous.data:
        last_sorted.next = first_unsorted.next
        first_unsorted.next = list.head
        list.head = first_unsorted
    else:
        current = previous.next
        while current is not first_unsorted:
            if first_unsorted.data < current.data:
                last_sorted.next = first_unsorted.next
                first_unsorted.next = current
                previous.next = first_unsorted
                break

            previous = current
            current = current.next

    if last_sorted.next == first_unsorted:
        do_sort(list, first_unsorted)
    else:
        do_sort(list, last_sorted)


def main():
    list = List(Node(3, Node(2, Node(4, Node(1, Node(0, None))))))
    insertion_sort(list)

    curr = list.head
    while curr is not None:
        print(curr.data)
        curr = curr.next


if __name__ == "__main__":
    main()
