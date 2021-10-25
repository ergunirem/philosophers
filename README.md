# philosophers
# Summary

> In this project, you will learn the basics of threading a process. You will learn how to make threads. You will discover the mutex.
> 

# Mandatory Part

One or more philosophers are sitting at a round table doing one of three things: 

eating, thinking, or sleeping.

While eating, they are not thinking or sleeping, while sleeping, they are not eating or thinking and of course, while thinking, they are not eating or sleeping.

The philosophers sit at a circular table with a large bowl of spaghetti in the center.

There are some forks on the table.

![Dining Philosophers](https://github.com/ergunirem/philosophers/blob/master/philos.png?raw=true)

> A philosopher must eat with two forks, one for each hand.
> 

> The philosophers must never be starving.
> 

> Every philosopher needs to eat.
> 

> Philosophers don’t speak with each other.
> 

> Philosophers don’t know when another philosopher is about to die.
> 

> Each time a philosopher has finished eating, he will drop his forks and start sleeping.
> 

> When a philosopher is done sleeping, he will start thinking.
> 

> The simulation stops when a philosopher dies.
> 

> Each program should have the same arguments:
> 

`number_of_philosophers` is the number of philosophers and also the number of forks

`time_to_die` is in milliseconds, if a philosopher doesn’t start eating ’time_to_die’ milliseconds after starting his last meal or the beginning of the simulation, it dies

`time_to_eat` is in milliseconds and is the time it takes for a philosopher to eat. During that time he will need to keep the two forks.
`time_to_sleep` is in milliseconds and is the time the philosopher will spend sleeping.

`number_of_times_each_philosopher_must_eat` argument is optional, if all philosophers eat at least ’number_of_times_each_philosopher_must_eat’ the simulation will stop. If not specified, the simulation will stop only at the death of a philosopher.

> Each philosopher should be given a number from 1 to ’number_of_philosophers’.
> 

> Philosopher number 1 is next to philosopher number ’number_of_philosophers’. Any other philosopher with the number N is seated between philosopher N - 1 and philosopher N + 1
> 

> Any change of status of a philosopher must be written as follows (with X replaced
with the philosopher number and timestamp_in_ms the current timestamp in milliseconds)
> 

```
 timestamp_in_ms X has taken a fork
 timestamp_in_ms X is eating
 timestamp_in_ms X is sleeping
 timestamp_in_ms X is thinking
 timestamp_in_ms X died
```

> The status printed should not be scrambled or intertwined with another philosopher’s status.
> 

> You can’t have more than 10 ms between the death of a philosopher and when it will print its death.
> 

> Again, philosophers should avoid dying!
> 

> One fork between each philosopher, therefore if they are multiple philosophers, there will be a fork at the right and the left of each philosopher.
> 

> To avoid philosophers duplicating forks, you should protect the forks state with a mutex for each of them.
> 

> Each philosopher should be a thread.
> 

### Allowed Functions

memset, printf, malloc, free, write, usleep, gettimeofday, pthread_create, pthread_detach, pthread_join, pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock

# My Notes

### Analysis

How do we write a threaded program to simulate philosophers? First, we notice that these philosophers are in a thinking-picking up chopsticks-eating-putting down chopsticks-sleeping cycle. This is a classic example of a synchronization problem.

`**The "pick up chopsticks" part is the key point**` 

How does a philosopher pick up chopsticks? Well, in a program, we simply print out messages such as ``Have left chopsticks'', which is very easy to do. The problem is each chopstick is shared by two philosophers and hence a shared resource. We certainly do not want a philosopher to pick up a chopstick that has already been picked up by his neighbor. This is `a race condition`. To address this problem, we may consider each chopstick as a shared item protected by a mutex lock. Each philosopher, before he can eat, locks his left chopstick and locks his right chopstick. If the acquisitions of both locks are successful, this philosopher now owns two locks (hence two chopsticks), and can eat. After finishes easting, this philosopher releases both chopsticks, and thinks! Because we need to lock and unlock a chopstick, each chopstick is associated with a mutex lock. Since each philosopher must have access to the two mutex locks that are associated with its left and right chopsticks, these mutex locks are global variables.

**The most serious problem of this program is that `deadlock` could occur!**

What if every philosopher sits down about the same time and picks up his left chopstick as shown in the following figure? In this case, all chopsticks are locked and none of the philosophers can successfully lock his right chopstick. As a result, we have a circular waiting (i.e. every philosopher waits for his right chopstick that is currently being locked by his right neighbor), and hence a deadlock occurs. So, any solution to the problem must include some provision for preventing or otherwise dealing with deadlocks.

**`Starvation` is also a problem!**

Imagine that two philosophers are fast thinkers and fast eaters. They think fast and get hungry fast. Then, they sit down in opposite chairs as shown below. Because they are so fast, it is possible that they can lock their chopsticks and eat. After finish eating and before their neighbors can lock the chopsticks and eat, they come back again and lock the chopsticks and eat. In this case, the other three philosophers, even though they have been sitting for a long time, they have no chance to eat. This is a **starvation**. Note that it is not a deadlock because there is no circular waiting, and every one has a chance to eat! So, any solution to the problem must include some provision for preventing starvation.

### Code & More

How to pass arguments to routine function that locks/unlocks mutexes? Sure, **a structure which contains all of the arguments** but ****there are more than one thread and therefore an index for philos and forks is needed. If I put mutexes as an array in a struct, I need to pass in the index as well separately which is not possible. Only one argument is needed. If I pass in each philosopher[i] as an argument than I need to access the mutex_t forks as well. Solution? Doubly linked structs: One big data struct which has all the arguments as well as the phillos thread array AND a philo struct which has arguments for single philo as well as a pointer to big struct data. So, both of them can be accessed from each other.

`intertwined status problem` > prevent mixed order of print messages with a mutex as well! This way, one philosopher is writing at a time.

`fork deadlock` > a small delay of time in ms between the creation of each philo thread:  *100ms delay to prevent philosophers from taking left fork simultaneously?*

`starvation problem` > a death monitor function that checks if last meal of a philo has been so long that time to die is reached. Because of the number of philosophers, the function faces time delays. To fix it, the function can be turned into a thread. To optimize time, it can be assigned to each philosopher.

`problems with usleep() func` >  since usleep is very inaccurate, it may sleep more or less than you want. This may lead to philosophers dying if you're unlucky. If you write a function that uses usleep for tiny portions at a time, it'll increase accuracy.

`milisecond calculation and gettime func` >  the microseconds in the struct are only the microsecond part AKA it doesn't count over a million So the formula: time = (now.tv_sec * 1000) + (now.tv_usec / 1000)

`one philo dying right after the other` > pthread_detach(data->philosophers[i]) + putting a big sleep time (usleep(2000)) in the death monitor kind of solved it. With 4 310 200 100, 3 and 1 still dies one after the other.

**How good is this solution?**

1. Is this solution subject to deadlock? If not, why not?
2. Is this solution subject to starvation? Why or why not?

[https://hpc.llnl.gov/training/tutorials/introduction-parallel-computing-tutorial](https://hpc.llnl.gov/training/tutorials/introduction-parallel-computing-tutorial)

[https://hpc-tutorials.llnl.gov/posix/](https://hpc-tutorials.llnl.gov/posix/)

[https://sites.cs.ucsb.edu/~rich/class/old.cs170/notes/DiningPhil/index.html](https://sites.cs.ucsb.edu/~rich/class/old.cs170/notes/DiningPhil/index.html)
