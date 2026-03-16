// SPDX-FileCopyrightText: Copyright (c) Stanford University, The Regents of the
// University of California, and others. SPDX-License-Identifier: BSD-3-Clause

#include "OneDSolverInterface.h"
#include <iostream>
#include <cstring>

using std::cout;
using std::cerr;
using std::endl;



OneDSolverInterface::OneDSolverInterface() {}

OneDSolverInterface::~OneDSolverInterface() {
  if (library_handle_) {
    dlclose(library_handle_);
  }
}

void OneDSolverInterface::load_library(const std::string& interface_lib) {
  cout << "[OneDSolverInterface::load_library] Loading: " << interface_lib << endl;
  
  library_handle_ = dlopen(interface_lib.c_str(), RTLD_LAZY);
  
  if (!library_handle_) {
    throw std::runtime_error(std::string("Error loading shared library '") + interface_lib +
                           "' with error: " + dlerror());
  }

  // Load initialize_1d function
  *(void**)(&initialize_1d_) = dlsym(library_handle_, "initialize_1d");
  if (!initialize_1d_) {
    throw std::runtime_error(std::string("Error loading function 'initialize_1d': ") + dlerror());
  }

  // Load set_external_step_size_1d function
  *(void**)(&set_external_step_size_1d_) = dlsym(library_handle_, "set_external_step_size_1d");
  if (!set_external_step_size_1d_) {
    throw std::runtime_error(
        std::string("Error loading function 'set_external_step_size_1d': ") + dlerror());
  }

  // Load update_coupled_bc_1d function
  *(void**)(&update_coupled_bc_1d_) = dlsym(library_handle_, "update_coupled_bc_1d");
  if (!update_coupled_bc_1d_) {
    throw std::runtime_error(std::string("Error loading function 'update_coupled_bc_1d': ") +
                           dlerror());
  }

  // Load get_coupled_solution_1d function
  *(void**)(&get_coupled_solution_1d_) = dlsym(library_handle_, "get_coupled_solution_1d");
  if (!get_coupled_solution_1d_) {
    throw std::runtime_error(std::string("Error loading function 'get_coupled_solution_1d': ") +
                           dlerror());
  }

  // Load run_1d_simulation_step_1d function
  *(void**)(&run_1d_simulation_step_1d_) = dlsym(library_handle_, "run_1d_simulation_step_1d");
  if (!run_1d_simulation_step_1d_) {
    throw std::runtime_error(
        std::string("Error loading function 'run_1d_simulation_step_1d': ") + dlerror());
  }

  // Load get_resistance_matrix_1d function
  *(void**)(&get_resistance_matrix_1d_) = dlsym(library_handle_, "get_resistance_matrix_1d");
  if (!get_resistance_matrix_1d_) {
    throw std::runtime_error(std::string("Error loading function 'get_resistance_matrix_1d': ") +
                           dlerror());
  }

  // Load cleanup_1d function
  *(void**)(&cleanup_1d_) = dlsym(library_handle_, "cleanup_1d");
  if (!cleanup_1d_) {
    throw std::runtime_error(std::string("Error loading function 'cleanup_1d': ") + dlerror());
  }

  cout << "[OneDSolverInterface::load_library] All functions loaded successfully" << endl;
}

void OneDSolverInterface::initialize(const std::string& input_file, int& problem_id_,  int& systemSize,
                                    const std::string& coupling_type) {
  if (!initialize_1d_) {
    throw std::runtime_error("initialize_1d not loaded");
  }

  // Call initialize
  initialize_1d_(input_file.c_str(), problem_id_, systemSize, (char*)coupling_type.c_str());


  cout << "[OneDSolverInterface::initialize] problem_id: " << problem_id_ << endl;
}

void OneDSolverInterface::set_external_step_size(double external_step_size) {
  if (!set_external_step_size_1d_) {
    throw std::runtime_error("set_external_step_size_1d not loaded");
  }
  set_external_step_size_1d_(problem_id_, external_step_size);
}

void OneDSolverInterface::update_coupled_bc(int num_surfaces, double* input_values,
                                           const std::string& coupling_type) {
  if (!update_coupled_bc_1d_) {
    throw std::runtime_error("update_coupled_bc_1d not loaded");
  }
  update_coupled_bc_1d_(problem_id_, num_surfaces, input_values, coupling_type.c_str());
}

void OneDSolverInterface::get_coupled_solution(int num_surfaces, double* flows_out,
                                             double* pressures_out) {
  if (!get_coupled_solution_1d_) {
    throw std::runtime_error("get_coupled_solution_1d not loaded");
  }
  get_coupled_solution_1d_(problem_id_, num_surfaces, flows_out, pressures_out);
}

void OneDSolverInterface::run_1d_simulation_step(double dt, int& error_code) {
  if (!run_1d_simulation_step_1d_) {
    throw std::runtime_error("run_1d_simulation_step_1d not loaded");
  }
  run_1d_simulation_step_1d_(problem_id_, dt, error_code);
}

void OneDSolverInterface::get_resistance_matrix(int num_surfaces, double** resistance_matrix) {
  if (!get_resistance_matrix_1d_) {
    throw std::runtime_error("get_resistance_matrix_1d not loaded");
  }
  get_resistance_matrix_1d_(problem_id_, num_surfaces, resistance_matrix);
}

void OneDSolverInterface::cleanup() {
  if (!cleanup_1d_) {
    throw std::runtime_error("cleanup_1d not loaded");
  }
  cleanup_1d_(problem_id_);
  cout << "[OneDSolverInterface::cleanup] Cleanup completed" << endl;
}