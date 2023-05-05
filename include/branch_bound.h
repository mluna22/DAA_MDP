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

auto compare_nodes = [](std::pair<Solution, double>& node1, std::pair<Solution, double>& node2) {
  return node1.second < node2.second;
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
  int solution_size{0};
  std::priority_queue<Node> nodes;
  Node exploring_node(best_solution, lower_bound, 0);
  while (solution_size < m) {
    for (int i{0}; i < problem.size(); ++i) {
      if (exploring_node.get_solution().has_point(i)) continue;
      Solution new_solution = exploring_node.get_solution();
      new_solution.insert(i);
      double upper_bound = calculate_upper_bound(new_solution, problem, distances, m);
      generated_nodes++;
      if (upper_bound < lower_bound) continue;
      Node new_node(new_solution, upper_bound, depth_search ? exploring_node.get_level() + 1 : 0);
      nodes.push(new_node);
    }
    if (nodes.empty()) break;
    exploring_node = nodes.top();
    nodes.pop();
    if (exploring_node.get_upper_bound() < lower_bound) continue;
    if (exploring_node.get_solution().size() > solution_size) {
      solution_size = exploring_node.get_solution().size();
      best_solution = exploring_node.get_solution();
    }
  }
  return best_solution;
}

#endif