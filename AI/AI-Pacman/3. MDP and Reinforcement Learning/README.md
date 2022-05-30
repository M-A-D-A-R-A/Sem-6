[Instructions Doc](/3.%20MDP%20and%20Reinforcement%20Learning/mdp_reinforcement_learning_documentation.pdf)

* Value Iteration
* Bridge Crossing Analysis
* Policies
* Asynchronous Value Iteration
* Prioritized Sweeping Value Iteration
* Q-Learning
* Epsilon Greedy
* Bridge Crossing Revisited
* Q-Learning and Pacman
* Approximate Q-Learning

The main ideas of implementation were to design value iteration and Q-learning and test them on Gridworld, then on a simulated robot controller (Crawler) and Pacman. The value iteration agent was created to solve the known MDPs in ValueIterationAgent via an offline planner and not a reinforcement learning agent. It takes an MDP on construction and runs value iteration for the specified number of iterations. Value iteration returns maximum q values and computers k-step estimates of all the optimal values. computeActionFromValues returns the best action according to the value function and computeQValueFromValues returns the maximum q values from a given state. Then, moving on to Q-learning agent, it learns by trial and error from different interactions in the environment. For computeActionFromQValues in QLearningAgent, use of random.choice() helped break all the ties randomly for better behavior. Implemented epsilon-greedy action selection to complete the Q-learning agents and it chooses random actions. ApproximateQLearning agent learns weights for features of states and is designed according to the given equations. I felt this assignment was comparatively easy as each and every step and action was described in detail to follow.
