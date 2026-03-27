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
  void set_external_step_size(int& problem_id_, double external_step_size);

  // 
  void return_solution(int& problem_id_, double* solution_1d, int solution_size);

  //
  void update_solution(int& problem_id_, double* previous_solution_data, int solution_size);

  // Run one time step
  void run_1d_simulation_step(int problem_id, double current_time, int save_time, const std::string& coupling_type, double* params,
                                          double* solution_vector,double& cplBCvalue,int& error_code);
  
  // Extract coupled DOF information
  void extract_coupled_dof(int problem_id, int& coupled_dof, const std::string& coupling_types);

  // Member data
  void* library_handle_ = nullptr;
  int problem_id_ = 0;
  int system_size_ = 0;

  // Function pointers to shared library functions
  void (*initialize_1d_)(const char*, int&, int&, const char*) = nullptr;
  void (*set_external_step_size_1d_)(int, double) = nullptr;
  void (*return_1d_solution_)(int, double*, int) = nullptr;
  void (*update_1d_solution_)(int, double*, int) = nullptr;
  void (*run_1d_simulation_step_1d_)(int, double, int, const char*, double*, double*, double&, int&) = nullptr;
  void (*extract_coupled_dof_)(int, int&, char*) = nullptr;
};

#endif