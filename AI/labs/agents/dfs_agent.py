from interface.node import Node
from interface.problem import Problem
from interface.queues import LIFOQueue
from numpy import inf


class DfsAgent:
    def __init__(self, logging=False):
        self.seq = []
        self.nodes_explored = 0
        self.logging = logging

    def depthFirstSearch(self, problem: Problem, depth_limit=inf,):
        frontier = LIFOQueue([Node(problem.initial)])
        explorer = set()
        # candidate_path = []
        while frontier:
            node = frontier.pop()
            # print(node.state)
            self.nodes_explored += 1
            if problem.goal_test(node.state):
                return (node.path_cost, self.nodes_explored, node,)
                # candidate_path.append(node)
            explorer.add((node.state, node.path_cost))

            children = node.expand(problem)

            if self.logging:
                print("current   : ", node,  "depth : ", node.path_cost)
                
                # "\nexploring : ",
                #       children,)

            for child in children:
                if child not in frontier and child.path_cost <= depth_limit:
                    flag = True
                    # dont add those nodes who are having the same state but more path cost if they are already in explorer
                    for state, depth in explorer:
                        if state == child.state and depth <= child.path_cost:
                            flag = False
                            break
                    if flag:
                        frontier.append(child)

        # if candidate_path:
        #     for x in candidate_path:
        #         print(x, "\n", x.path_cost)
        return (depth_limit, self.nodes_explored, 'Not found',)
