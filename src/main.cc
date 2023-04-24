/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * 
 * @author Miguel Luna García
 * @since 21 Apr 2023
 * @file main.cc
 * @brief Maximum Diversity Problem Client
 *        This program solves the maximum diversity problem using different algorithms.
*/

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <chrono>

#include "greedy.h"
#include "local_search.h"

#define N_EXECUTIONS 4

void printGreedy(std::ostream& os, std::string instance_path, Problem& problem, Greedy& algorithm) {
  for (int m{2}; m < N_EXECUTIONS + 2; ++m) {
    auto start = std::chrono::high_resolution_clock::now();
    Solution solution = algorithm.solve(problem, m);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    os << instance_path << "," << problem.size() << "," << problem.dimensions() << "," << m << "," << solution.evaluate(problem) << "," << solution << "," << elapsed.count() << std::endl;
  }
}

void printLocalSearch(std::ostream& os, std::string instance_path, Problem& problem, Local_Search& algoritm) {
  for (int m{2}; m < N_EXECUTIONS + 2; ++m) {
    auto start = std::chrono::high_resolution_clock::now();
    Solution solution = algoritm.solve(problem, m);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    os << instance_path << "," << problem.size() << "," << problem.dimensions() << "," << m << "," << solution.evaluate(problem) << "," << solution << "," << elapsed.count() << std::endl;
  }
}

Problem loadProblem(std::string instance_path) {
  std::ifstream file(instance_path);
  if (!file.is_open()) {
    std::cout << "Error opening file " << instance_path << std::endl;
    throw;
  }
  int m, n;
  file >> m >> n;
  Problem problem(m, n);
  for (int i{0}; i < m; ++i) {
    for (int j{0}; j < n; ++j) {
      file >> problem[i][j];
    }
  }
  file.close();
  return problem;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " <instance_folder>" << std::endl;
    return 1;
  }
  std::string instance_folder = argv[1];

  Greedy greedy;
  std::cout << "Algoritmo constructivo voraz" << std::endl;
  std::cout << "Problema,n,k,m,z,S,CPU(s)" << std::endl;
  for (const auto& entry : std::filesystem::directory_iterator(instance_folder)) {
    std::string instance_path = entry.path();
    Problem matrix = loadProblem(instance_path);
    printGreedy(std::cout, instance_path, matrix, greedy);
  }

  Local_Search localsearch;
  std::cout << "Algoritmo de búsqueda local" << std::endl;
  std::cout << "Problema,n,k,m,z,S,CPU(s)" << std::endl;
  for (const auto& entry : std::filesystem::directory_iterator(instance_folder)) {
    std::string instance_path = entry.path();
    Problem matrix = loadProblem(instance_path);
    printLocalSearch(std::cout, instance_path, matrix, localsearch);
  }

  return 0;
}