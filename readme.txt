#Excersise 5 - Desigen Patterns:
##Programmers: Assaf Shmaryahu & Lior Jerbi.

This Ex is about "ActiveObject" DesigenPattern funcionality and logic.
The Program is coded in C++ and compiled with clang14.

Instructions:
*Extract the compressed zip file that holds the program files.(st_pipeline.hpp,st_pipeline.cpp,makefile)
*After extracting the files, you can open a terminal window and write the command "make all"/"make".
*Now we have the exe file of the program named "st_pipeline" in our folder.

Running instructions:
*The usage of the program should be like this:

--> ./st_pipeline <Number of tasks> (optional)<Seed>

*Number of tasks - this argument will tell the program how much tasks it should generate and pass between the AO's.
*Seed - represent an int number that will implement the random number for each task.(Optional argument, if empty Seed=time(NULL))

each task represent a number with 6 digits.
in our code we have 4 ActiveObjects:

ActiveObject1:genarate N random numbers,and pass them one by one to the next ActiveObject.

ActiveObject2:print the number and then check if the number is prime(print: true/false) and add 11 to the num and move it to the next ActiveObject.

ActiveObject3:print the number and then check if the number is prime(print: true/false) and sub 13 to the num and move it to the next ActiveObject.

ActiveObject4:print the number and then add 2 to it,then print again(the last num that was print need to be like the first num we print in ActiveObject2).
	
Note that you have an example of runnig in the zipFile.

