import time
from interface.node import Node
from interface.problem import Problem
from interface.queues import PriorityQueue


class BefsAgent:
    def __init__(self, logging=False):
        self.seq = []
        self.depth = 0
        self.logging = logging
        self.nodes_explored = 0

    def bestFirstSearch(self, problem: Problem, f):
        node = Node(problem.initial)
        frontier = PriorityQueue("max", f)
        frontier.append(node)
        explored = set()
        while frontier:
            node = frontier.pop()
            if problem.goal_test(node.state):
                return (node.path_cost, self.nodes_explored, node)
            self.nodes_explored += 1
            explored.add(node.state)
            children = node.expand(problem)
            if self.logging:
                printArray(node.state.board)
                print("path cost :", node.path_cost,"node value :" ,node.value,)
                x = 5
            for child in children:
                if problem.goal_test(child.state):
                    return (child.path_cost, self.nodes_explored, child)
                if child.state in explored:
                    continue
                else:
                    frontier.append(child)
                    for x in child.state.getSymmetricBoards():
                        explored.add(x)
        return ('unknown', self.nodes_explored, 'Not found',)


def printArray(a):
    for row in range(len(a[0])):
        for col in range(len(a[0])):
            b = print("{}".format(a[row][col]), end=" ")
        print(b)
