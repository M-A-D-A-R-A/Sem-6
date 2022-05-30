# multiAgents.py
# --------------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
#
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


from util import getProbability, manhattanDistance
from game import Directions
import random
import util

from game import Agent


class ReflexAgent(Agent):
    """
    A reflex agent chooses an action at each choice point by examining
    its alternatives via a state evaluation function.

    The code below is provided as a guide.  You are welcome to change
    it in any way you see fit, so long as you don't touch our method
    headers.
    """

    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {NORTH, SOUTH, WEST, EAST, STOP}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(
            gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(
            len(scores)) if scores[index] == bestScore]
        # Pick randomly among the best
        chosenIndex = random.choice(bestIndices)

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [
            ghostState.scaredTimer for ghostState in newGhostStates]

        "*** YOUR CODE HERE ***"
        # eating food while ghosts are not nearby
        newFood = successorGameState.getFood().asList()
        foodList = float("inf")
        for food in newFood:
            foodList = min(foodList, manhattanDistance(newPos, food))

        # avoiding ghosts if they are nearby
        for ghost in successorGameState.getGhostPositions():
            if(manhattanDistance(newPos, ghost) < 2):
                return -float('inf')

        # returning the reciprocal instead of original values
        reciprocal = successorGameState.getScore() + 1.0/foodList
        return reciprocal


def scoreEvaluationFunction(currentGameState):
    """
    This default evaluation function just returns the score of the state.
    The score is the same one displayed in the Pacman GUI.

    This evaluation function is meant for use with adversarial search agents
    (not reflex agents).
    """
    return currentGameState.getScore()


class MultiAgentSearchAgent(Agent):
    """
    This class provides some common elements to all of your
    multi-agent searchers.  Any methods defined here will be available
    to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

    You *do not* need to make any changes here, but you can if you want to
    add functionality to all your adversarial search agents.  Please do not
    remove anything, however.

    Note: this is an abstract class: one that should not be instantiated.  It's
    only partially specified, and designed to be extended.  Agent (game.py)
    is another abstract class.
    """

    def __init__(self, evalFn='scoreEvaluationFunction', depth='2'):
        self.index = 0  # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)


class MinimaxAgent(MultiAgentSearchAgent):
    """
    Your minimax agent (question 2)
    """

    def getAction(self, gameState):
        """
        Returns the minimax action from the current gameState using self.depth
        and self.evaluationFunction.

        Here are some method calls that might be useful when implementing minimax.

        gameState.getLegalActions(agentIndex):
        Returns a list of legal actions for an agent
        agentIndex=0 means Pacman, ghosts are >= 1

        gameState.generateSuccessor(agentIndex, action):
        Returns the successor game state after an agent takes an action

        gameState.getNumAgents():
        Returns the total number of agents in the game

        gameState.isWin():
        Returns whether or not the game state is a winning state

        gameState.isLose():
        Returns whether or not the game state is a losing state
        """
        "*** YOUR CODE HERE ***"

        # gameState, current AgentIndex, current Depth
        return self.maxValue(gameState, 0, 0)[0]

    # checking depth to evaluate the current game state
    def value(self, gameState, agentIndex, depth):
        if depth is self.depth * gameState.getNumAgents() or gameState.isLose() or gameState.isWin():
            return self.evaluationFunction(gameState)
        # Pacman - returning max successor value (MAX agent)
        if agentIndex == 0:
            return self.maxValue(gameState, agentIndex, depth)[1]
        else:  # Ghost - returning max successor value (MIN agent)
            return self.minValue(gameState, agentIndex, depth)[1]

    def minValue(self, gameState, agentIndex, depth):
        # best case for ghost, never move, but thats not possible
        v = ("min", float("inf"))
        # iterating over each action performed by Ghost
        for action in gameState.getLegalActions(agentIndex):
            succState = gameState.generateSuccessor(
                agentIndex, action)  # successor state
            # next agent index
            nextIndex = (depth + 1) % gameState.getNumAgents()
            # max-value(successor)
            move = (action, self.value(succState, nextIndex, depth+1))
            v = min(v, move, key=lambda x: x[1])
        return v

    def maxValue(self, gameState, agentIndex, depth):
        v = ("max", -float("inf"))  # worst case for Pacman, idle
        # iterationg over each action performed by Pacman
        for action in gameState.getLegalActions(agentIndex):
            succState = gameState.generateSuccessor(
                agentIndex, action)  # successor state
            # next agent index
            nextIndex = (depth + 1) % gameState.getNumAgents()
            # min-value(successor)
            move = (action, self.value(succState, nextIndex, depth+1))
            v = max(v, move, key=lambda x: x[1])
        return v

        # util.raiseNotDefined()


class AlphaBetaAgent(MultiAgentSearchAgent):
    """
    Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState):
        """
        Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"

        # gameState, current AgentIndex, current Depth, alpha, beta
        return self.maxValue(gameState, 0, 0, -float("inf"), float("inf"))[0]

    # checking depth to evaluate the current game state
    def alphabeta(self, gameState, agentIndex, depth, alpha, beta):
        if depth is self.depth * gameState.getNumAgents() or gameState.isLose() or gameState.isWin():
            return self.evaluationFunction(gameState)
        if agentIndex == 0:
            return self.maxValue(gameState, agentIndex, depth, alpha, beta)[1]
        else:
            return self.minValue(gameState, agentIndex, depth, alpha, beta)[1]

    def minValue(self, gameState, agentIndex, depth, alpha, beta):
        v = ("min", float("inf"))
        for action in gameState.getLegalActions(agentIndex):
            succState = gameState.generateSuccessor(agentIndex, action)
            nextIndex = (depth + 1) % gameState.getNumAgents()
            move = (action, self.alphabeta(
                succState, nextIndex, depth+1, alpha, beta))  # value(successor, alpha, beta)
            v = min(v, move, key=lambda x: x[1])

            if v[1] < alpha:  # Prunning
                return v
            else:
                beta = min(beta, v[1])
        return v

    def maxValue(self, gameState, agentIndex, depth, alpha, beta):
        v = ("max", -float("inf"))
        for action in gameState.getLegalActions(agentIndex):
            succState = gameState.generateSuccessor(agentIndex, action)
            nextIndex = (depth + 1) % gameState.getNumAgents()
            move = (action, self.alphabeta(
                succState, nextIndex, depth+1, alpha, beta))  # value(successor, alpha, beta)
            v = max(v, move, key=lambda x: x[1])

            if v[1] > beta:  # Prunning
                return v
            else:
                alpha = max(alpha, v[1])
        return v

       # util.raiseNotDefined()


class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState):
        """
        Returns the expectimax action using self.depth and self.evaluationFunction

        All ghosts should be modeled as choosing uniformly at random from their
        legal moves.
        """
        "*** YOUR CODE HERE ***"

        # maximum depth for expectimax
        maxDepth = self.depth * gameState.getNumAgents()
        return self.value(gameState, 0, maxDepth, "exp")[0]

    def value(self, gameState, agentIndex, depth, action):

        if depth == 0 or gameState.isLose() or gameState.isWin():
            return (action, self.evaluationFunction(gameState))
        # Pacman - returning max successor value (MAX agent)
        if agentIndex == 0:
            return self.maxValue(gameState, agentIndex, depth, action)
        else:  # Ghost - returning probability value (EXP agent)
            return self.expValue(gameState, agentIndex, depth, action)

    def expValue(self, gameState, agentIndex, depth, action):
        v = 0
        # probability for length of game state
        prob = 1.0/len(gameState.getLegalActions(agentIndex))
        for legalAction in gameState.getLegalActions(agentIndex):
            succState = gameState.generateSuccessor(agentIndex, legalAction)
            nextIndex = (agentIndex + 1) % gameState.getNumAgents()
            ret = self.value(succState, nextIndex, depth -
                             1, action)  # value(successor)
            # weighted average/ calculating the expectation from probability
            v += ret[1] * prob
        return (action, v)  # returning the action with avg.

    def maxValue(self, gameState, agentIndex, depth, action):
        v = ("max", -float("inf"))
        for legalAction in gameState.getLegalActions(agentIndex):
            succState = gameState.generateSuccessor(agentIndex, legalAction)
            nextIndex = (agentIndex + 1) % gameState.getNumAgents()
            move = None
            if depth != self.depth * gameState.getNumAgents():  # if not current depth
                move = action
            else:
                move = legalAction
            successor = self.value(succState, nextIndex, depth - 1, move)
            v = max(v, successor, key=lambda x: x[1])
        return v

        # util.raiseNotDefined()


def betterEvaluationFunction(currentGameState):
    """
    Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
    evaluation function (question 5).

    DESCRIPTION: Score more whenever : 
                        Less food remaining
                        Distance to all food remaining is less
                        Ghost far away/ reasonable enough to ignore it
    """
    "*** YOUR CODE HERE ***"

    foodPos = currentGameState.getFood().asList()
    pacmanPos = currentGameState.getPacmanPosition()
    ghostPos = currentGameState.getGhostPositions()

    foodDist = 0.001  # initializing food Distance
    minGhostDist = None  # initializing minimum Ghost Distance

    for food in foodPos:  # food heuristic
        foodDist += manhattanDistance(pacmanPos, food)

    for ghost in ghostPos:  # ghost heuristic
        ghostDist = manhattanDistance(pacmanPos, ghost)
        if minGhostDist == None or minGhostDist > ghostDist:
            minGhostDist = ghostDist
    if minGhostDist > 3:
        minGhostDist = 0

    currScore = currentGameState.getScore()  # current Score for present game state
    # current Food count for present game state
    currFood = currentGameState.getNumFood()

    return currScore + 10.0 * (1.0 / foodDist) + 10.0 * (2.0 / (currFood + 1.0)) + 0.1 * minGhostDist
   # util.raiseNotDefined()


# Abbreviation
better = betterEvaluationFunction
