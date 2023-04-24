/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * 
 * @author Miguel Luna García
 * @since 18 Mar 2023
 * @file greedy.h
 * @brief Greedy class
 *        This class implements a greedy algorithm for the maximum diversity problem
 */

#ifndef GREEDY_H
#define GREEDY_H

#include <vector>
#include <set>
#include <cmath>
#include "solution.h"

/**
 * @brief Defines a greedy algorithm for the maximum diversity problem
 */
class Greedy {
 public:
  Greedy();
  Solution solve(const Problem& problem, int k);
};

Greedy::Greedy() {}

Solution Greedy::solve(const Problem& problem, int k) {
  Solution solution(problem.dimensions());
  std::set<int> remaining_points;
  for (int i{0}; i < problem.size(); ++i) {
    remaining_points.insert(i);
  }
  while (solution.size() < k) {
    int best_point{0};
    Point center = problem.centroid(remaining_points);
    double best_distance{euclidean_distance(problem[0], center)};
    for (int point: remaining_points) {
      double distance{euclidean_distance(problem[point], center)};
      if (distance > best_distance) {
        best_distance = distance;
        best_point = point;
      }
    }
    solution.insert(best_point);
    remaining_points.erase(best_point);
  }
  
  return solution;
}

#endif  // GREEDY_H