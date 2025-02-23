[![progress-banner](https://backend.codecrafters.io/progress/shell/f49eb20a-56dd-4ead-9e7d-793914f77ba6)](https://app.codecrafters.io/users/codecrafters-bot?r=2qF)

POSIX compliant shell that's capable of interpreting shell commands, running external programs and builtin commands like
cd, pwd, echo and more.

## Features  
✅ **Built-in commands:**  
- `cd` – Change directory  
- `echo` – Print text to the terminal  
- `pwd` – Print the current working directory  
- `type` – Check if a command is built-in  
- `exit` – Exit the shell  

✅ **Command execution** (supports external programs)  
✅ **Tab autocompletion**  


## Installation & Setup  
```
git clone https://github.com/GabriellaGloria/shell-cpp.git
cd shell-cpp
```

## Build & Run : 
1. Ensure you have `cmake` installed locally
2. Run `./shell.sh` to run your program, which is implemented in
   `src/main.cpp`.

## Usage :
```
$ echo Hello, world!
Hello, world!

$ pwd
/home/user/shell-cpp

$ cd ..
$ pwd
/home/user
```

## Credits & References :
[CodeCrafters - "Build Your Own Shell"](https://app.codecrafters.io/courses/shell/overview).
