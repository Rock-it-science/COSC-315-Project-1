==========================================================
|| Members			Student ID		||
|| Will McFarland		86184900		||
|| Alvin Krisnanto Putra	54658380		||
|| (Winter) Manassawin Rotsawatsuk	12682936				||
|| 							||
|| Group 14						||
==========================================================
Member contributions:
Will:
    - Wrote most of the sequential code
    - Modified parallel code and timeout system
    - Code Testing
Winter:
    - Wrote most of the parallel code and handled timed-out children
    - Wrote the documentation
    - Code Testing
Alvin:
    - Wrote some of the sequential code
    - Refactored code and general initial work on sequential code and planning
    - Code Testing

All commits by each group member can be seen on our Github repo: https://github.com/Rock-it-science/COSC-315-Project-1

Design Choices:

    Timeout:
        We decided to modify the timeout handler so it terminates any forked children that are still running after the timeout.
        We also decided remove exit(1) from the timeout handler and have closh wait for the next user command instead.

    Parallel:
        The program starts a timeout timer using alarm and signal then begins creating forks from the parent process the specified number of times by the user.
        When a child process starts, it immediately executes the program specified by the user. If the program crashes, an error message is displayed and the child process is killed using exit(1).
        Once the parent process finishes forking, it either waits for the child processes to finish or terminates everything if the child processes take longer than the specified timeout time.
        If all child processes terminate before the timeout timer, the parent program loops back to the main shell input and waits for the next set of commands from the user.

        Notes:
            The variable child_pids is used to store all the child process ids. 
            child_pids is used in conjunction waitpid and a for loop in order for the parent process to wait for the child processes to finish. 

    Sequential:
        The program starts by forking one child process. 
        The parent process then starts a timeout timer using alarm and signal while the child process executes the program specified by the user.
        The parent process then either waits for the child process to finish or terminates everything if the child process takes longer than the specified timeout time. 
        If the child process terminates before the timeout timer, the parent process then stops the timeout timer and forks another child process and repeats the steps above until the program is ran the number of times specified by the user.
        Once the user specified program has been run the specified number of times, the parent program loops back to the main shell input and waits for the next set of commands from the user.
        
        Notes:
            child_pid is used in conjunction with waitpid so the parent process waits for the child process to finish before starting the next one or terminating.

Resources used:
[1] alarm - https://pubs.opengroup.org/onlinepubs/9699919799/functions/alarm.html
[2] signal - https://pubs.opengroup.org/onlinepubs/9699919799/functions/signal.html
[3] waitpid - https://pubs.opengroup.org/onlinepubs/9699919799/functions/waitpid.html
[4] fork - https://pubs.opengroup.org/onlinepubs/9699919799/functions/fork.html
[5] sleep - https://pubs.opengroup.org/onlinepubs/9699919799/functions/sleep.html
[6] kill - https://pubs.opengroup.org/onlinepubs/9699919799/functions/kill.html
