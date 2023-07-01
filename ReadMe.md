

This is a repository for a custom shell implementation called "Average Shell" . The shell is programmed in C and aims to resemble the functionality of a bash shell. Here is some information about the terminal commands, functionality, and files in the repository:

### How to run?:
To run and use this shell download zip and unzip and write the following commands in terminal: 
 ``` sh
  make 
 ./a.out 
 ```

### Functionality:
- Upon entering the terminal, the screen is cleared, and the shell is launched.
- The shell is invoked from the home folder (represented by ~).
- Input commands are executed based on the current directory and the command itself.
- To indicate the end of input, press Enter after entering a command.
- Typing "exit" will exit the shell.
- The shell supports directory switching and command execution.

### File Information:
- `main.c`: This file contains basic input handling, printing the current directory, command functions, input redirection, pipelines, signal handling, and auto-complete functionality.
- `cd.c`: This file contains functions for the `cd`, `pwd`, `clear`, and `echo` commands.
- `ls.c`: This file contains the function for the `ls` command.
- `fgbg.c`: This file contains functions for the `fg` and `bg` process commands.
- `pinfo.c`: This file contains the function for the `pinfo` command, which prints process-related information.
- `discover.c`: This file contains the function for the `discover` command, which finds and shows directories and files that are not hidden.
- `history.c`: This file contains the function for the `history` command.

### About:
- Running `"cd -"` as the first command when the shell is launched will give the current working directory.
- `";"` cannot be the starting word of an input command.
- `"&"` functionality is not implemented.
- Empty commands just display the prompt again with the current working directory and user/system information.
- Errors are handled using `perror`, and the program continues to show the prompt after displaying the error message.
- In the `ls` command, files are sorted alphabetically, ignoring cases. Files starting with `.` are also sorted by ignoring the `.`.
- Duplicate flags in the `ls` command are not allowed.
- The `discover` command only finds and shows non-hidden directories and files.
- The `discover` command only accepts one file name and one directory name.
- Only the `-d` and `-f` flags are acceptable in the `directory` command.
- The `discover` command searches for directory and file names specified as arguments.
- In the `history` command, if a command contains `;`, it is printed as two separate commands in the history.
- The repository includes two PDF files, `1.pdf` and `2.pdf`, contains the commands required to run the shell and has a description of how the command works.

This shell was made as requirement for assignment 2 and assignment 3 of a course named OSN (Operating System Network) in IIIT-H
