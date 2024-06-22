import numpy as np
import math

SearchDepth = 2

class FiveChessAI:
    def __init__(self):
        self.BOARD_SIZE = 15
        self.PLAYER_PIECE = 1
        self.AI_PIECE = -1
        self.WINNING_CONDITION = 5
        self.SCORE_MAP = {
            (0, 0, 0, 1, 0): 10,
            (0, 0, 1, 0, 0): 10,
            (0, 1, 0, 0, 0): 10,
            (1, 0, 0, 0, 0): 10,
            (0, 0, 0, 1, 1, 0): 50,
            (0, 0, 1, 1, 0, 0): 50,
            (0, 1, 1, 0, 0, 0): 50,
            (1, 1, 0, 0, 0, 0): 50,
            (0, 0, 0, 1, 1, 1, 0): 500,
            (0, 0, 1, 1, 1, 0, 0): 500,
            (0, 1, 1, 1, 0, 0, 0): 500,
            (1, 1, 1, 0, 0, 0, 0): 500,
            (0, 0, 0, 1, 1, 1, 1, 0): 5000,
            (0, 0, 1, 1, 1, 1, 0, 0): 5000,
            (0, 1, 1, 1, 1, 0, 0, 0): 5000,
            (1, 1, 1, 1, 0, 0, 0, 0): 5000,
            (0, 0, 0, 0, 0, 0): 0
        }
        self.board = self.initialize_board()

    def initialize_board(self):
        return np.zeros((self.BOARD_SIZE, self.BOARD_SIZE))

    def check_win(self, piece):
        for r in range(self.BOARD_SIZE):
            for c in range(self.BOARD_SIZE):
                if c + self.WINNING_CONDITION <= self.BOARD_SIZE and np.array_equal(self.board[r, c:c+self.WINNING_CONDITION], np.full((self.WINNING_CONDITION,), piece)):
                    return True
                if r + self.WINNING_CONDITION <= self.BOARD_SIZE and np.array_equal(self.board[r:r+self.WINNING_CONDITION, c], np.full((self.WINNING_CONDITION,), piece)):
                    return True
                if r + self.WINNING_CONDITION <= self.BOARD_SIZE and c + self.WINNING_CONDITION <= self.BOARD_SIZE and np.array_equal(np.diagonal(self.board[r:r+self.WINNING_CONDITION, c:c+self.WINNING_CONDITION]), np.full((self.WINNING_CONDITION,), piece)):
                    return True
                if r - self.WINNING_CONDITION >= -1 and c + self.WINNING_CONDITION <= self.BOARD_SIZE and np.array_equal(np.diagonal(self.board[r:r-self.WINNING_CONDITION:-1, c:c+self.WINNING_CONDITION]), np.full((self.WINNING_CONDITION,), piece)):
                    return True
        return False

    def evaluate(self):
        score = 0
        for r in range(self.BOARD_SIZE):
            for c in range(self.BOARD_SIZE):
                if self.board[r, c] == 0:
                    continue
                if c + 4 < self.BOARD_SIZE:
                    piece = tuple(self.board[r, c:c+5])
                    score += self.SCORE_MAP.get(piece, 0)
                if r + 4 < self.BOARD_SIZE:
                    piece = tuple(self.board[r:r+5, c])
                    score += self.SCORE_MAP.get(piece, 0)
                if r + 4 < self.BOARD_SIZE and c + 4 < self.BOARD_SIZE:
                    piece = tuple(np.diagonal(self.board[r:r+5, c:c+5]))
                    score += self.SCORE_MAP.get(piece, 0)
                if r - 4 >= 0 and c + 4 < self.BOARD_SIZE:
                    piece = tuple(np.diagonal(self.board[r:r-5:-1, c:c+5]))
                    score += self.SCORE_MAP.get(piece, 0)
        return score

    def minimax(self, depth, alpha, beta, maximizing_player):
        if depth == 0 or self.check_win(self.PLAYER_PIECE) or self.check_win(self.AI_PIECE):
            return self.evaluate()

        if maximizing_player:
            max_eval = -math.inf
            for r in range(self.BOARD_SIZE):
                for c in range(self.BOARD_SIZE):
                    if self.board[r, c] == 0:
                        self.board[r, c] = self.AI_PIECE
                        eval = self.minimax(depth-1, alpha, beta, False)
                        self.board[r, c] = 0
                        max_eval = max(max_eval, eval)
                        alpha = max(alpha, eval)
                        if beta <= alpha:
                            break
            return max_eval
        else:
            min_eval = math.inf
            for r in range(self.BOARD_SIZE):
                for c in range(self.BOARD_SIZE):
                    if self.board[r, c] == 0:
                        self.board[r, c] = self.PLAYER_PIECE
                        eval = self.minimax(depth-1, alpha, beta, True)
                        self.board[r, c] = 0
                        min_eval = min(min_eval, eval)
                        beta = min(beta, eval)
                        if beta <= alpha:
                            break
            return min_eval

    def ai_move(self):
        print("AI正在思考")
        best_move = None
        max_eval = -math.inf
        for r in range(self.BOARD_SIZE):
            for c in range(self.BOARD_SIZE):
                if self.board[r, c] == 0:
                    self.board[r, c] = self.AI_PIECE
                    eval = self.minimax(SearchDepth, -math.inf, math.inf, False)  # 调整搜索深度
                    self.board[r, c] = 0
                    if eval > max_eval:
                        max_eval = eval
                        best_move = (r, c)
        print(f"AI:({r},{c})")
        return best_move

    def player_move(self, r , c):
        if self.board[r, c] == 0:
            self.board[r, c] = self.PLAYER_PIECE
        else:
            print("这个位置已经有棋子了，请选择其他位置。")

    def play_game(self):
        print("初始化棋盘:")
        print(self.board)
        while not self.check_win(self.PLAYER_PIECE) and not self.check_win(self.AI_PIECE):
            player_move = tuple(map(int, input("请输入你的下一步落子坐标（如：1,1）：").split(',')))
            self.player_move(player_move)
            print("玩家落子后的棋盘:")
            print(self.board)
            if self.check_win(self.PLAYER_PIECE):
                print("玩家胜利！")
                break
            if np.count_nonzero(self.board == 0) == 0:
                print("平局！")
                break
            ai_move = self.ai_move()
            self.board[ai_move] = self.AI_PIECE
            print("AI落子后的棋盘:")
            print(self.board)
            if self.check_win(self.AI_PIECE):
                print("AI胜利！")
                break
            if np.count_nonzero(self.board == 0) == 0:
                print("平局！")
                break
