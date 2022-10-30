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
one they will be: Roberto Legno, Thiago, Marcello, Lapo Raspanti and Rekkless.
