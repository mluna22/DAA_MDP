/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * 
 * @author Miguel Luna García
 * @since 18 Mar 2023
 * @file problem.h
 * @brief Solution Class
 *        This file contains the Solution class declaration
 */

#ifndef SOLUTION_H
#define SOLUTION_H

#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include "problem.h"

/**
 * @brief Defines a solution to the problem
*/
class Solution {
 public:
  Solution();
  std::set<int>::iterator begin() const;
  std::set<int>::iterator end() const;
  void insert(int i);
  void erase(int i);
  void clear();
  const int size() const;
  const double evaluate(const Problem& problem) const;
  const bool operator==(const Solution& other) const;
  const bool operator!=(const Solution& other) const;
  Point centroid(const Problem& problem) const;
  Solution swap_search(const Problem& problem, double& value) const;
  bool has_point(int i) const;
  friend std::ostream& operator<<(std::ostream& os, Solution& solution);
 private:
  std::set<int> points_;
};

Solution::Solution() {}

std::set<int>::iterator Solution::begin() const {
  return points_.begin();
}

std::set<int>::iterator Solution::end() const {
  return points_.end();
}

void Solution::insert(int i) {
  points_.insert(i);
}

void Solution::erase(int i) {
  points_.erase(i);
}

void Solution::clear() {
  points_.clear();
}

const int Solution::size() const {
  return points_.size();
}

const double Solution::evaluate(const Problem& problem) const {
  double sum_of_distances{0};
  for (std::set<int>::iterator point{points_.begin()}; point != points_.end(); ++point) {
    std::set<int>::iterator other_point{point};
    other_point++;
    for (; other_point != points_.end(); ++other_point) {
      sum_of_distances += euclidean_distance(problem[*point], problem[*other_point]);
    }
  }
  return sum_of_distances;
}

const bool Solution::operator==(const Solution& other) const {
  return points_ == other.points_;
}

const bool Solution::operator!=(const Solution& other) const {
  return points_ != other.points_;
}

Point Solution::centroid(const Problem& points) const {
  Point centroid(points[0].size(), 0);
  for (int point: points_) {
    for (int i{0}; i < centroid.size(); ++i) {
      centroid[i] += points[point][i];
    }
  }
  for (int i{0}; i < centroid.size(); ++i) {
    centroid[i] /= points_.size();
  }
  return centroid;
}

Solution Solution::swap_search(const Problem& problem, double& value) const {
  double best_evaluation{value};
  Solution best_solution = *this;
  for (int point: points_) {
    for (int i{0}; i < problem.size(); ++i) {
      if (points_.find(i) == points_.end()) {
        Solution new_solution = *this;
        new_solution.points_.erase(point);
        new_solution.points_.insert(i);
        double new_evaluation{value};
        for (int point_check: points_) {
          if (point_check != point) {
            new_evaluation -= euclidean_distance(problem[point], problem[point_check]);
            new_evaluation += euclidean_distance(problem[i], problem[point_check]);
          }
        }
        if (new_evaluation > best_evaluation) {
          best_evaluation = new_evaluation;
          best_solution = new_solution;
        }
      }
    }
  }
  value = best_evaluation;
  return best_solution;
}

bool Solution::has_point(int i) const {
  return points_.find(i) != points_.end();
}

std::ostream& operator<<(std::ostream& os, Solution& solution) {
  std::set<int>::iterator point{solution.begin()};
  os << *point;
  for (++point; point != solution.end(); ++point) {
    os << "-" << *point;
  }
  return os;
}

#endif  // SOLUTION_H