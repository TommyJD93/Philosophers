<div align='center'> <h1>Philosophers</h1> </div>
<p align='center'>I never thought philosophy would be so deadly</p>
<p align='center'>Dining philosophers problem's solution for 42 cursus project</p>
<div align='center'> <h1>General idea</h1> </div>
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
<p>Let's say that Roberto Legno wants to eat, so he picks his right and left fork, at this point we notice that Rekkless can't eat since Roberto Legno picked his right fork witch was shared with Rekkless; this might seem a little obvious but keep in mind this situation because the main problem of this project is how to organize the eating action of the philosophers.<br>
Probably the first solution that came to your mind is to simply make the odd and even philos eat separately, well we are not going to do that, it's too hard coded and we would loose the meaning of the project, <i><b>philos have to organize by themselves.</b></i></br>
But, how are we going to do that? Using mutex!</p></p>

<div align='center'> <h1>Race Conditions & Mutex</h1> </div>

<h3>Race conditions</h3>
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

<h3>Mutex</h3>
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
You have surely noticed that we initialize and destroy the mutex, and you <b><i>have to</b></i> do that every time you want to use a mutex (destroy it after you finished using it) otherwise it won't work. <b><i>Please note that if you don't destroy a mutex you may end up having leaks, but MacOS aren't detected.</b></i><br>
If you want more informations about mutex i suggest you to take a look at the pthread documentation and to the video below<br>
<a href='https://www.youtube.com/watch?v=oq29KUy29iQ'>video about mutex</a>

<div align='center'> <h1>Step to step guide</h1> </div>

<h3>Step 1: Checking the input</h3>
The first thing we are going to do it checking the input that we recive. As first thing let's take a analyze a standard input that we are going to recive: 5 800 200 200 7
<ul>
<li>5: is the number of philos</li>
<li>800: is the time a philosopher must fast to die</li>
<li>200: is the time a philosopher takes to eat</li>
<li>200: is the time a philosopher takes to sleep</li>
<li>7: are the times all the philos must eat in order to terminate the program</li>
</ul>

Let's think to the conditions the inputs has to respect: obviously the input must contain only numbers; the correction sheet, at the moment I'm writing this guide (4/11/2022) tells you to test with no more that 200 philos so we can set the limit of philos to 200 and can't be less than 1; for what concerns the death, eat and think time the only thing you. have to check is that they are bigger than 0. 
<h3>Step 2: Creating the structures</h3>
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
