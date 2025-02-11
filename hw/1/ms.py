import random

def merge_sort(list):
    if len(list) <= 1:
        return list

    sublist_1 = merge_sort(list[:(len(list) // 2)])
    sublist_2 = merge_sort(list[(len(list) // 2):])

    pointer_1 = 0
    pointer_2 = 0
    out_list = []

    while pointer_1 < len(sublist_1) and pointer_2 < len(sublist_2):
        if sublist_1[pointer_1] < sublist_2[pointer_2]:
            out_list.append(sublist_1[pointer_1])
            pointer_1 += 1
        else:
            out_list.append(sublist_2[pointer_2])
            pointer_2 += 1

    if pointer_1 < len(sublist_1):
        out_list.extend(sublist_1[pointer_1:])
    else:
        out_list.extend(sublist_2[pointer_2:])

    return out_list

def is_sorted(list):
    if len(list) == 0:
        return True

    last = list[0]
    for n in list:
        if n < last:
            return False

        last = n

    return True

lists = []
for n in range(24):
    list = []
    for m in range(n):
        list.append(random.randint(0,99))

    lists.append(list)

for list in lists:
    sorted = merge_sort(list)
    print(sorted)
    if not is_sorted(sorted):
        print("FAILED!")
