<div align='center'> <h1>Philosophers</h1> </div>
<p align='center'>I never thought philosophy would be so deadly</p>
<p align='center'>Dining philosophers problem's solution for 42 cursus project</p>

## <h1 align='center'>Index</h1>
1. [General idea](#General_idea)
2. [Race Conditions and Mutexes](#RaceConditions&mutexes)
	- [Race Condition](#RaceConditions)
	- [Mutexes](#Mutexes)
3. [Step to Step Guide](#Guide)
	- [Step 1: Checking the input](#step1)
	- [Step 2: Creating the structures](#step2)
	- [Step 3: Initialization and allocation](#step3)
	- [Step 4: Structuring the philo's routine, the supervisor and the monitor](#step4)
	- [Step 5: Clearing the memory](#step5)
4. [Utils Functions](#utilsfunc)
	- [clear data](#clear_data)
	- [ft_exit](#ft_exit)
	- [error](#error)
	- [get_time](#get_time)
	- [ft_usleep](#ft_usleep)


<div align='center'><h1><a name='General_idea'>General idea</a></h1></div>
The mandatory part of this project asks us to solve the <a href='https://en.wikipedia.org/wiki/Dining_philosophers_problem'>dining philosophers problem</a> and 
implement a mutithreading solution. In order to better understand the solution that we are going to implement in this project I suggest you to read something about what
a thread is and how multithreading works, I'll leave a couple of wikipedia references to start learning about these topics:
<ul>
<li><a href='https://en.wikipedia.org/wiki/Thread_(computing)'>Thread</a></li>
<li><a href='https://en.wikipedia.org/wiki/Multithreading_(computer_architecture)'>Multi Threading</a></li>
</ul>
Another raccomandation is to read the subject before starting, I'll leave a link also to that: <a href='https://cdn.intra.42.fr/pdf/pdf/51354/en.subject.pdf'>subject</a>.<br>
Now that we know what we have to do we can start explaining the general idea that I've applied in this project. First of all we have to immagine a round table, N num of 
philosophers sits around it and each of them brings a fork and let's say that they place it on the table on their right (doesn't really change if they place it on the right or left).
At this point we know that a philosopher can do three things: eat, think or sleep; but in order to eat he has to pick two forks (the one on his right and the one on his left).
Let's use a picture to have a more concrete idea of what we are talking about:
<br><br>
<a href='https://en.wikipedia.org/wiki/Dining_philosophers_problem#/media/File:An_illustration_of_the_dining_philosophers_problem.png'><img src='https://user-images.githubusercontent.com/59456000/198906008-4135d5d2-de53-4a8b-9c24-744181f04851.png' width='275' heigth='275'></img></a>
<br><br>
Since I'm not a big fan of philosophy i don't know a single name one these guys up here so I'll give them some more familiar names and starting from the bottom left
one they will be: Roberto Legno, Thiago, Marcello, Lapo Raspanti and Rekkless.<br>
<p>Let's say that Roberto Legno wants to eat, so he picks his right and left fork, at this point we notice that Rekkless can't eat since Roberto Legno picked his right fork which was shared with Rekkless; this might seem a little obvious but keep in mind this situation because the main problem of this project is how to organize the eating action of the philosophers.<br>
Probably the first solution that came to your mind is to simply make the odd and even philos eat separately, well we are not going to do that, it's too hard coded and we would loose the meaning of the project, <i><b>philos have to organize by themselves.</b></i></br>
But, how are we going to do that? Using mutex!</p></p>

<div align='center'><h1><a name='RaceConditions&mutexes'>Race Conditions & Mutex</a></h1></div>

<h3><a name='RaceConditions'>Race conditions</a></h3>
Before explaining what mutex are and why we have to use them, let's talk about what race conditions are. A <a href='https://stackoverflow.com/questions/34510/what-is-a-race-condition'>Race condition</a> it is a condition in which one or more threads are trying to access and modify a same variable at the same time, this can lead to an error in the final value of that variable. To better understan the race condition here's an example:
Let's say that we want to count to 2.000.000, to do that with the multithreading we simply make two threads that execute the same routine, and the routine increase the variable cont to 1.000.000, in this way we should execute the while inside routine 2 times and when cont is printed we should get 2.000.000. Well, that's not exactly how it works.

<pre>
<code>
#include <pthread.h>
#include <stdio.h>

int cont = 0;

void  *routine()
{
  int i;

  i = -1;
  while (++i < 1000000)
  	  cont++;
  return (NULL);
}

int main()
{
  pthread_t tid1, tid2;

  pthread_create(&tid1, NULL, &routine, NULL);
  pthread_create(&tid2, NULL, &routine, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  printf("cont: %d\n", cont);
}
</code>
</pre>

If you try to execute the code above you will see that you'll never get 2.000.000 that's because a race condition is happening. To better understand what's happening let's take a look at the assembly of the "cont++" instruction.
To increase a variable by 1 the assembly execute 3 operations:
- read, simply get the variable value 
- increase, increment locally the variable
- write, updates the value of the variable
<br>
Assume that the current value of cont is 23, let's see what the assembly would do:
<pre>
<code>
read: 23
increase: 23
write: 24
</code>
</pre>
That happens when we simply do cont++ in a non multithreaded program, let's see what happens in a mutithreaded program when a race condition happens:

<pre>
<code>
--------------------------------
|   Thread A   |   Thread B    |
| read:   23   | read:   23    |
| increase: 23 | increase:     |
| write:   24  | write:        |
--------------------------------
</code>
</pre>

At this point the thread B is paused, because is doing the same operation as A and it's restarted after a while, but while B is paused A continue to increase the cont value (for the example we say it reaches 30). Let's se what happens when B restart: 

<pre>
<code>
--------------------------------
|   Thread A   |   Thread B    |
| read:   30   | read:   23    |
| increase: 30 | increase:  23 |
| write:   31  | write:   24   |
--------------------------------
</code>
</pre>

As we can see B restart from where he was paused, since he already read 23 he won't read the current value of cont, he will keep doing his operation with the last value he read before stopping, in this case it's 23 so he will update the cont value to 24 and therefore A's next iteration won't read 31 but 24.

<a href='https://www.youtube.com/watch?v=FY9livorrJI'>video about race condition</a>

<h3><a name='Mutexes'>Mutex</a></h3>
Now that we know what a race condition is we'll talk about mutex, that are what we need to avoid a data racing. Immagine these as locks, if a mutex is already locked and a thread tries to lock it he we'll be stopped untill the mutex will be unlocked. Taking up the previous example, we could avoid the race condition simply adding a lock before we increase the value, in this way thread B can't overwrite the value of cont with what he read before being stopped.

<pre>
<code>

#include <pthread.h>
#include <stdio.h>

int cont = 0;
pthread_mutex_t lock;

void  *routine()
{
  int i;

  i = -1;
  while (++i < 1000000)
  {
    pthread_mutex_lock(&lock);
  	  cont++;
    pthread_mutex_unlock(&lock);
  }
  return (NULL);
}

int main()
{
  pthread_t tid1, tid2;

  pthread_mutex_init(&lock, NULL);
  pthread_create(&tid1, NULL, &routine, NULL);
  pthread_create(&tid2, NULL, &routine, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_mutex_destroy(&lock);
  printf("cont: %d\n", cont);
}
</code>
</pre>

You have surely noticed that we initialize and destroy the mutex, and you <b><i>have to</b></i> do that every time you want to use a mutex (destroy it after you finished using it) otherwise it won't work. <b><i>Please note that if you don't destroy a mutex you may end up having leaks, but on MacOS aren't detected.</b></i><br>
If you want more informations about mutex i suggest you to take a look at the pthread documentation and to the video below.<br><br>
<a href='https://www.youtube.com/watch?v=oq29KUy29iQ'>video about mutex</a>

<div align='center'><h1><a name='guide'>Step to step guide</a></h1></div>

<h3><a name='step1'>Step 1: Checking the input</a></h3>
The first thing we are going to do it checking the input that we recive. As first thing let's take a analyze a standard input that we are going to recive: 5 800 200 200 7
<ul>
<li>5: is the number of philos</li>
<li>800: is the time a philosopher must fast to die</li>
<li>200: is the time a philosopher takes to eat</li>
<li>200: is the time a philosopher takes to sleep</li>
<li>7: are the times all the philos must eat in order to terminate the program</li>
</ul>

Let's think to the conditions the inputs has to respect: obviously the input must contain only numbers; the correction sheet, at the moment I'm writing this guide (4/11/2022) tells you to test with no more that 200 philos so we can set the limit of philos to 200 and can't be less than 1; for what concerns the death, eat and think time the only thing you. have to check is that they are bigger than 0.

<h3><a name='step2'>Step 2: Creating the structures</a></h3>
Since we need to handle a lot of data, we will need a couple of structures. Personally I've made 2 structures: one with the general set of rules (input datas, mutex array for forks, philos array, ...); and one for the philos where I save the personal data of each philo (pointer to the general data structure, time left before his death, pointers to forks, ...). You can handle the structures as you want, but one thing that i suggest you is to make a pointer to the structure of the general data (or where you want to save the input datas) because you will need them later, I'll leave a snippet to show you how I've done that.

<pre>
<code>
struct	s_data; //this line is to define the structure before actually saying what's inside it

typedef struct s_philo
{
	struct s_data	*data;
	pthread_t       t1;
	int             id;
	int             eat_cont;
	int             status;
	int             eating;
	uint64_t        time_to_die;
	pthread_mutex_t	lock;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
} t_philo;

typedef struct s_data
{
	pthread_t       *tid;
	int             philo_num;
	int             meals_nb;
	int             dead;
	int             finished;
	t_philo         *philos;
	u_int64_t       death_time;
	u_int64_t       eat_time;
	u_int64_t       sleep_time;
	u_int64_t       start_time;
	pthread_mutex_t *forks;
	pthread_mutex_t lock;
	pthread_mutex_t write;
} t_data;
</code>
</pre>

<h3><a name='step3'>Step 3: Initialization and allocation</a></h3>
Now that we have everything setted up we need to allocate the structures, initialize all the mutexes and start the threads. First of all we allocate all the structures, then we initialize the mutexes with the function <a href='https://www.ibm.com/docs/en/i/7.3?topic=ssw_ibm_i_73/apis/users_61.html'>pthread_mutex_init()</a>, here's a little example on how to use it:

<pre>
<code>
	pthread_mutex_t	mutex;

	pthread_mutex_init(&mutex, NULL);
</code>
</pre>

Remember to put the '&' before the mutex name since the function requires a pointer to the variable, for this project you can NULL the second parameter since we are not going to specify any attribute for our mutexes.<br>
To start the threads we are going to use the function <a href='https://man7.org/linux/man-pages/man3/pthread_create.3.html'>pthread_create</a>

<pre>
<code>
void	*routine(void *data_pointer)
{
//some code
}

int main()
{
	pthread_t	tid;

	pthread_create(&tid, NULL, &routine, &data_pointer);
}
</code>
</pre>

The first parameter of this function is the pointer to the tid variable (of type pthread_t), the second one is nullable (as for pthread_mutex_init we don't have to specify attributes in this project), the third parameter is the pointer to the function that the thread is going to execute and the forth parameter is the pointer to the datas that we give to the routine function. Please note that the routine is always a "void *" function and the datas <b><i>must</b></i> be given to it through a pointer to the data that we are trying to pass.

<h3><a name='step4'>Step 4: Structuring the philo's routine, the supervisor and the monitor</a></h3>

Now that we have everything setted up we need to sturcture the routine for the philos, a supervisor for each of them that is going to tell us when a philo is dead and a monitor that is going to tell us when all the philos have eaten all the times that are required.

<ul>
<li><h3>Routine</h3></li>
The routine will be the function executed over and over by the philos, and is also going to start the supervisor for each philo.<br>
Basically we have to create a loop that will be interrupted by the death of one of the philos or by reaching the times that each of them must eat, in this loop we'll tell them to: eat, sleep and think. But how? Starting from the thinking action, a philo has to think whenever he can't eat so it's pretty simple we just have to insert the message "Philo x is thinking". Moving on we have the eating action, this one is going to be divided in four main actions: picking the two forks needed, eating, dropping the forks and sleeping (you can also make sleeping action apart).<br>
Let's start with the forks picking, this is pretty simple, a philo to pick a fork locks the mutex refered to it so we are going to use the <a href='https://www.ibm.com/docs/en/i/7.2?topic=ssw_ibm_i_72/apis/users_62.html'>pthread_mutex_lock</a> function. Note that if you consider the philos disposed clockwise you are going to lock the right fork before the left one, if you consider them disposed counterclockwise then you are going to lock the left one first.<br>
Now that the philo has taken the forks he need to eat and here we update the status(to do that I've made an int inside the philo struct) of the philo so that the supervisor will know that he's eating and he don't have to die, and then we simply use an usleep (i suggest you to recode it by your self for making it faster, you can find mine in src/utils/utils.c). Right after that, before dropping the forks, we update the philo status.<br>
At this point the philo have to drop the froks, we replicate the pick fork function but this time we use the function <a href='https://www.ibm.com/docs/en/i/7.2?topic=ssw_ibm_i_72/apis/users_65.html'>pthread_muted_unlock</a> to unlock the mutexes previously locked. At this point we make the philo sleep using another time the usleep function.

<li><h3>Supervisor</h3></li>
In this thread we are going to check whether the time passed by the last time a philo have eaten is greater or equal to the time it takes to a philo to die from starvation, to make everything work a little bit better we are also going to check if in the momet a philo should die he's eating, in that case we'll let him survive. Other than that we are also going to check when the philo reaches the number of times he must eat and mark him as "finished", in thi way the monitor will know when all the philos have eaten.

<li><h3>Monitor</h3></li>
This thread is going to be started(before all the philos) just if we have the optional parameter. Here we just have to check if the status of all the philos is marked as "finished", and in that case we are going to tell the program to stop all the threads.
</ul>

<h3><a name='step5'>Step 5: Clearing the memory</a></h3>
At this point we are almost done, we just have to join the threads, destroy the mutexes and clear the memory we have allocated. Let's start with joining threads, right below the cycle where we start the philos we make another cycle where we join them using the function <a href= 'https://www.ibm.com/docs/en/zos/2.1.0?topic=functions-pthread-join-wait-thread-end'>pthread_join</a> wich is simply going to tell the program "wait untill all the threads terminate in order to continue the execution of the program".<br>
Once we joined al the threads we need to destroy all the mutexes, to do that we are going to use the function <a href='https://www.ibm.com/docs/en/i/7.2?topic=ssw_ibm_i_72/apis/users_60.html'>pthread_mutex_destroy</a>.<br>
As final step we need to free al the allocations that we made. And that's it, now you have a perfectly functioning Philosophers!!!
</ul>

<div align='center'><h3><a name='utilsfunc'>Utils Functions</a></h3></div>
To help you understand which utils functions you will need in this project I'll leave a list with some snippets :)
<ul>

<li><h3><a name='clear_data'>clear_data</a></h3></li>
A function to clear all the memory allocations.

<pre>
<code>
void	clear_data(t_data	*data)
{
	if (data->tid)
		free(data->tid);
	if (data->forks)
		free(data->forks);
	if (data->philos)
		free(data->philos);
}
</code>
</pre>

<li><h3><a name='ft_exit'>ft_exit</a></h3></li>
A function that clears the memory by destroying the mutexes and calling the clear_data funciton that clears the allocations.

<pre>
<code>
void	ft_exit(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_num)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].lock);
	}
	pthread_mutex_destroy(&data->write);
	pthread_mutex_destroy(&data->lock);
	clear_data(data);
}
</code>
</pre>

<li><h3><a name='error'>error</a></h3></li>
A function that is called whenever an error occours and clears all the mutexes and allocations (calling the ft_exit function)

<pre>
<code>
int	error(char *str, t_data *data)
{
	printf("%s\n", str);
	if (data)
		ft_exit(data);
	return (1);
}
</code>
</pre>


<li><h3><a name='get_time'>get_time</a></h3></li>
A fucntion to take the current time in milliseconds.

<pre>
<code>
u_int64_t	get_time(void)
{
	struct timeval	tv;
	
	if (gettimeofday(&tv, NULL))
		return (error("gettimeofday() FAILURE\n", NULL));
	return ((tv.tv_sec * (u_int64_t)1000) + (tv.tv_usec / 1000));
}
</code>
</pre>

<li><h3><a name='ft_usleep'>ft_usleep</a></h3></li>
Basically a recode of the usleep function, just more precise.

<pre>
<code>
int	ft_usleep(useconds_t time)
{
	u_int64_t	start;
	start = get_time();
	while ((get_time() - start) < time)
		usleep(time / 10);
	return(0);
}
</code>
</pre>
</ul>
