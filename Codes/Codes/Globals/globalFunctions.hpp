#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H

#include <vector>
#include <algorithm>

template <typename ListType, typename FindType>
int findInList(const std::vector<ListType> &list, FindType find, void(*findFunction)(ListType)) {
    for (std::size_t i = 0; i < list.size(); i++) {
        if (findFunction(list[i]) == find) {
            return i;
        }
    }

    return -1;
}

template <typename FindType>
int findInList(const std::vector<FindType> &list, FindType find) {
    auto i = std::find(list.begin(), list.end(), find);
    if (i != list.end()) {
        return std::distance(list.begin(), i);
    }

    return -1;
}

#endif
