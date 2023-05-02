/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * 
 * @author Miguel Luna García
 * @since 18 Mar 2023
 * @file local_search.h
 * @brief Local Search class
 *        This class implements a Local Search algorithm for the maximum diversity problem
 */

#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include <vector>
#include <set>
#include <cmath>
#include "solution.h"

class Local_Search {
 public:
  Local_Search();
  Solution solve(const Problem& problem, int k);
};

Local_Search::Local_Search() {}

Solution Local_Search::solve(const Problem& problem, int k) {
  Solution solution;
  Solution remaining_points;
  for (int i{0}; i < problem.size(); ++i) {
    remaining_points.insert(i);
  }
  Point center = remaining_points.centroid(problem);
  while (solution.size() < k) {
    int best_point{0};
    double best_distance{0};
    for (int point: remaining_points) {
      double distance{euclidean_distance(problem[point], center)};
      if (distance > best_distance) {
        best_distance = distance;
        best_point = point;
      }
    }
    solution.insert(best_point);
    remaining_points.erase(best_point);
    center = solution.centroid(problem);
  }
  Solution best_solution;
  double oldvalue = solution.evaluate(problem);
  double newvalue = oldvalue;
  do {
    best_solution = solution.swap_search(problem, newvalue);
    if (newvalue > oldvalue) {
      solution = best_solution;
      oldvalue = newvalue;
    }
  } while (newvalue > oldvalue);
  return solution;
}

#endif // LOCAL_SEARCH_H