board = [
  [
    "a",
    "b",
    "a",
    "a",
    "b",
    "a",
    "e"
  ],
  [
    "a",
    "b",
    "b",
    "b",
    "a",
    "e",
    "e"
  ],
  [
    "b",
    "a",
    "a",
    "b",
    "e",
    "e",
    "e"
  ],
  [
    "b",
    "a",
    "b",
    "e",
    "e",
    "e",
    "e"
  ],
  [
    "e",
    "e",
    "e",
    "e",
    "e",
    "e",
    "e"
  ],
  [
    "e",
    "e",
    "e",
    "e",
    "e",
    "e",
    "e"
  ]
]

red_cells = []
yellow_cells = []

for file in range(7):
    for rank in range(6):
        cell = board[rank][file]
        
        if cell == "a":
            red_cells.append(f'SQ_{chr(ord("A")+file)}{(rank+1):1}')
        elif cell == "b":
            yellow_cells.append(f'SQ_{chr(ord("A")+file)}{(rank+1):1}')
            
print('Red : (', '|'.join(red_cells), sep='', end=')\n')
print('Yellow : (', '|'.join(yellow_cells), sep='', end=')\n')

