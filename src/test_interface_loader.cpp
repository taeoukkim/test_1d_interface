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
    std::string input_file = "/Users/taeoukkim/Desktop/Simvascular_solvers/code_test/test_1d_interface/src/solver_1d_neu.in";
    //std::string input_file = "/Users/taeoukkim/Desktop/Simvascular_solvers/code_test/test_1d_interface/src/0129.in";

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
    std::string cpltype = "NEU"; // "NEU" or "DIR"
    double coupled_value =0.0;

    interface->initialize(input_file, problem_id, system_size, cpltype);
    cout << "✓ Problem ID: " << problem_id << endl;
    cout << "✓ System Size: " << system_size << endl;
    model_id = problem_id;
    interfaces[model_id] = interface; // store initialized interface in map for 나중 이용. 이 파일안에선 언제든 이용 가능 (static이니까)
    cout << "✓ Initialization successful!" << endl;


    // ===== TEST 3: Run simulation =====
    // ===== TEST 3: Run simulation with time loop =====
    cout << "\n[TEST 3] Running time-stepping simulation..." << endl;
    
    // Time stepping parameters
    double dt = 0.005;  // Time step size (seconds)
    int max_steps = 10;  // Maximum number of time steps
    int save_time = 1; // Save every N steps (not used in this test, but can be implemented in the future)
    double current_time = 0.0;

    // ===== Set time step size =====
    interface->set_external_step_size(problem_id, dt);
    
    cout << "  Time step size (dt): " << dt << " s" << endl;
    cout << "  Maximum steps: " << max_steps << endl;
    cout << "  System size (num_nodes * 2): " << system_size << endl;

    // get coupled DOF information
    int coupled_dof = 0;
    interface->extract_coupled_dof(problem_id, coupled_dof, cpltype);

    
    std::vector<double> solution_vector(system_size);
    // copy 1D solution to 3D solver solution vector (initial condition)
    interface->return_solution(problem_id, solution_vector.data(), system_size);

    // TODO: 지금 여기서 initial condition을 solution_vector에 coupled_dof에만 적용하는 과정이 있음
    // flow 값의 경우 간단한데, area가 문제. 왜냐면 initial값은 pressure로 주어지니까
    // 그런데 여기서 initialize가 꼭 필요한가??????


    

    cout << "\n  Starting time loop..." << endl;
    cout << std::string(70, '-') << endl;
    std::vector<double> params(5);
    params[0] = 2.0;
    
    for (int step = 0; step < max_steps; step++) {
      cout << "\n  === Time Step " << step << " ===" << endl;
    
      if (cpltype == "DIR"){
        params[1] = current_time;
        params[2] = current_time+dt;
        params[3] = 10000.0;
        params[4] = 10000.0;
      }else if(cpltype == "NEU"){
        params[1] = current_time;
        params[2] = current_time+dt;
        params[3] = 100.0;
        params[4] = 100.0;
      }

      // update 1D solution with previous time step's solution(using solution stored at 3D solver)
      interface->update_solution(problem_id, solution_vector.data(), system_size);
      // Call the 1D simulation step
      int error_code = 0;
      cout << "    Running 1D solver step..." << endl;
      interface->run_1d_simulation_step(problem_id, current_time, save_time, cpltype, params.data(),
                                        solution_vector.data(), coupled_value, error_code);
      
      if (error_code != 0) {
        cerr << "    ERROR: 1D solver step failed with error code: " 
             << error_code << endl;
        break;
      }

      cout <<"    Coupled DOF: " << coupled_dof << endl;
      cout <<"    Coupled BC value from 1D solver: " << coupled_value << endl;
      
      // Update current time for next step
      current_time += dt;
    }


    // // print final solution
    // for (int i = 0; i < system_size; i++) {
    //   if (i % 2 == 0) {
    //     cout << "\n[" << i/2 << "] Flow: " << solution_vector[i];
    //   } else {
    //     cout << " | Pressure: " << solution_vector[i];
    //   }
    // }

    
    cout << "\n" << std::string(70, '-') << endl;
    cout << "\n✓ Time-stepping simulation completed!" << endl;
    cout << "  Final simulation time: " << std::fixed << std::setprecision(6) 
         << current_time << " s" << endl;
    
    // Optional: Cleanup
    // cout << "\n[TEST 4] Cleaning up..." << endl;
    // interface->cleanup();
    // delete interface;
    // cout << "✓ Cleanup completed!" << endl;

    cout << "\n" << std::string(70, '=') << endl;
    cout << "  Test completed successfully!" << endl;
    cout << std::string(70, '=') << "\n" << endl;

    return 0;










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