import os
import sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from interface.problem import Problem
from interface.board import Board
from agents.befs_agent import BefsAgent
import search


class MarbleSolitaire(Problem):

    def __init__(self, initial, goal=None):
        """The constructor specifies the initial state, and possibly a goal
        state, if there is a unique goal.  Your subclass's constructor can add
        other arguments."""
        self.initial = initial
        self.goal = goal

    def successor(self, state: Board):
        return state.successors()

    def goal_test(self, state: Board):
        return state.is_goal(state)

    def value(self, state: Board):
        # return self.possibleMoves(state)
        # return self.heuristicMatrix(state)
        return self.mandist(state)
        # return max(self.mandist(state),self.heuristicMatrix(state))

    def mandist(self, state: Board):
        man = 0
        for r, c in state._free_positions():
            for i, j in state._free_positions():
                x = abs(r - i)
                y = abs(c - j)
                temp = x + y
                if temp < 2 and (x != 0 or y != 0):
                    man += 1
        return man

    def heuristicMatrix(self, state: Board):
        v = 0
        h = [[0, 0, 8, 8, 8, 0, 0, ],
             [0, 0, 6, 6, 6, 0, 0, ],
             [8, 6, 4, 4, 4, 6, 8, ],
             [8, 6, 4, 2, 4, 6, 8, ],
             [8, 6, 4, 4, 4, 6, 8, ],
             [0, 0, 6, 6, 6, 0, 0, ],
             [0, 0, 8, 8, 8, 0, 0, ], ]
        for r, c in state._free_positions():
            v += h[r][c]
        return v/2

    def possibleMoves(self, state):
        return state.count_possible_moves()


def main():
    initial_state = Board.board_from_file("./inputs/marble.txt")
    goal_state = Board.board_from_file("./inputs/goal.txt")
    marble = MarbleSolitaire(initial=initial_state, goal=goal_state)
    print(marble.value(goal_state))
    search.befs(marble, hfunc=lambda n: marble.value(n.state), log=True)


if __name__ == "__main__":
    main()
