# NEAT-Env (WIP)

The target of this project is to train NN in order to play games using the [NEAT algorithm](https://en.wikipedia.org/wiki/Neuroevolution_of_augmenting_topologies) originally designed by K. O. Stanley (original paper [here](nn.cs.utexas.edu/downloads/papers/stanley.ec02.pd)) 
The learning algorithm is designed to be completely separated from the game environment, which only need to implements the *Environment* interface. 
The rendering is completely up to the environment. Neat-Env does not provide any drawing utilities.

## Build
The project is CMake based so just type
```
cmake CMakeLists.txt
make
```

## Custom environment
To implement your own game environment, provide your implementation inside the project folder. Place your implementation in the ```src/``` and ```include/``` folders to be visible. 


## Motivation
When training a NN a loss function is required in order to perform the minimization. A classification problem will have the correct answer set to 0.0 and the minimum set at 1.0. 
When it comes to play games, there is no such a function to minimize since every decision can not be considered on its own. Any attempt to write one,  will bias the system over the final target, not allowing the agent to completely visit all the possible choices and strategies.
NEAT instead does not attempt to train a fully connected network but instead creates the network layout on its own, assigning a fitness score to each one. Then a genetic algorithm is applied transmitting the features of the most successful networks to the next generation. 