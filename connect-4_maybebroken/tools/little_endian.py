NUM_ENTRIES = 4200899

with open('./out/opening_book_be.dat', 'rb') as rf:
    all_bytes = rf.read() # read all

with open('./out/opening_book_le.dat', 'wb') as wf:

    # Big to little-endian for board representation
    for index in range(NUM_ENTRIES): # number of entries in file
        wf.write(all_bytes[(5*index + 3) : (5*index + 4)]) # LSByte
        wf.write(all_bytes[(5*index + 2) : (5*index + 3)]) # next byte
        wf.write(all_bytes[(5*index + 1) : (5*index + 2)]) # next byte
        wf.write(all_bytes[(5*index)     : (5*index + 1)]) # MSByte
    
    for index in range(NUM_ENTRIES):
        wf.write(all_bytes[(5*index + 4) : (5*index + 5)]) # value
