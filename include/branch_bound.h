/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * 
 * @author Miguel Luna García
 * @since 18 Mar 2023
 * @file branch_bound.h
 * @brief Branch_Bound class
 *        This class implements a Branch & Bound algorithm for the maximum diversity problem
 */

#ifndef BRANCH_BOUND_H
#define BRANCH_BOUND_H

#include <vector>
#include <queue>
#include <set>
#include <cmath>
#include "solution.h"
#include "node.h"

struct compare_nodes_by_upper_bound {
  bool operator()(Node node1, Node node2) {
    return node1.get_upper_bound() < node2.get_upper_bound();
  }
};

struct compare_nodes_by_depth {
  bool operator()(Node node1, Node node2) {
    if (node1.get_depth() == node2.get_depth()) return node1.get_upper_bound() < node2.get_upper_bound();
    return node1.get_depth() < node2.get_depth();
  }
};

class Branch_Bound {
 public:
  Branch_Bound();
  Solution solve(const Problem& problem, int m, double lower_bound, int& generated_nodes, bool depth_search = false);
 private:
  int calculate_remaining_edges(int actual_points, int solution_size);
  double calculate_upper_bound(const Solution& solution, const Problem& problem, const std::vector<std::vector<double>>& distances, int m);
  std::vector<std::vector<double>> calculate_distances(const Problem& problem);
};

Branch_Bound::Branch_Bound() {}

int Branch_Bound::calculate_remaining_edges(int actual_points, int solution_size) {
  return (solution_size * (solution_size - 1) - actual_points * (actual_points - 1)) / 2;
}

double Branch_Bound::calculate_upper_bound(const Solution& solution, const Problem& problem, const std::vector<std::vector<double>>& distances, int m) {
  double max_distance{0};
  for (int i{0}; i < problem.size(); ++i) {
    if (solution.has_point(i)) continue;
    for (int j{0}; j < problem.size(); ++j) {
      if (distances[i][j] > max_distance) max_distance = distances[i][j];
    }
  }
  return solution.evaluate(problem) + max_distance * calculate_remaining_edges(solution.size(), m);
}

std::vector<std::vector<double>> Branch_Bound::calculate_distances(const Problem& problem) {
  std::vector<std::vector<double>> distances;
  for (int i{0}; i < problem.size(); ++i) {
    std::vector<double> row;
    for (int j{0}; j < problem.size(); ++j) {
      row.push_back(euclidean_distance(problem[i], problem[j]));
    }
    distances.push_back(row);
  }
  return distances;
}

Solution Branch_Bound::solve(const Problem& problem, int m, double lower_bound, int& generated_nodes, bool depth_search) {
  Solution best_solution;
  std::vector<std::vector<double>> distances = calculate_distances(problem);
  std::priority_queue<Node, std::vector<Node>, compare_nodes_by_upper_bound> nodes_by_upper_bound;
  std::priority_queue<Node, std::vector<Node>, compare_nodes_by_depth> nodes_by_depth;
  Node exploring_node(best_solution, lower_bound, -1, 0);
  depth_search ? nodes_by_depth.push(exploring_node) : nodes_by_upper_bound.push(exploring_node);
  generated_nodes = 1;
  while (depth_search ? !nodes_by_depth.empty() : !nodes_by_upper_bound.empty()) {
    exploring_node = depth_search ? nodes_by_depth.top() : nodes_by_upper_bound.top();
    depth_search ? nodes_by_depth.pop() : nodes_by_upper_bound.pop();
    for (int i{exploring_node.get_tag() + 1}; i < (problem.size() - (m - exploring_node.get_depth())); ++i) {
      Solution new_solution = exploring_node.get_solution();
      if (new_solution.size() == m) continue;
      new_solution.insert(i);
      generated_nodes++;
      double upper_bound = calculate_upper_bound(new_solution, problem, distances, m);
      if (upper_bound < lower_bound) continue;
      Node new_node(new_solution, upper_bound, i, exploring_node.get_depth() + 1);
      depth_search ? nodes_by_depth.push(new_node) : nodes_by_upper_bound.push(new_node);
    }
    if (exploring_node.get_upper_bound() < lower_bound) continue;
    if (exploring_node.get_solution().size() == m && exploring_node.get_solution().evaluate(problem) > best_solution.evaluate(problem)) {
      best_solution = exploring_node.get_solution();
      lower_bound = best_solution.evaluate(problem);
    }
  }
  return best_solution;
}

#endif