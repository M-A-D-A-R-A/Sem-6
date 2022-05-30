import numpy as np
from enum import IntEnum


class Spot(IntEnum):
    PEG, FREE, OUT_OF_BOUNDS = range(3)


class Board:
    def __init__(self, board):
        self.board = board
        self.size = board.shape[0]
        # self.directions = ['n', 'e', 's', 'w']
        self.directions = ['s', 'e', 'w', 'n']

    @classmethod
    def board_from_file(cls, file_name):
        with open(file_name, 'r') as peg_file:
            _, *matrix_lines = peg_file.readlines()

        matrix = np.array([[spot for spot in line.strip().split(' ')]
                           for line in matrix_lines])

        board_matrix = np.zeros(matrix.shape, dtype=np.uint8)

        for r in range(matrix.shape[0]):
            for c in range(matrix.shape[1]):
                if matrix[r, c] == '*':
                    board_matrix[r, c] = int(Spot.PEG)
                elif matrix[r, c] == 'o':
                    board_matrix[r, c] = int(Spot.FREE)
                else:
                    board_matrix[r, c] = int(Spot.OUT_OF_BOUNDS)

        return Board(board=board_matrix,)

    @classmethod
    def copyBoard(cls, other):
        return Board(np.copy(other.board))

    def getSymmetricBoards(self):
        return [Board(np.rot90(np.copy(self.board), i)) for i in range(1, 4)]

    def successors(self):
        moves = self.get_possible_moves()
        for move in moves:
            yield (move, self.make_move(*move))

    def count_possible_moves(self):
        move = self.get_possible_moves()
        i = 0
        for m in move:
            i += 1
        return i

    def check_peg(self, start_position, direction):
        return self._get_spot(start_position, direction) == Spot.PEG

    def check_free(self, start_position, direction):
        return self._get_spot(start_position, direction) == Spot.FREE

    def is_goal(self, test):
        """
        Checks if the current board has one peg and in centre
        """
        pegs = 0
        p = 0
        q = 0
        for r in range(test.size):
            for c in range(test.size):
                if test.board[r, c] == Spot.PEG:
                    pegs += 1
                    # p = r
                    # q = c

                if pegs > 1:
                    return False
        # half = int(self.size/2)
        return pegs == 1

    def peg_count(self):
        pegs = 0

        for r in range(self.size):
            for c in range(self.size):
                if self.board[r, c] == Spot.PEG:
                    pegs += 1

        return pegs

    def free_count(self):
        free = 0

        for r in range(self.size):
            for c in range(self.size):
                if self.board[r, c] == Spot.FREE:
                    free += 1

        return free

    def get_possible_moves(self):
        """
        :returns: a list of lists of tuples of tuples of possible moves from source > destination
        """
        for free_position in self._free_positions():
            yield from [(jump, free_position) for jump in self._possible_jumps_into_empty(free_position)]

    def make_move(self, source, destination):
        """
        :param source: The coordinate of the pin that you'd like to move
        :param destination: The coordinate of the empty position that you'd like to move the pin into
        :param apply: If you'd like to apply the move to the current board and change its matrix accordingly
        :return: new_board: a new board with the move applied
        """
        new_board = Board.copyBoard(self)

        # Calculate the coordinates of the pixel that is between the source and destination
        h = tuple(np.divide(np.add(source, destination), (2, 2)))
        hop = tuple((int(h[0]), int(h[1])))

        new_board.board[source] = Spot.FREE
        new_board.board[destination] = Spot.PEG
        new_board.board[hop] = Spot.FREE

        return new_board

    @staticmethod
    def _adjusts_coords_to_direction(start_position, direction):
        """
        :param start_position: The coordinates that you'd like to start from
        :param direction: The direction that you'd like to look in
        :return: The adjusted index of (r, c) after moving in a certain direction
        """
        r, c = start_position

        r -= direction.count('n')
        r += direction.count('s')
        c += direction.count('e')
        c -= direction.count('w')

        return r, c

    def _possible_jumps_into_empty(self, empty_coord):
        """
        Given the coordinate of an empty space, will return the locations of all the pegs that could jump into that cell
        :param empty_coord: the coordinate of the empty spot that you'd like to check from
        """

        for direction in self.directions:
            if self.check_peg(empty_coord, direction) and self.check_peg(empty_coord, direction * 2):
                yield self._adjusts_coords_to_direction(empty_coord, direction * 2)

    def _free_positions(self):
        """
        Looks through the matrix and returns a list of empty spaces.
        :returns a list of (r, c) coordinates where blank spots can be found on the board.
        """
        for r in range(self.size):
            for c in range(self.size):
                if self.board[r, c] == Spot.FREE:
                    yield (r, c)

    def _get_spot(self, start_position, direction):
        """
        :param start_position: The coordinates that you'd like to start from
        :param direction: The direction that you'd like to look in
        :return: the character at that position in the board
        """

        r, c = self._adjusts_coords_to_direction(start_position, direction)
        if self._out_of_bounds(r, c):
            return '.'

        return self.board[r, c]

    def _out_of_bounds(self, r, c):
        """
        Checks to see if a given r, c is out of bounds
        """
        return min(r, c) < 0 or max(r, c) >= self.size

    def __eq__(self, other):
        return (self.board == other.board).all()

    def __str__(self):
        ret = '  '
        for i in range(self.size):
            ret += str(i) + ' '
        ret += '\n'

        for r in range(self.size):
            ret += str(r) + ' '
            for c in range(self.size):
                ret += '{} '.format(self.board[r, c])
            ret += '\n'
        return ret

    def __repr__(self) -> str:
        return "%s" % (self.board)

    def __key(self):
        return (self.board)

    def __hash__(self):
        a = self.__key()
        x = str(a)
        return hash(x)
