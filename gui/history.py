class History:

    def __init__(self) -> None :
        self.first_position:str = ""
        self.moves:list[str] = []

    def new_position(self, new_position:str) -> None:
        self.first_position:str = new_position
        self.moves:list[str] = []

    def push_move(self, move:str) -> None:
        '''add a move to the history'''
        self.moves.append(move)

    def get_fen_and_moves(self) -> str:
        '''Return a string with the format: first_pos moves move1 move2 move3 ...'''
        return ' '.join([self.first_position, 'moves'] + self.moves)
