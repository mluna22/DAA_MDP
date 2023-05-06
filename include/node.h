/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * 
 * @author Miguel Luna García
 * @since 18 Mar 2023
 * @file graph.h
 * @brief Node class
 *        This class implements a Node for the branch and bound algorithm
 */


#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <vector>
#include <solution.h>

class Node {
 public:
  Node(Solution solution, double upper_bound, int tag, int level);
  Solution get_solution() const;
  double get_upper_bound() const;
  int get_tag() const;
  int get_level() const;
  const bool operator<(const Node& other) const; 
 private:
  Solution solution_;
  double upper_bound_;
  int tag_;
  int level_;
};

Node::Node(Solution solution, double upper_bound, int tag, int level) {
  solution_ = solution;
  upper_bound_ = upper_bound;
  tag_ = tag;
  level_ = level;
}

Solution Node::get_solution() const {
  return solution_;
}

double Node::get_upper_bound() const {
  return upper_bound_;
}

int Node::get_tag() const {
  return tag_;
}

int Node::get_level() const {
  return level_;
}

const bool Node::operator<(const Node& other) const {
  if (level_ == other.level_) {
    return upper_bound_ < other.upper_bound_;
  }
  return level_ < other.level_;
}

#endif  // GRAPH_H_
