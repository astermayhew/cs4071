#include <algorithm>
#include <cstddef>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

std::vector<size_t> stable_match(
    const std::vector<std::vector<size_t>>& proposers,
    const std::vector<std::vector<size_t>>& acceptors) {
  std::vector<size_t> proposers_top_choices(proposers.size());
  std::vector<size_t> acceptors_matches(proposers.size(),
                                        std::numeric_limits<size_t>::max());

  std::vector<std::vector<size_t>> acceptors_preference_map(acceptors.size());
  for (size_t i = 0; i < acceptors.size(); ++i) {
    acceptors_preference_map[i].resize(acceptors[i].size());
    for (size_t j = 0; j < acceptors[i].size(); ++j) {
      acceptors_preference_map[i][acceptors[i][j]] = j;
    }
  }

  std::vector<size_t> proposer_stack(proposers.size());
  std::iota(proposer_stack.rbegin(), proposer_stack.rend(), 0);

  while (!proposer_stack.empty()) {
    const size_t current_proposer = proposer_stack.back();
    const size_t target_acceptor =
        proposers[current_proposer][proposers_top_choices[current_proposer]++];
    if (acceptors_matches[target_acceptor] ==
        std::numeric_limits<size_t>::max()) {
      // acceptor is unmatched, so acceptor accepts
      proposer_stack.pop_back();
      acceptors_matches[target_acceptor] = current_proposer;
    } else if (acceptors_preference_map[target_acceptor][acceptors_matches[target_acceptor]] >
               acceptors_preference_map[target_acceptor][current_proposer]) {
      // acceptor prefers proposer to old match, so acceptor accepts
      proposer_stack.back() = acceptors_matches[target_acceptor];
      // add old match back to proposer stack
      acceptors_matches[target_acceptor] = current_proposer;
    } else {
      // acceptor rejects. try next acceptor for current proposer
    }
  }

  return acceptors_matches;
}

int main() {
  std::vector<std::string> proposers = {
    "angus",
    "floyd",
    "norman",
    "trevor",
    "wilder"
  };

  std::vector<std::string> acceptors = {
    "bertha",
    "brunhilda",
    "gertrude",
    "helga",
    "ursula"
  };

  std::vector<std::vector<size_t>> proposer_prefs = {
    {1, 0, 3, 4, 2},
    {3, 1, 0, 2, 4},
    {1, 4, 2, 3, 0},
    {0, 3, 2, 1, 4},
    {1, 3, 0, 4, 2}
  };

  std::vector<std::vector<size_t>> acceptor_prefs = {
    {4, 0, 1, 3, 2},
    {2, 1, 3, 0, 4},
    {1, 2, 3, 4, 0},
    {0, 4, 3, 2, 1},
    {3, 1, 4, 2, 0},
  };

  auto match = stable_match(proposer_prefs, acceptor_prefs);
  for (int i = 0; i < match.size(); ++i) {
    std::cout << acceptors[i] << ": " << proposers[match[i]] << '\n';
  }
}
