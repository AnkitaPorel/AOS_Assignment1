Program 1- 2024201043_A1_Q1.cpp

Instructions to execute-

To execute this program, you need to provide proper command line inputs, program will terminate for invalid inputs. The program will create a directory named "Assignment1". Program will terminate if the directory already exists in the same path as the program file.

Permissible input format in command line- ./output_executable_file_name input_file_path 0 
or, for input flag=1-> ./output_executable_file_name input_file_path 1 3 100

Description-

The program uses system calls, such as- lseek(),open(),read(),write() etc. The program takes an input file, and creates a directory named "Assignment1", creates an output file in this directory, reverses the content of the input file, saves it in the new file created. Since the input size can be large, it is broken up in chunks of size 4096 Bytes. Program uses 3 user defined functions other than the main() function.
(1) print_progress() function prints the progress on console, it takes two arguments to calculate the percentage of input file reversed in the new file.
(2) reverse_full() function reverses the input file in full, for input flag=0.
(3) reverse_part() function reverses the input file in part, for input flag=1.

Program 2- 2024201043_A1_Q2.cpp

Instructions to execute-

To execute this program, you need to provide proper command line inputs, program will terminate for invalid inputs. Since the new file created in previous program is inside the directory Assignment1, so you need provide the new_file path as Assignment1/0_input_file_name. 

Permissible input format in command line- ./output_executable_file_name path/Assignment1/0_input_file_name input_file_path path/Assignment1

Description-

The program uses system calls, such as- lseek(),open(),read()etc. This program checks whether the old file, new file and the directory exists, and checks whether the new file is reverse of old file. It also prints user, group and others read, write and execute permissions for the old file, new file, and the directory. Program uses 2 user defined functions other than the main() function.
checkPermission() checks the above mentioned permissions.
isReverse() checks whether the new file is the reversed version of the old file. To check this, again files are broken up in chunks of size 4096 Bytes, since file size can be large.
