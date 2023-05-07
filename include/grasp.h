/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * 
 * @author Miguel Luna García
 * @since 18 Mar 2023
 * @file grasp.h
 * @brief Grasp class
 *        This class implements a Grasp algorithm for the maximum diversity problem
 */

#ifndef GRASP_H
#define GRASP_H

#include <vector>
#include <set>
#include <cmath>
#include "solution.h"

class GRASP {
 public:
  GRASP();
  Solution solve(const Problem& problem, int k, int iterations, int lrc_size);
 private:
  void insertLRC(std::set<int>& lrc, Solution remaining_points, const Problem& problem, int lrc_size);
};

GRASP::GRASP() {}

void GRASP::insertLRC(std::set<int>& lrc, Solution remaining_points, const Problem& problem, int lrc_size) {
  Point center = remaining_points.centroid(problem);
  while (lrc.size() < lrc_size) {
    int best_point{0};
    double best_distance{0};
    for (int point: remaining_points) {
      double distance{euclidean_distance(problem[point], center)};
      if (distance > best_distance) {
        best_distance = distance;
        best_point = point;
      }
    }
    lrc.insert(best_point);
    remaining_points.erase(best_point);
  }
}


Solution GRASP::solve(const Problem& problem, int k, int iterations, int lrc_size) {
  Solution best_solution;
  for (int iteration{0}; iteration < iterations;) {
    Solution solution;
    Solution remaining_points;
    for (int i{0}; i < problem.size(); ++i) {
      remaining_points.insert(i);
    }
    Point center = remaining_points.centroid(problem);
    while (solution.size() < k) {
      std::set<int> lrc;
      insertLRC(lrc, remaining_points, problem, lrc_size);
      int best_point = random(lrc);
      solution.insert(best_point);
      remaining_points.erase(best_point);
      center = solution.centroid(problem);
    }
    Solution search_solution;
    double oldvalue = solution.evaluate(problem);
    double newvalue = oldvalue;
    do {
      search_solution = solution.swap_search(problem, newvalue);
      if (newvalue > oldvalue) {
        solution = search_solution;
        oldvalue = newvalue;
      }
    } while (newvalue > oldvalue);
    if (solution.evaluate(problem) > best_solution.evaluate(problem)) {
      best_solution = solution;
    } else {
      ++iteration;
    }
  }
  return best_solution;
}

#endif // GRASP_H