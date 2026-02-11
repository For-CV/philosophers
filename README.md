*This project was created as part of the 42 curriculum by rafael-m.*

# Philosophers

## Description
This project is a classic synchronization problem in computer science: the **Dining Philosophers**. It serves as an introduction to concurrent programming, focusing on the management of threads, processes, and shared resources without encountering deadlocks or race conditions.

The objective is to simulate a group of philosophers sitting at a round table, alternating between eating, thinking, and sleeping. To eat, a philosopher needs two forks (one from their left and one from their right). Since the number of forks equals the number of philosophers, they must synchronize their actions to ensure everyone eats and no one starves.

### Key Learning Objectives
- Understanding threads and mutexes (Mandatory part).
- Understanding processes and semaphores (Bonus part).
- Preventing deadlocks and race conditions.
- Precise time management in C.

## Instructions

### Compilation
The project uses a `Makefile` with the following rules:
- `make`: Compiles the mandatory part (`philo`).
- `make bonus`: Compiles the bonus part (`philo_bonus`).
- `make clean`: Removes object files.
- `make fclean`: Removes object files and executables.
- `make re`: Recompiles the project.

### Usage
The executable takes the following arguments:
```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

- `number_of_philosophers`: The number of philosophers and also the number of forks.
- `time_to_die`: (in milliseconds) If a philosopher doesn't start eating `time_to_die` milliseconds after starting their last meal or the beginning of the simulation, they die.
- `time_to_eat`: (in milliseconds) The time it takes for a philosopher to eat. During this time, they will hold two forks.
- `time_to_sleep`: (in milliseconds) The time a philosopher spends sleeping.
- `[number_of_times_each_philosopher_must_eat]`: (Optional) If all philosophers have eaten at least this many times, the simulation stops. If not specified, the simulation stops when a philosopher dies.

**Example:**
```bash
./philo 5 800 200 200
```

## Resources

### Documentation & Articles
- [Dining Philosophers Problem - Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Introduction to Threads (pthread)](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [man]
### AI Usage
AI (Gemini) was utilized during the development of this project for the following tasks:
- **Documentation**: Structuring and generating the `README.md` to comply with project standards.
- **Code Analysis**: Identifying potential edge cases and assisting in debugging synchronization logic.
- **Testing Strategy**: Designing test cases to verify the robustness of the simulation under stress.
