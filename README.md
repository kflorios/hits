# hits
HITS: Hyperplanes Intersection Tabu Search for maximum score estimation

# Description

Hyperplanes Intersection Tabu Search (HITS) is a program that calculates the Maximum Score Estimator (MS) of the binary choice model (Manski, 1975; 1985). It is a variant of Tabu Search (TS) method (Glover, 1989) especially modified for MS. Specifically, it uses the gaussian elimination to locate the trial points of hyperplanes intersections (Pinkse, 1993). It uses the concept of “Neighbourhood” which is popular in trajectory-based methods of combinatorial optimization in order to search nearby solutions of the current solution and avoid complete enumeration. It also uses a “Tabu List” as a short-term memory of prohibited neighbouring solutions to which the search is prevented from moving to. The program is coded in Microsoft Visual Studio 2017 with Visual C++.  

# Input 

The basic input of the HITS program is 4 text files that contain the required information. Assume that we have the following dataset in a file called **X.txt**:   
1	11.077	2	0	6	4.1	16.81	1  
2	11.835	2	0	6	4.4	19.36	1  
3	11.225	2	0	4	4.6	21.16	1  
4	11.835	2	0	6	4.4	19.36	1  
5	11.835	2	0	6	4.4	19.36	1  
…  
2335	11.835	2	0	6	5.6	31.36	1  
2336	11.352	2	0	4	4.7	22.09	1  
2337	10.510	0	0	6	3.0	9.00	1  
2338	11.222	0	0	17	5.4	29.16	1  
2339	10.964	0	0	17	3.9	15.21	1  

Also, assume the response variable saved in file **y.txt**:  
1       -1  
2       -1  
3       -1  
4       -1  
5       -1  
…  
2335    1  
2336    -1  
2337    1  
2338    1  
2339    1  

Then, the auxiliary text files to complete the estimation problem formulation have the following form: 
First the **names.txt** file looks like:  

OBS HW CHILD YCHILD EDU AGE AGE2 INTCPT  

Second the **params.txt** file looks like:  

n = 2339  
p = 7  
b0 = -1  
Nreps = 10  
d = 1e300  

# PARAMETERS

The required parameters for HITS program are five: 

1.	**“n”:** the number of rows in the loaded dataset or the number of observations in the regression.  
2.	**“p”:** the number of columns in the loaded dataset or the number of independent variables in the regression.  
3.	**“b0”:** is set to either 1 or -1 and is the coefficient of the first independent variable. This is done for identification and because the MS is identified up-to-scale. Multiplying the regression coefficients by any positive constant number provides the same objective function value. Prior knowledge can be used to select 1 or -1, or in general both values can be tried, and the regression will be run twice.
4.	**“Nreps”:** the number of randomized independent runs of the tabu search algorithm. This is done to avoid getting trapped in local maxima using a single run.
5.	**“d”:** the upper bound for the absolute value of each regression coefficient. It can be set to an arbitrary large value to allow unconstrained maximization. Alternatively, it can be set to a stricter value (say 1e5) to put bound constraints on the regression coefficients. The parameter d applied to each regression coefficient.

# Running 

During the run the user can see:  

1.	The progress bar filling in (proportionally to: run i over Nreps, the total number of runs).
2.	The log file: DemoExample_Cpp_tabusearch_opt_log_iter200.txt with the trajectory of the search.
3.	The info file: DemoExample_Cpp_tabusearch_opt_info_iter200 .txt with advanced debug information.

# Output 

The basic output of HITS is a text file with name DemoExample_Cpp_tabusearch_opt_log_iter200.txt that contains all the information:  

The software at the time of inspection of results presents a screen with the progress bar filled in 100%:  

Then, the user can complete the analysis hitting CLOSE button and inspect the solution output files already discussed above.  

# Acknowledgements 

I would like to thank Dr. **Alexandros Louka** and Professor **Yannis Bilias** for their valuable help and the shared ideas for the development of HITS. The development of the HITS software was supported by the Hellenic Foundation for Research and Innovation (H.F.R.I.) under the '2nd Call for H.F.R.I. Research Projects to support Post-Doctoral Researchers' (Project Number: 902). 


