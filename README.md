*This project was created as part of the 42 curriculum by jnovais and rida-cos*.

# Description

Shells have existed since the beginning of IT.

Back then, developers agreed that communicating with a computer via aligned input/output switches was extremely frustrating.

It was only natural that they came up with the idea of creating software to communicate with a computer using interactive command lines in a language somewhat close to human language.

**Minishell** is a simplified version of the original Bash that allows us to travel back in time and experience the challenges developers faced before Windows existed. It is a project where we were challenged to reproduce basic shell behaviors: reading commands from standard input, parsing them, and executing them correctly while handling child processes, redirections, and pipes.

### Overview

We divided the project into two major parts: **Parsing** and the **Executor**. The first is responsible for processing data from the user input until it is delivered in the appropriate format to be consumed by the Executor. The second is responsible for executing the structured commands, applying redirections and pipes, creating child processes when necessary, executing built-ins, and updating `g_exit_status` following Bash behavior.

### Parsing
Parsing was divided into 5 different parts: **Lexer**, **Expander**, **Splitter**, **Quote Removal**, and the **Parser** itself.

* **Lexer**: Transforms the command line into a list of "Tokens" (words, pipes, redirections).
* **Expander**: Replaces environment variables (such as `$USER` or `$?`) with their actual values, respecting quotes.
* **Splitter**: Divides tokens into logical units of execution.
* **Quote Removal**: Removes unnecessary quotes (' or ") after the content has been interpreted.
* **Parser**: Organizes everything into a command structure ready to be executed.

### Executor
The Executor receives a linked list of commands (`t_cmd`) already prepared (with `args`, `fd_in`, `fd_out`, and flags like `invalid`) and decides how to execute each command.

Generally, it covers:

1. **Single Command** (`execute_cmd`)
   - Validates the node (`invalid`, `args`, etc.).
   - Resolves the executable path via `find_cmd_path`.
   - Performs a `fork()` and, in the child process, executes:
     - `apply_redirections` to connect `fd_in`/`fd_out` to `STDIN_FILENO`/`STDOUT_FILENO` via `dup2`.
     - `execve`.
   - In the parent process, it calls `waitpid` and updates `g_exit_status`.
   - Error handling follows the standard:
     - **127**: Command not found (`command not found` / `ENOENT`).
     - **126**: Failed to execute (e.g., permission / `execve` failed for another reason).

2. **Pipeline** (`execute_pipeline`)
   - Iterates through the `t_cmd` list in a loop (using a context `t_exec_ctx` to maintain state: current command, `last_pid`, flags, `envp`).
   - For each command:
     - If the node is invalid, it closes and resets FDs (`close_and_reset_fds`) and continues.
     - If it is a built-in and **there is no pipeline** (only 1 command), it executes in the parent process (`execute_builtin`) to allow side effects (e.g., `cd`, `export`, `unset`) and then closes FDs.
     - Otherwise, it creates a child process (`create_child_process`). In the child:
       - Closes FDs that do not belong to the command (`close_other_fds`).
       - Applies redirections (`apply_redirections`).
       - Executes the built-in (if applicable) or calls `execve`.
   - After starting the processes, the parent:
     - Waits for the last command of the pipeline (`waitpid(last_pid, ...)`) and uses `handle_pipeline_status` to define the final `g_exit_status`.
     - Waits for the remaining children (`restore_signals_and_wait`).

3. **Signals (Ctrl+C / Ctrl+\\)**
   - During pipeline execution, the parent process ignores `SIGINT` and `SIGQUIT` so as not to “kill” the prompt.
   - Children use default signal behavior (e.g., `SIGQUIT` causes status **131**).
   - The final status is converted to the shell standard: `128 + signal`.
     - Example: `SIGINT` => 130, `SIGQUIT` => 131.
   - To maintain Bash's visible behavior, when the last process terminates via `SIGQUIT`, we print `Quit (core dumped)`.

4. **File Architecture (Quick Reference)**
   - `src/executor.c`: Main entry point (`execute_cmd`, `execute_pipeline`) and local wait/status helpers.
   - `src/executor_helpers.c`: Pipeline execution loop and flow control utilities.
   - `src/executor_utils.c`: Child execution utilities (redirections, `execve`, error handling).
   - `src/executor_pipeline.c`: Pipeline helpers (closing FDs of other commands, resetting FDs, final status).

# Instructions

This section describes how to prepare the environment, compile, and run Minishell.

### Pre-requisites
The project depends on the GNU Readline library. If you do not have it installed (common in Debian/Ubuntu-based Linux environments), run:
```bash
sudo apt-get install libreadline-dev
```

### Compilation

Use the included Makefile in the root of the repository to compile the executable:
```bash
make
```
* `make clean`: Removes object files (`*.o`).
* `make fclean`: Removes objects and the final executable.
* `make re`: Cleans and recompiles the entire project.

### Execution
After successful compilation, start the interactive shell:
```bash
./minishell
```
### Usage Restrictions

Per project requirements, Minishell does not accept arguments upon initialization.
* Correct: `./minishell`
* Incorrect: `./minishell script.sh` (The program will display an error message and exit).

# Resources

1. **[The Open Group Base Specifications Issue](https://pubs.opengroup.org/onlinepubs/9699919799/functions/contents.html)**: Information on various basic C functions.
2. **[CodeVault](https://www.youtube.com/@CodeVault)**: A YouTube channel that explains basic C functions and precautions.
3. **Man pages**: Constant consultation of `execve(2)`, `sigaction(2)`, and `dup2(2)`.
4. **Bash**: Used as the primary reference for expected behavior.

### How AI was used
In this project, Artificial Intelligence (Gemini 3 Flash and Claude Code) was integrated as a central tool for learning acceleration and technical decision support. The use of AI focused on four fundamental pillars:

* ***Agility in Learning and Debugging***: I used AI to anticipate and diagnose complex logic errors (such as variable expansion failures or signal interpretation issues) that would normally require more debugging time.
* ***Analysis of Trade-offs and Perspectives***: AI was consulted to offer multiple perspectives on the same functionality, allowing for the comparison of different architectural approaches (such as linked lists vs. arrays).
* ***Development Optimization***: By simulating different approaches, it was possible to save development time and focus on the solution that best fit the project requirements and 42 standards.
* ***Edge Case Validation***: In critical tasks such as Parsing and Signal Management, the AI served as a "peer reviewer" to validate if the implemented logic complied with the POSIX standard.
* ***Test Case Generation (QA)***: AI was used to generate complex combinations of commands and edge cases for comparative testing with the original Bash, ensuring the program's robustness.

# Challenges

## Linked List VS Binary Tree

### 1. Linked List

Since basic Minishell only requires linear pipes (cmd1 | cmd2 | cmd3), a list is perfectly capable of representing this sequence.

* **Advantages**:
    * **Simplicity**: Much easier to iterate through and free memory.
    * **Linearity**: Perfectly matches the execution loop where you fork and pass the pipe to the next command.
    * **Fewer Leaks**: Memory management in linked lists is less prone to errors than in recursive structures.

* **Disadvantages**:
    * **Limited**: If you decide to implement the bonus **Logical Operators** (`&&` and `||`) or **Parentheses**, the list becomes a logical nightmare regarding precedence.

### 2. Binary Tree (Abstract Syntax Tree - AST)

Generally used by those who want to implement the full bonus with parentheses and logical operators.

* **Advantages**:
    * **Precedence**: The tree naturally resolves execution order; deeper nodes (leaves) are executed first.
    * **Scalability**: This is how the real Bash works, making it easier to add complex commands later.

* **Disadvantages**:
    * **Parsing Complexity**: Creating the tree requires a much more sophisticated recursive parser.
    * **Complex Execution**: Requires recursive functions to traverse the tree and manage pipes between nodes.


## Quote Status - Handling Quotes

Handling quotes was a true challenge. The logic adopted was to create a status to guide us and identify if the parser was inside or outside a quote, and whether it was single or double. Creating an `enum` structure with types `OUT_QUOTE`, `IN_SQUOTE`, and `IN_DQUOTE` made this much easier.

### 1. Single Quotes (`'`) - "The Absolute Literal"

Single quotes are the strictest; they ignore **everything** inside them. Nothing is expanded or interpreted.
- **Result:** `'$VAR'` will always be the literal string `$`, `V`, `A`, `R`.

### 2. Double Quotes (`"`) - "The Selective Filter"

Double quotes protect the string from being split into multiple tokens (preserving spaces) but allow the dollar sign (`$`) and question mark (`$?`) to function.
- **Result:** `"$VAR"` becomes the value of the variable, but if the value is `ls -l`, it remains **one single token**.

In other words...

Single quotes (') preserve the literal value of all characters, while double quotes (") preserve the literal value of almost all characters, except for the dollar sign ($), which still allows for variable expansion, and the maintenance of the string as a single argument.