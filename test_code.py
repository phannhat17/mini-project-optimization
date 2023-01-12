def read_input(file_path):
    '''
        Return number of packs (n_packs); number of bins (n_bins); 
        and two list of pack's width and height; bin's width, height and cost.

        file_path: path to file containing the input data
    '''
    with open(file_path) as f:
        data = f.readlines()
        n_packs, n_bins = map(int, data[0].split())
        packs, bins = [], []

        for i in range(1, n_packs+1):
            packs.append(tuple(map(int, data[i].split())))

        for i in range(n_packs+1, len(data)):
            bins.append(tuple(map(int, data[i].split())))

    return n_packs, n_bins, packs, bins


if __name__ == "__main__":
    n_packs, n_bins, packs, bins = read_input('./input_data/15.txt')
    print()
    print(f'Number of packs: {n_packs}')
    print(packs)
    print()
    print(f'Number of bins: {n_bins}')
    print(bins)
    print()
