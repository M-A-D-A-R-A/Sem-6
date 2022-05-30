from cmath import inf
from agents.befs_agent import BefsAgent
from interface.problem import Problem
import time
from agents.bfs_agent import BfsAgent
from agents.dfs_agent import DfsAgent
from agents.idfs_agent import IdfsAgent
from interface.problem import Problem
from interface.node import Node


def dfs(problem: Problem, log=False, depth=inf):

    print("\n\n********************* DFS ************************\n\n")
    begin = time.time()
    dfsAgent = DfsAgent(logging=log)
    depth, nodes_explored, dfsGoal = dfsAgent.depthFirstSearch(
        problem, depth_limit=depth)
    if(dfsGoal != 'Not found'):
        print("GOAL FOUND")
        print("Path : ", dfsGoal.path())
        print("Depth Reached : ", depth)
    else:
        print("Goal not found/n")
        print("Depth Limit : ", depth)
    print("nodes explored : ", nodes_explored)
    end = time.time()
    print("Time taken for execution : ", (end - begin)*1000, "ms")


def idfs(problem: Problem, log=False):

    print("\n\n********************* Iterative DFS ***********************\n\n")
    begin = time.time()
    idfsAgent = IdfsAgent(logging=log)
    depth, nodes_explored, dfsGoal = idfsAgent.iterativeDepthFirstSearch(
        problem)
    if(dfsGoal != 'Not found'):
        print("GOAL FOUND")
        print("Path : ", dfsGoal.path())
        print("Depth Reached : ", depth)
    else:
        print("Goal not found/n")
        print("Depth Limit : ", depth)
    print("nodes explored : ", nodes_explored)

    end = time.time()
    print("Time taken for execution : ", (end - begin)*1000, "ms")


def bfs(problem: Problem, log=False):
    print("\n\n********************* BFS ************************\n\n")
    begin = time.time()
    bfsAgent = BfsAgent(logging=log)
    depth, nodes_explored, bfsGoal = bfsAgent.breadthFirstSearch(problem)
    if(bfsGoal != 'Not found'):
        print("GOAL FOUND")
        print("Path : ", bfsGoal.path())
        print("Depth Reached : ", depth)
    else:
        print("Goal not found/n")
    print("nodes explored : ", nodes_explored)
    end = time.time()
    print("Time taken for execution  : ", (end - begin)*1000, "ms")


def befs(problem: Problem,hfunc, log=False):
    print("\n\n***************** Best First Search ********************\n\n")
    begin = time.time()
    bfsAgent = BefsAgent(logging=log)
    depth, nodes_explored, befsGoal = bfsAgent.bestFirstSearch(problem, f=lambda n: hfunc(n))
    if(befsGoal != 'Not found'):
        print("GOAL FOUND")
        print("Path : ", befsGoal.path())
        print("Depth Reached : ", depth)
    else:
        print("Goal not found/n")
    print("nodes explored : ", nodes_explored)
    end = time.time()
    print("Time taken for execution  : ", (end - begin)*1000, "ms")
