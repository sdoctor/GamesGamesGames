Sara Drakeley
Assignment 3
*******************************************************************************
To compile and run the code, uncompress the zip file into a directory, and type 

$ make 

Then, to run the code, you can pass the following parameters:

// To run the code with the default settings being Cloth system, Trapezoid Method
//   and stepsize of .02, simply enter:
$ a3 

// Optional parameters:
$ a3 integration_method step_size system_type

// integration method: choose either "e" for euler, or "t" for trapezoid
// step_size: .02 seems to work best for cloth or pendulum. Slightly smaller 
is better for chain.

// system_type: There are 4 system types to choose from: "simple" "pendulum" 
"chain" and "cloth". Try them all! :D

// * Note * you have to specify all previous parameters to specify the system 

Then, to play with the code: 
press "s" and the cloth will shake around
press "w" and the particles will disappear just showing the wireframe
press "r" to refresh
press "b" and see the particles blown around by a random breeze :)



******************************************************************************
I collaborated with Katie Kaufman mostly, also Connie Chan, Shari Hynes, and
Brodrick Childs, as well as office hours. Most of the collaboration involved
debugging and tracking down C++ errors, as well as giving tips about and then 
discussing the various design approaches I used.

******************************************************************************

There are no known problems with my code :D

******************************************************************************

Extra Credit:
Wind:
Press 'b' to toggle breeze! Implemented in evalF to add a breeze force

Rendering the surface:
I started to implement the surface rendering, and it's commented out as a 
draw method for Cloth in clothSystem.cpp. 


******************************************************************************
Comments: 
Really enjoyed this assignment and can see myself playing around with it 
in the future! This assignment also forced me to learn a lot about C++, for 
which I am grateful.

