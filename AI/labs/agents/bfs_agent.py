from interface.node import Node
from interface.problem import Problem
from interface.queues import FIFOQueue


class BfsAgent:
    def __init__(self, logging=False):
        self.seq = []
        self.depth = 0
        self.logging = logging
        self.nodes_explored = 0

    def breadthFirstSearch(self, problem: Problem):
        node = Node(problem.initial)
        if problem.goal_test(node.state):
            return (node.path_cost, self.nodes_explored, node,)
            # candidate_path.append(node)
        frontier = FIFOQueue([node])
        explorer = set()
        while frontier:
            node = frontier.popleft()
            self.nodes_explored += 1
            explorer.add(tuple((node.state, node.path_cost)))
            children = node.expand(problem)

            if self.logging:
                print("current   : ", node, "depth : ", node.path_cost)

            for child in children:
                if child not in frontier:
                    flag = True
                    # dont add those nodes who are having the same state but more path cost if they are already in explorer
                    for state, depth in explorer:
                        if state == child.state and depth <= child.path_cost:
                            flag = False
                            break
                    if flag:
                        if problem.goal_test(child.state):
                            return (child.path_cost, self.nodes_explored, child,)
                        frontier.append(child)

        # if candidate_path:
        #     for x in candidate_path:
        #         print(x, "\n",x.path_cost)
        return ('unknown', self.nodes_explored, 'Not found',)
