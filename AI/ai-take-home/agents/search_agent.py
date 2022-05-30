import time
from interface.node import Node
from interface.problem import Problem
from interface.queues import PriorityQueue



class SearchAgent:
    def __init__(self, logging=False):
        self.logging = logging
        self.nodes_explored = 0

    def search(self, problem: Problem, f):
        node = Node(problem.initial)
        frontier = PriorityQueue("min", f)
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
                print("path cost : ", node.path_cost,)
            for child in children:
                if problem.goal_test(child.state):
                    return (child.path_cost, self.nodes_explored, child)
                if child.state in explored or child.state in frontier:
                    continue
                else:
                    frontier.append(child)

        return ('unknown', self.nodes_explored, 'Not found',)

