# hits
HITS: Hyperplanes Intersection Tabu Search for maximum score estimation

# Description

Hyperplanes Intersection Tabu Search (HITS) is a program that calculates the Maximum Score Estimator (MS) of the binary choice model (Manski, 1975; 1985). It is a variant of Tabu Search (TS) method (Glover, 1989) especially modified for MS. Specifically, it uses the gaussian elimination to locate the trial points of hyperplanes intersections (Pinkse, 1993). It uses the concept of “Neighbourhood” which is popular in trajectory-based methods of combinatorial optimization in order to search nearby solutions of the current solution and avoid complete enumeration. It also uses a “Tabu List” as a short-term memory of prohibited neighbouring solutions to which the search is prevented from moving to. The program is coded in Microsoft Visual Studio 2017 with Visual C++.  

# Input 

The basic input of the HITS program is 4 text files that contain the required information. Assume that we have the following dataset in a file called X.txt: 
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

Also, assume the response variable saved in file y.txt:
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
First the names.txt file looks like:

OBS HW CHILD YCHILD EDU AGE AGE2 INTCPT

Second the params.txt file looks like:

n = 2339
p = 7
b0 = -1
Nreps = 10
d = 1e300

