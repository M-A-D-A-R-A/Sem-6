import os, sys

# from agents.befs_agent import BefsAgent
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from copy import deepcopy
from interface.problem import Problem
from search import bfs, dfs, idfs
"""In the rabbit leap problem, three east-bound rabbits stand in a line blocked by three west-bound rabbits. They are crossing a stream with stones placed in the east west direction in a line. There is one empty stone between them. The rabbits can only move forward one or two steps. They can jump over one rabbit if the need arises, but not more than that.

We know that the only thing that diffrentiaties a rabit is their direction

so we represents east boud rabbits by 1 and west bound rabbits by 2
the blank space is being represented as 0

   1    1    1    0    2    2    2
  ---  ---  ---  ---  ---  ---  --- 
        |        /|\
        |_________|

   1    0    1    1    2    2    2
  ---  ---  ---  ---  ---  ---  --- 

"""

class RabbitLeap(Problem):

    def __init__(self, initial, goal=None):
        """The constructor specifies the initial state, and possibly a goal
        state, if there is a unique goal.  Your subclass's constructor can add
        other arguments."""
        self.initial = initial
        self.goal = goal

    def successor(self, state):
        children = []
        empty = -1
        for i in range(len(state)):
            if(state[i] == 0):
                empty = i
        l = len(self.initial)
        for i in [2, 1, -2, -1]:
            # for i in range(-2,3):
            if(i == 0):
                continue

            child = list(deepcopy(state))
            dest = empty + i

            if(empty >= 0 and empty < l and dest >= 0 and dest < l):
                child[empty], child[dest] = child[dest], child[empty]
                child = tuple(child)
                children.append((i, child))

        return children


def main():
    initial_state = tuple((1, 1, 1, 0, 2, 2, 2))
    goal_state = tuple((2, 2, 2, 0, 1, 1, 1))
    rabbitLeap = RabbitLeap(initial_state, goal=goal_state)
    # bfs(rabbitLeap, log=False)
    # dfs(rabbitLeap)
    # idfs(rabbitLeap,log=False)
    print(rabbitLeap.goal_test(goal_state))
    # befsAgent = BefsAgent(logging=True)
    # node = befsAgent.bestFirstSearch(
    #     rabbitLeap, f=lambda n: len(n))
    bfs(rabbitLeap)


if __name__ == "__main__":
    main()
