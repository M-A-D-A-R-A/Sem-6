[Instructions Doc](/2.%20Multiagent%20Search/multiagent_search_documentation.pdf)

* Reflex Agent
* Minimax
* Alpha Beta Pruning
* Expectimax
* Evaluation Function

The main ideas of implementation were to build and design agents for Pacman, including ghosts, using mainly minimax and expectimax search algorithms and apply them on the agents to make the environment more efficient. I added the algorithms in multiagents.py by using the algorithm prompts and pseudo code as reference and building on it. It was really helpful and made the process really smooth. The agents (Pacman and Ghosts) were designed in ReflexAgent. The implemented algorithms are as follows : Minimax, Expectimax and Alpha- Beta Pruning. In addition to that I also solved the evaluation function using the most effective heuristic to follow admissibility and consistency. Creating the non trivial heuristic for betterEvaluationFunction was a little bit tricky but eventually made it through. Once Minimax was implemented it was fairly easy to follow Alpha- Beta Pruning and then using the max function for Expectimax. The Evaluation function uses basic operations and concepts from previous project to make the Pacman smart and score maximum. 
