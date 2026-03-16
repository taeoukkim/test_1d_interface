// SPDX-FileCopyrightText: Copyright (c) Stanford University, The Regents of the
// University of California, and others. SPDX-License-Identifier: BSD-3-Clause

#include "OneDSolverInterface.h"

#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <map>


using std::cout;
using std::cerr;
using std::endl;

int model_id;

static std::map<int,OneDSolverInterface*> interfaces;

int main(int argc, char* argv[]) {
  cout << "\n" << std::string(70, '=') << endl;
  cout << "  1D Solver Interface Loader Test (Dynamic Library Loading)" << endl;
  cout << std::string(70, '=') << endl;

  try { // error가 이 안에서 나면, 바로 catch로 넘어감
    
    std::string library_path = "/Users/taeoukkim/Desktop/Simvascular_solvers/1Dsolver/svOneDSolver/build/lib/libsvoned_interface.dylib";
    std::string input_file = "/Users/taeoukkim/Desktop/Simvascular_solvers/code_test/test_1d_interface/src/solver_1d.in";

    // 여기부터 create_svZeroD_model 함수 역할
    // input: library_path, file_name
    
    auto interface = new OneDSolverInterface(); // OneDSolverInterface interface;
    
    // ===== TEST 1: Load shared library =====
    cout << "\n[TEST 1] Loading 1D solver shared library..." << endl;
    interface->load_library(library_path);
    cout << "✓ Shared library loaded successfully!" << endl;

    // ===== TEST 2: Initialize 1D solver =====
    cout << "\n[TEST 2] Initializing 1D solver..." << endl;
    
    int problem_id = 0;
    int system_size = 0;
    std::string cpltype = "DIR";

    interface->initialize(input_file, problem_id, system_size, cpltype);
    cout << "✓ Problem ID: " << problem_id << endl;
    cout << "✓ System Size: " << system_size << endl;
    model_id = problem_id;
    interfaces[model_id] = interface; // store initialized interface in map for 나중 이용. 이 파일안에선 언제든 이용 가능 (static이니까)
    cout << "✓ Initialization successful!" << endl;


    // ===== TEST 3: Run simulation =====
    // make a time loop here.
    









    // // ===== TEST 3: Set external time step =====
    // cout << "\n[TEST 3] Setting external time step size..." << endl;
    
    // double external_dt = 0.001;
    // interface->set_external_step_size(external_dt);
    
    // cout << "✓ External time step set to: " << external_dt << " s" << endl;

    // // ===== TEST 4: Run simulation steps =====
    // cout << "\n[TEST 4] Running coupling simulation steps..." << endl;
    
    // int num_steps = 5;
    // double dt = 0.001;
    // int num_coupled_surfaces = 2;
    // std::vector<double> flows_at_surfaces(num_coupled_surfaces);
    // std::vector<double> pressures_at_surfaces(num_coupled_surfaces);
    
    // for (int step = 0; step < num_steps; step++) {
    //   cout << "\n--- Step " << step << " ---" << endl;
      
    //   double time = step * dt;
    //   std::vector<double> bc_values(num_coupled_surfaces);
      
    //   for (int i = 0; i < num_coupled_surfaces; i++) {
    //     if (coupling_types_array[i] == "DIR") {
    //       bc_values[i] = 100.0 + 10.0 * std::sin(2.0 * M_PI * time);
    //     } else {
    //       bc_values[i] = 50.0 + 10.0 * std::cos(2.0 * M_PI * time);
    //     }
    //   }
      
    //   cout << "  Updating boundary conditions..." << endl;
    //   interface->update_coupled_bc(num_coupled_surfaces, bc_values.data(),
    //                              coupling_types[0]);
      
    //   int error_code = 0;
    //   cout << "  Running 1D solver step (dt = " << dt << " s)..." << endl;
    //   interface->run_1d_simulation_step(dt, error_code);
      
    //   if (error_code != 0) {
    //     cerr << "  ERROR: 1D solver failed!" << endl;
    //     break;
    //   }
      
    //   cout << "  Retrieving solution..." << endl;
    //   interface->get_coupled_solution(num_coupled_surfaces, flows_at_surfaces.data(),
    //                                 pressures_at_surfaces.data());
      
    //   cout << "  Results:" << endl;
    //   cout << std::fixed << std::setprecision(4);
    //   for (int i = 0; i < num_coupled_surfaces; i++) {
    //     cout << "    Surface " << i << ": Q = " << flows_at_surfaces[i] 
    //          << " mL/s, P = " << pressures_at_surfaces[i] << " mmHg" << endl;
    //   }
    // }
    
    // cout << "\n✓ Simulation completed successfully!" << endl;

    // // ===== TEST 5: Get resistance matrix =====
    // cout << "\n[TEST 5] Computing resistance matrix..." << endl;
    
    // std::vector<std::vector<double>> resistance_matrix(
    //     num_coupled_surfaces, std::vector<double>(num_coupled_surfaces, 0.0));
    
    // std::vector<double*> resistance_ptrs(num_coupled_surfaces);
    // for (int i = 0; i < num_coupled_surfaces; i++) {
    //   resistance_ptrs[i] = resistance_matrix[i].data();
    // }
    
    // interface->get_resistance_matrix(num_coupled_surfaces, resistance_ptrs.data());
    
    // cout << "  Resistance matrix (dP/dQ):" << endl;
    // for (int i = 0; i < num_coupled_surfaces; i++) {
    //   cout << "    ";
    //   for (int j = 0; j < num_coupled_surfaces; j++) {
    //     cout << std::setw(12) << resistance_matrix[i][j];
    //   }
    //   cout << endl;
    // }
    
    // cout << "✓ Resistance matrix computed!" << endl;

    // // ===== TEST 6: Cleanup =====
    // cout << "\n[TEST 6] Cleaning up..." << endl;
    // interface->cleanup();
    // delete interface;
    // cout << "✓ Cleanup completed!" << endl;

    // cout << "\n" << std::string(70, '=') << endl;
    // cout << "  All tests passed!" << endl;
    // cout << std::string(70, '=') << "\n" << endl;

    return 0;

  } catch (const std::exception& e) {
    cerr << "\nERROR: " << e.what() << endl;
    return 1;
  }
}