/*
Run dynamics analysis for NN learning BG prediction
test insulin levels and resultant change in reachability

-uses sherlock, gurobi

Taisa Kushner
taisa.kushner@colorado.edu

*/

#include "propagate_intervals.h"
#include <fstream>
#include <iostream>
using namespace std;
using namespace std::chrono;

int main(int argc, char ** argv)
{
	int bolus_min = -1;
	int bolus_max = -1;
	char key[] = "all";

	if(argc != 3)
	{
		cout << "Wrong number of command line arguments : " << endl;
		cout << "Please pass the bolus range to run. " << endl;
		cout << "Exiting... " << endl;
		exit(0);
	}

	else
	{
		sscanf(argv[1], "%d", &bolus_min);
		sscanf(argv[2], "%d", &bolus_max);
	}

	int num_insulin_inputs = 37;
	int num_other_inputs = 37;
	int bolus_location = 28;

	vector< vector< datatype > > input_interval(num_insulin_inputs+num_other_inputs, vector< datatype >(2,0));
	vector< vector< datatype > > input_constraints;
	 clock_t begin, end;
	 vector< datatype > output_range(2,0);
	 // Simple range propagation
	 // sherlock_parameters.verbosity = true;
	 // sherlock_parameters.grad_search_point_verbosity = true;
	 sherlock_parameters.time_verbosity = true;


		char nn_test_name[] = "./BGnetwork_files/PSO3-001-0001_high_APNN" ;
		network_handler benchmark_BG(nn_test_name);


		ifstream glucFile("./BGnetwork_files/PSO3-001-0001_TFinput_capGluc_7.csv");
		string gluc_val = "";
		int delta = 2;
		for (int i=0; i<num_other_inputs; i++){
			getline(glucFile,gluc_val);
			input_interval[i][0] = stoi(gluc_val)-delta; input_interval[i][1]= stoi(gluc_val)+delta;
		}
		glucFile.close();


		for (int i=num_other_inputs; i<num_insulin_inputs+num_other_inputs; i++){
			if (i == bolus_location+num_other_inputs){
				input_interval[i][0] = bolus_min; input_interval[i][1] = bolus_max;
			}
			else {
				input_interval[i][0] = 0; input_interval[i][1] = 0.05;
			}
		}


		//cout<<input_interval.size()<<" input constraint "<<input_constraints.size()<<endl;

		create_constraint_from_interval(input_constraints, input_interval);
		//cout<<input_constraints.size()<<endl;
		//cout<<input_interval[1][0]<<" "<<input_interval[1][1]<<endl;

		begin = clock();

		vector< vector< vector< datatype > > > weights;
		  vector< vector< datatype > > biases;
			benchmark_BG.return_network_information(weights, biases);

			vector < datatype> upper_bound_vector;
			vector < datatype> lower_bound_vector;
		//output_range[1] = do_MILP_optimization(input_constraints, weights, biases, upper_bound_vector, 1);
		//output_range[0] = do_MILP_optimization(input_constraints, weights, biases, lower_bound_vector, -1);
	 	benchmark_BG.return_interval_output(input_constraints, output_range, 1);
		cout << "output_range = [" << output_range[0] << " , " << output_range[1] << " ]" << endl;
		end = clock();
		printf("time cost for Sherlock BG test ------------------ %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
		// std::ofstream output_file("upperVector.txt");
		// for (int i=0;i<upper_bound_vector.size();i++){
		// 	output_file<<upper_bound_vector[i]<<endl;
		// }
		// output_file.close();
		//
		// std::ofstream output_file2("lowerVector.txt");
		// for (int i=0;i<lower_bound_vector.size();i++){
		// 	output_file2<<lower_bound_vector[i]<<endl;
		// }
		// output_file2.close();
		//
		// vector < vector < unsigned int > > active_weights;
		// cout<<compute_network_output(upper_bound_vector,weights,biases,active_weights)<<endl;

  return 0;
}
