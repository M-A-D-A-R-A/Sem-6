from interface.problem import Problem


class Node:
    """A node in a search tree. Contains a pointer to the parent (the node
    that this is a successor of) and to the actual state for this node. Note
    that if a state is arrived at by two paths, then there are two nodes with
    the same state.  Also includes the action that got us to this state, and
    the total path_cost (also known as g) to reach the node.  Other functions
    may add an f and h value; see best_first_graph_search and astar_search for
    an explanation of how the f and h values are handled. You will not need to
    subclass this class."""

    def __init__(self, state, parent=None, action=None, path_cost=0,value=0,distance=0):
        "Create a search tree Node, derived from a parent by an action."
        self.state = state
        self.parent = parent
        self.action = action
        self.distance = distance
        self.path_cost = path_cost
        self.depth = 0
        self.value = value
        if parent:
            self.depth = parent.depth + 1

    def __repr__(self):
        return "%s " % (self.state,)

    def __len__(self): return self.path_cost

    def __lt__(self, other): return self.path_cost < other.path_cost

    def path(self):
        "Create a list of nodes from the root to this node."
        x, result = self, [self]
        while x.parent != None:
            result.append(x.parent)
            x = x.parent
        result.reverse()
        return result

    def path_actions(self,):
        "The sequence of actions to get to this node."
        if self.parent is None:
            return []  
        return self.parent.path_actions() + [self.action]


    def expand(self, problem: Problem):
        "Return a list of nodes reachable from this node. [Fig. 3.8]"
        return [Node(next, self, 0,
                     problem.path_cost(self.path_cost, self.state, dist, next),problem.value(next))
                for (dist, next) in problem.successor(self)]
