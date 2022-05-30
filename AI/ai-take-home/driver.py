from agents.search_agent import SearchAgent
from interface.problem import Problem
import os
import sys
import time
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


class TakeHomeMidSem(Problem):

    def __init__(self, initial, goal, cities):
        """The constructor specifies the initial state, and possibly a goal
        state, if there is a unique goal.  Your subclass's constructor can add
        other arguments."""
        self.cities = cities
        self.initial = initial
        self.goal = goal

    def h(self, node):
        r = abs(node.state[0] - self.goal[0])
        c = abs(node.state[1] - self.goal[1])
        return 10*(r + c)

    def g(self, node):
        return node.path_cost

    def successor(self, node):
        children = []
        for source, dest, dist in self.cities:
            if node.state == source:
                children.append(tuple((dist, dest)))
        return children


def main():
    list_of_city = [
        (tuple((0, 0)),  tuple((1, 1)),  25),
        (tuple((1, 1)),  tuple((0, 0)),  25),
        (tuple((1, 1)),  tuple((2, 0)),  24),
        (tuple((1, 1)),  tuple((0, 3)),  32),
        (tuple((1, 1)),  tuple((3, 1)),  26),
        (tuple((2, 0)),  tuple((1, 1)),  24),
        (tuple((2, 0)),  tuple((4, 0)),  27),
        (tuple((0, 3)),  tuple((1, 1)),  32),
        (tuple((0, 3)),  tuple((0, 4)),  11),
        (tuple((0, 3)),  tuple((2, 4)),  36),
        (tuple((3, 1)),  tuple((1, 1)),  26),
        (tuple((3, 1)),  tuple((4, 0)),  21),
        (tuple((3, 1)),  tuple((4, 3)),  32),
        (tuple((4, 0)),  tuple((3, 1)),  21),
        (tuple((4, 0)),  tuple((2, 0)),  27),
        (tuple((4, 0)),  tuple((5, 1)),  26),
        (tuple((5, 1)),  tuple((4, 0)),  26),
        (tuple((5, 1)),  tuple((6, 2)),  27),
        (tuple((6, 2)),  tuple((5, 1)),  27),
        (tuple((6, 2)),  tuple((6, 6)),  52),
        (tuple((4, 3)),  tuple((3, 1)),  32),
        (tuple((4, 3)),  tuple((5, 4)),  23),
        (tuple((4, 3)),  tuple((3, 3)),  20),
        (tuple((5, 4)),  tuple((4, 3)),  32),
        (tuple((3, 3)),  tuple((4, 3)),  20),
        (tuple((3, 3)),  tuple((2, 4)),  22),
        (tuple((2, 4)),  tuple((0, 3)),  36),
        (tuple((2, 4)),  tuple((3, 3)),  22),
        (tuple((2, 4)),  tuple((2, 6)),  28),
        (tuple((2, 4)),  tuple((4, 7)),  44),
        (tuple((0, 4)),  tuple((0, 3)),  11),
        (tuple((0, 4)),  tuple((2, 6)),  42),
        (tuple((0, 4)),  tuple((0, 6)),  24),
        (tuple((0, 6)),  tuple((0, 4)),  24),
        (tuple((0, 6)),  tuple((1, 8)),  40),
        (tuple((1, 8)),  tuple((0, 6)),  40),
        (tuple((1, 8)),  tuple((2, 6)),  32),
        (tuple((2, 6)),  tuple((0, 4)),  42),
        (tuple((2, 6)),  tuple((2, 4)),  28),
        (tuple((2, 6)),  tuple((4, 7)),  27),
        (tuple((2, 6)),  tuple((5, 8)),  62),
        (tuple((2, 6)),  tuple((4, 7)),  27),
        (tuple((4, 7)),  tuple((2, 4)),  44),
        (tuple((4, 7)),  tuple((2, 6)),  27),
        (tuple((4, 7)),  tuple((5, 8)),  32),
        (tuple((4, 7)),  tuple((6, 8)),  42),
        (tuple((5, 8)),  tuple((2, 6)),  62),
        (tuple((5, 8)),  tuple((4, 7)),  42),
        (tuple((6, 8)),  tuple((4, 7)),  42),
        (tuple((6, 8)),  tuple((6, 6)),  32),
        (tuple((6, 6)),  tuple((6, 8)),  32),
        (tuple((6, 6)),  tuple((6, 2)),  52),
    ]
    initial_state = tuple((0, 0))
    goal_state = tuple((6, 8))
    task = TakeHomeMidSem(initial_state, goal_state, list_of_city)
    print("\n\n***************** Searching Space ********************\n")
    search(task, 1, 0) # w1 = 1; w2 = 0 (Maintain a frontier)
    search(task, 0, 1) # w1 = 0; w2 = 1 (Maintain a frontier)
    search(task, 1, 1) # w1 = 1; w2 = 1 (Maintain a frontier)


def search(problem: Problem, w1, w2, log=False):
    print("W1 = ", w1, "\nW2 = ", w2, "\n")
    begin = time.time()
    searchAgent = SearchAgent(logging=log)
    depth, nodes_explored, goal = searchAgent.search(
        problem, f=lambda n: w1 * problem.g(n) + w2 * problem.h(n))
    if(goal != 'Not found'):
        print("GOAL FOUND")
        print("Path : ", goal.path())
        print("Total Path Cost : ", depth)
    else:
        print("Goal not found/n")
    print("nodes explored : ", nodes_explored)
    end = time.time()
    print("Time taken for execution  : ", (end - begin)*1000, "ms\n")


if __name__ == "__main__":
    main()
