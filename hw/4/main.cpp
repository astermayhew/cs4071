#include <cstddef>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

std::vector<size_t> stable_match(
    const std::vector<std::vector<size_t>>& proposer_lists,
    const std::vector<std::vector<size_t>>& proposee_lists) {
  std::vector<size_t> proposers_top_choices(proposer_lists.size());
  std::vector<size_t> proposees_matches(proposer_lists.size(),
                                        std::numeric_limits<size_t>::max());

  // create table mapping proposer index to proposee's ranking
  std::vector<std::vector<size_t>> proposees_preference_map(
      proposee_lists.size());
  for (size_t i = 0; i < proposee_lists.size(); ++i) {
    proposees_preference_map[i].resize(proposee_lists[i].size());
    for (size_t j = 0; j < proposee_lists[i].size(); ++j) {
      proposees_preference_map[i][proposee_lists[i][j]] = j;
    }
  }

  std::vector<size_t> proposer_stack(proposer_lists.size());
  std::iota(proposer_stack.rbegin(), proposer_stack.rend(), 0);

  while (!proposer_stack.empty()) {
    const size_t current_proposer = proposer_stack.back();
    const size_t target_proposee =
        proposer_lists[current_proposer]
                      [proposers_top_choices[current_proposer]++];

    if (proposees_matches[target_proposee] ==
        std::numeric_limits<size_t>::max()) {
      // proposee is unmatched, so proposee accepts
      proposer_stack.pop_back();
      proposees_matches[target_proposee] = current_proposer;

    } else if (proposees_preference_map[target_proposee]
                                       [proposees_matches[target_proposee]] >
               proposees_preference_map[target_proposee][current_proposer]) {
      // proposee prefers proposer to old match, so proposee accepts
      // add old match back to proposer stack
      proposer_stack.back() = proposees_matches[target_proposee];
      proposees_matches[target_proposee] = current_proposer;

    } else {
      // proposee rejects. try next proposee for current proposer
    }
  }

  return proposees_matches;
}

int main() {
  std::istringstream stream;
  std::string string_input;
  int num_input;

  std::vector<std::string> proposers;
  std::cout << "enter proposer names separated by spaces:\n";
  std::getline(std::cin, string_input);
  stream.str(string_input);
  while (stream >> string_input) {
    proposers.push_back(string_input);
  }

  std::vector<std::string> proposees;
  std::cout << "enter proposee names separated by spaces:\n";
  std::getline(std::cin, string_input);
  stream.clear();
  stream.str(string_input);
  while (stream >> string_input) {
    proposees.push_back(string_input);
  }

  std::vector<std::vector<size_t>> proposer_prefs;
  for (const std::string& proposer : proposers) {
    std::vector<size_t> pref_list;
    std::cout << "enter preference list for " << proposer
              << " separated by spaces:\n";
    std::getline(std::cin, string_input);
    stream.clear();
    stream.str(string_input);
    while (stream >> num_input) {
      pref_list.push_back(num_input - 1);
    }

    proposer_prefs.push_back(pref_list);
  }

  std::vector<std::vector<size_t>> proposee_prefs;
  for (const std::string& proposee : proposees) {
    std::vector<size_t> pref_list;
    std::cout << "enter preference list for " << proposee
              << " separated by spaces:\n";
    std::getline(std::cin, string_input);
    stream.clear();
    stream.str(string_input);
    while (stream >> num_input) {
      pref_list.push_back(num_input - 1);
    }

    proposee_prefs.push_back(pref_list);
  }
  std::cout << '\n';

  std::cout << "proposers:\n";
  for (int i = 0; i < proposers.size(); ++i) {
    std::cout << '\t' << proposers[i] << ": ";
    for (const size_t index : proposer_prefs[i]) {
      std::cout << proposees[index] << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n';

  std::cout << "proposees:\n";
  for (int i = 0; i < proposees.size(); ++i) {
    std::cout << '\t' << proposees[i] << ": ";
    for (const size_t index : proposee_prefs[i]) {
      std::cout << proposers[index] << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n';

  std::vector<size_t> match = stable_match(proposer_prefs, proposee_prefs);
  for (int i = 0; i < match.size(); ++i) {
    std::cout << '(' << proposers[match[i]] << ", " << proposees[i] << ")\n";
  }
}
