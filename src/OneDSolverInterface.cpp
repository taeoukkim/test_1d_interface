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

  *(void**)(&return_1d_solution_) = dlsym(library_handle_, "return_1d_solution");
  if (!return_1d_solution_) {
    throw std::runtime_error(
        std::string("Error loading function 'return_1d_solution': ") + dlerror());
  }

  *(void**)(&update_1d_solution_) = dlsym(library_handle_, "update_1d_solution");
  if (!update_1d_solution_) {
    throw std::runtime_error(
        std::string("Error loading function 'update_1d_solution': ") + dlerror());
  }

  // Load run_1d_simulation_step_1d function
  *(void**)(&run_1d_simulation_step_1d_) = dlsym(library_handle_, "run_1d_simulation_step_1d");
  if (!run_1d_simulation_step_1d_) {
    throw std::runtime_error(
        std::string("Error loading function 'run_1d_simulation_step_1d': ") + dlerror());
  }

  *(void**)(&extract_coupled_dof_) = dlsym(library_handle_, "extract_coupled_dof");
  if (!extract_coupled_dof_) {
    throw std::runtime_error(
        std::string("Error loading function 'extract_coupled_dof': ") + dlerror());
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

void OneDSolverInterface::set_external_step_size(int& problem_id_, double external_step_size) {
  if (!set_external_step_size_1d_) {
    throw std::runtime_error("set_external_step_size_1d not loaded");
  }
  set_external_step_size_1d_(problem_id_, external_step_size);
}

void OneDSolverInterface::return_solution(int& problem_id_, double* solution_1d, int solution_size) {
  if (!return_1d_solution_) {
    throw std::runtime_error("return_1d_solution not loaded");
  }
  return_1d_solution_(problem_id_, solution_1d, solution_size);
}

void OneDSolverInterface::update_solution(int& problem_id_, double* previous_solution_data, int solution_size) {
  if (!update_1d_solution_) {
    throw std::runtime_error("update_1d_solution not loaded");
  }
  update_1d_solution_(problem_id_, previous_solution_data, solution_size);
}


void OneDSolverInterface::run_1d_simulation_step(int problem_id_, double current_time, int save_time, const std::string& coupling_type, double* params, double* solution_vector, double& cplBCvalue, int& error_code) {
  if (!run_1d_simulation_step_1d_) {
    throw std::runtime_error("run_1d_simulation_step_1d not loaded");
  }
  run_1d_simulation_step_1d_(problem_id_, current_time, save_time, (char*)coupling_type.c_str(), params, solution_vector, cplBCvalue, error_code);
}

void OneDSolverInterface::extract_coupled_dof(int problem_id, int& coupled_dof, const std::string& coupling_types) {
  if (!extract_coupled_dof_) {
    throw std::runtime_error("extract_coupled_dof not loaded");
  }
  extract_coupled_dof_(problem_id, coupled_dof, (char*)coupling_types.c_str());
}