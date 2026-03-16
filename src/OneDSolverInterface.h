// SPDX-FileCopyrightText: Copyright (c) Stanford University, The Regents of the
// University of California, and others. SPDX-License-Identifier: BSD-3-Clause

#ifndef ONEDSOLVER_INTERFACE_H
#define ONEDSOLVER_INTERFACE_H

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

/**
 * @brief Interface wrapper for loading and calling 1D solver shared library
 */
class OneDSolverInterface {
 public:
  OneDSolverInterface();
  ~OneDSolverInterface();

  // Load the shared library
  void load_library(const std::string& interface_lib);

  // Initialize 1D solver
  void initialize(const std::string& input_file, int& problem_id,  int& systemSize,
                  const std::string& coupling_type);

  // Set external time step
  void set_external_step_size(double external_step_size);

  // Update boundary conditions
  void update_coupled_bc(int num_surfaces, double* input_values,
                        const std::string& coupling_type);

  // Get solution at coupled surfaces
  void get_coupled_solution(int num_surfaces, double* flows_out,
                           double* pressures_out);

  // Run one time step
  void run_1d_simulation_step(double dt, int& error_code);

  // Get resistance matrix
  void get_resistance_matrix(int num_surfaces, double** resistance_matrix);

  // Cleanup
  void cleanup();

  // Member data
  void* library_handle_ = nullptr;
  int problem_id_ = 0;
  int system_size_ = 0;

  // Function pointers to shared library functions
  void (*initialize_1d_)(const char*, int&, int&, const char*) = nullptr;
  void (*set_external_step_size_1d_)(int, double) = nullptr;
  void (*update_coupled_bc_1d_)(int, int, double*, const char*) = nullptr;
  void (*get_coupled_solution_1d_)(int, int, double*, double*) = nullptr;
  void (*run_1d_simulation_step_1d_)(int, double, int&) = nullptr;
  void (*get_resistance_matrix_1d_)(int, int, double**) = nullptr;
  void (*cleanup_1d_)(int) = nullptr;
};

#endif