import csv
import sys

Transitions = {}
Reward = {}

#gamma is the discount factor
if len(sys.argv)>1:
    gamma = float(sys.argv[1])
else:
    gamma = 0.9

#the maximum error allowed in the utility of any state
if len(sys.argv)>2:
    epsilon = float(sys.argv[2])
else:
    epsilon = 0.001

def read_file():
    #read transitions from file and store it to a variable
    with open('transition.csv', 'r') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        for row in reader:
            if row[0] in Transitions:
                if row[1] in Transitions[row[0]]:
                    Transitions[row[0]][row[1]].append((float(row[3]), row[2]))
                else:
                    Transitions[row[0]][row[1]] = [(float(row[3]), row[2])]
            else:
                Transitions[row[0]] = {row[1]:[(float(row[3]),row[2])]}

    #read rewards file and save it to a variable
    with open('rewards.csv', 'r') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        for row in reader:
            Reward[row[0]] = float(row[1]) if row[1] != 'None' else None

read_file()


class MarkovDecisionProcess:

    """A Markov Decision Process, defined by an states, actions, transition model and reward function."""

    def __init__(self, transition={}, reward={}, gamma=.9):
        #collect all nodes from the transition models
        self.states = transition.keys()
        #initialize transition
        self.transition = transition
        #initialize reward
        self.reward = reward
        #initialize gamma
        self.gamma = gamma

    def R(self, state):
        """return reward for this state."""
        return self.reward[state]

    def actions(self, state):
        """return set of actions that can be performed in this state"""
        return self.transition[state].keys()

    def T(self, state, action):
        """for a state and an action, return a list of (probability, result-state) pairs."""
        return self.transition[state][action]

#Initialize the MarkovDecisionProcess object
mdp = MarkovDecisionProcess(transition=Transitions, reward=Reward)

def value_iteration():
    """
    Solving the MDP by value iteration.
    returns utility values for states after convergence

    # Input π, the policy to be evaluated
    # Algorithm parameter: a small threshold θ > 0 determining accuracy of estimation
    # Initialize V (s), for all s = S*, arbitrarily except that V (terminal)=0

    # 2. Policy Evaluation
    # Loop:
    # ∆ 0 <- 0
    # Loop for each s = S*:
        # v <- V (s)
        # V (s) sum {P a π(a|s) P s 0 ,r p(s 0 , r|s, a) ⇥ r + γV (s 0 )}
        # ∆ tends to max(∆, |v V (s)|)
    # while ∆ < 0
    """
    states = mdp.states
    actions = mdp.actions
    T = mdp.T
    R = mdp.R

    #initialize value of all the states to 0 (this is k=0 case)
    V1 = {s: 0 for s in states}
    while True:
        V = V1.copy()
        delta = 0
        for s in states:
            #Bellman update, update the utility values
            V1[s] = R(s) + gamma * max([ sum([p * V[s1] for (p, s1) in T(s, a)]) for a in actions(s)])
            #calculate maximum difference in value
            delta = max(delta, abs(V1[s] - V[s]))

        #check for convergence, if values converged then return V
        if delta < epsilon * (1 - gamma) / gamma:
            return V


def best_policy(V):
    """
    Given an MDP and a utility values V, determine the best policy as a mapping from state to action.
    returns policies which is dictionary of the form {state1: action1, state2: action2}

    # Policy Iteration (using iterative policy evaluation) for estimating π = π*

    # 1. Initialization
    # V (s) 2 R and π(s) = A(s) arbitrarily for all s = S

    # 2. Policy Evaluation
    # Loop:
    # ∆ 0 <- 0
    # Loop for each s = S*:
        # v <- V (s)
        # V (s) sum {P a π(a|s) P s 0 ,r p(s 0 , r|s, a) ⇥ r + γV (s 0 )}
        # ∆ tends to max(∆, |v V (s)|)
    # while ∆ < 0

    # 3. Policy Improvement
    # policy-stable true
    # For each s =  S:
        # old-action <- π(s)
        # π(s) <- sum {P a π(a|s) P s 0 ,r p(s 0 , r|s, a) ⇥ r + γV (s 0 )}
        #If old-action 6= π(s), then policy-stable f alse
    # If policy-stable, then stop and return V ⇡ v⇤ and π ⇡ π⇤; else go to 2

    """
    states = mdp.states
    actions = mdp.actions
    pi = {}
    for s in states:
        pi[s] = max(actions(s), key=lambda a: expected_utility(a, s, V))
    return pi


def expected_utility(a, s, V):
    """returns the expected utility of doing a in state s, according to the MDP and V."""
    T = mdp.T
    return sum([p * V[s1] for (p, s1) in mdp.T(s, a)])


#call value iteration
V = value_iteration()
print ('State - Value')
for s in V:
    print (s, ' - ' , V[s])
pi = best_policy(V)
print ('\nOptimal policy is \nState - Action')
for s in pi:
    print (s, ' - ' , pi[s])