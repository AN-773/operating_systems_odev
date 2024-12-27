# **Simple Linux Shell Application**

## **Overview**

This project is a simple Linux shell implementation written in C. The shell provides basic command-line functionality, allowing users to execute commands, chain commands with pipes, redirect input/output, run commands in the background, and navigate directories.

### **Features**

1. **Execute Basic Commands**: Run standard Linux commands like `ls`, `pwd`, etc.  
2. **Piping**: Chain multiple commands using `|`.  
3. **Input/Output Redirection**: Use `<` for input redirection and `>` for output redirection.  
4. **Background Execution**: Run commands in the background with `&`.  
5. **Directory Navigation**: Change directories using the `cd` command.  
6. **Quit Command**: Exit the shell with the `quit` command.

## **Requirements**

* GCC (GNU Compiler Collection)  
* Linux-based operating system (or any system supporting POSIX APIs)

## **Installation**

Clone the repository or copy the source files into a directory:

 git clone \<repository-url\>  
cd \<repository-directory\>

1. Compile the code using the provided Makefile:

 make

2.  This generates an executable named `shell`.

## **Usage**

Start the shell:

 ./shell

1. Use the following commands within the shell:

   * **Run Commands**: Enter standard Linux commands, e.g., `ls -l`.  
   * **Piping**: Chain commands, e.g., `ls | grep .c`.  
   * **Input/Output Redirection**:  
     * Redirect input: `cat < input.txt`  
     * Redirect output: `ls > output.txt`  
   * **Background Execution**: Run commands in the background with `&`, e.g., `sleep 10 &`.  
   * **Change Directory**: Use `cd <directory>`. Enter `cd` without arguments to return to the home directory.  
   * **Quit**: Type `quit` to exit the shell.

## **File Structure**

* `shell.c`: Implementation of the shell.  
* `shell.h`: Header file containing function declarations and global variables.  
* `Makefile`: Automates the build process.

## **Features in Detail**

### **Input/Output Redirection**

Redirect standard input and output to/from files:

Input Redirection (`<`):  
 cat \< file.txt

*  Reads input from `file.txt`.

Output Redirection (`>`):  
 ls \> output.txt

*  Writes output to `output.txt`.

### **Redirect input from a file and output to another file:**
 sort \< input.txt \> output.txt

### 

### **Background Execution**

Run commands in the background using `&`:

sleep 10 &

The shell will display the process ID (PID) of the background task.

### **Piping**

Chain multiple commands using `|`:

ls | grep .c

This will pass the output of `ls` as input to `grep`.

### **Directory Navigation**

Change the current working directory using the `cd` command:

cd /path/to/directory

Use `cd` without arguments to return to the home directory.

### **Custom commands**
quit to close the shell

## **Known Limitations**

1. **Pipe Handling**: The implementation for pipes is partially completed and needs refinement.  
2. **Error Handling**: Limited error handling for complex command chaining.  
3. **Maximum Limits**: The shell enforces limits for:  
   * Command length (1024 characters)  
   * Number of arguments (100)  
   * Background processes (100)

## **Future Improvements**

* Complete and refine the pipe implementation.  
* Add support for environment variable expansion (e.g., `$HOME`).  
* Enhance error handling for edge cases.  
* Add support for command history.

## **Cleaning Up**

To remove the compiled files:

make clean

## **Created by**
* Abdalrahman Nasr
* BERKAY CAN
* FUAD AISHAN
* Shahd Numa Yaldanı
