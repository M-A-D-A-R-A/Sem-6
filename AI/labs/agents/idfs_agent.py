from cmath import log
import sys
from typing import List
from agents.dfs_agent import DfsAgent
from interface.node import Node
from interface.problem import Problem
from numpy import inf


class IdfsAgent:
    def __init__(self, depth_limit=inf, logging=False):
        self.seq = []
        self.depth_limit = depth_limit
        self.logging = logging
        self.nodes_explored = 0

    def iterativeDepthFirstSearch(self, problem: Problem) -> List[Node]:
        for d in range(sys.maxsize):
            depth, nodes_explored, dfsGoal = DfsAgent().depthFirstSearch(
                problem, depth_limit=d)
            if(dfsGoal != 'Not found'):
                return (depth, nodes_explored, dfsGoal)
            else:
                print("Goal not found")
                print("Depth Limit : ", depth)
            print("nodes explored : ", nodes_explored)
            print()
        return (depth, nodes_explored, 'Not found',)

