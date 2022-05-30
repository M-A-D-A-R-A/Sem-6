from json.tool import main
import os
import sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from search import bfs, dfs, idfs
from interface.problem import Problem
from agents.befs_agent import BefsAgent


"""The missionaries and cannibals problem is usually stated as follows. Three missionaries and three cannibals are on one side of a river, along with a boat that can hold one or two people. Find a way to get everyone to the other side without ever leaving a group of missionaries in one place outnumbered by the cannibals in that place.

                                Boat on left = 0
         Cannibal Missionary
    left    3          3     \____/                               right
         ^^^^^^^^^^^^^^^^^^^ ----------------------- ^^^^^^^^^^^^^^

inital state = (3,3,0)

first place is the number of cannibals on left side
second place is the number of missionaries on left side
third place is the where the boat is left = 0, right = 1
"""


class MissionaryBoat(Problem):

    def __init__(self, initial, goal=None):
        self.initial = initial
        self.goal = goal

    def successor(self, state):
        children = []
        for c in range(0, 3):
            for m in range(0, 3):
                # if invalid amount of people are travelling in boat
                if(c + m > 2 or (c == 0 and m == 0)):
                    continue

                # generate the child state
                if(state[2] == 0):
                    ch = tuple((state[0] - c, state[1] - m, 1))
                else:
                    ch = tuple((state[0] + c, state[1] + m, 0))

                # if there is an invalid no of people
                if(ch[0] > 3 or ch[1] > 3 or ch[0] < 0 or ch[1] < 0):
                    continue

                # if there are more canninbals than missionaries
                if(ch[2] == 1 and ch[0] > ch[1]):
                    continue

                if(ch[2] == 0 and (3 - ch[0] > (3 - ch[1]))):
                    continue

                children.append(((c, m, ch[2]), ch))
        return children


def main():
    initial_state = tuple((3, 3, 0))
    goal_state = tuple((0, 0, 1))
    missionaryBoat = MissionaryBoat(initial=initial_state, goal=goal_state)

    # bfs(missionaryBoat)
    dfs(missionaryBoat)
    # idfs(missionaryBoat)
    # print(missionaryBoat.goal_test(goal_state))
    # befsAgent = BefsAgent(logging=True)
    # node = befsAgent.bestFirstSearch(
    #     missionaryBoat, f=lambda n: len(n))
    # if(node != None):
    #     print("GOAL FOUND")
    #     print("Path : ", node.path())
    #     print("Path cost : ", node.path_cost)
    #     # print("Depth Reached : ", depth)
    # else:
    #     print("Goal not found/n")


if __name__ == "__main__":
    main()
