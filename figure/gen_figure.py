import matplotlib.pyplot as plt
import random, sys
colors = ['red', 'green', 'blue', 'purple', 'orange', 'pink', 'brown', 'cyan', 'magenta']

def read_input(file_path):
    '''
        Return number of packs (n_packs); number of bins (n_bins); 
        and two tuple of pack's width and height; bin's width, height and cost.

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

def get_data(input_path, output_path):
    n_packs, n_bins, packs, bins = read_input(input_path)
    with open(output_path, 'r') as f:
        lines = f.readlines()
        used_bins = {}
        coordinates = {}

        for line in lines:
            pack = int(line.split('pack ')[1].split()[0])
            bin_use = int(line.split('in bin ')[1][0])
            if bin_use not in used_bins:
                used_bins[bin_use] = [pack]
            else:
                used_bins[bin_use].append(pack)
            x, y = map(int, line.split('(x, y) is ')[1].replace('(', '').replace(')', '').split(','))
            if 'Rotate' in line:
                packs[pack-1] = (packs[pack-1][1], packs[pack-1][0])
                coordinates[pack] = [int(x)-packs[pack-1][0], int(y)-packs[pack-1][1]]
            else:
                coordinates[pack] = [int(x)-packs[pack-1][0], int(y)-packs[pack-1][1]]
    return bins, packs, used_bins, coordinates

def draw_rect(ax, x, y, width, height, color):
    ax.add_patch(plt.Rectangle((x,y), width, height, fill=True, edgecolor='black', linewidth=2, facecolor=color))
 

if __name__ == '__main__':
    input_path = sys.argv[1]
    output_path = sys.argv[2]
    bins, packs, used_bins, coordinates = get_data(input_path, output_path)

    save_to_path = sys.argv[3]

    for _bin in used_bins:
        fig, ax = plt.subplots(figsize=(10, 10))

        # Draw the bin
        draw_rect(ax, 0, 0, bins[_bin-1][0], bins[_bin-1][1], 'gray')

        # Draw the packs inside the bin
        for pack in used_bins[_bin]:
            color = random.choice(colors)

            draw_rect(ax, coordinates[pack][0], coordinates[pack][1], packs[pack-1][0], packs[pack-1][1], color)

            ax.annotate(pack, (coordinates[pack][0]+packs[pack-1][0]/2, coordinates[pack][1] + packs[pack-1][1]/2), color='black', weight='bold', fontsize=10, ha='center', va='center')



        ax.set_xlim([0, bins[_bin-1][0]])
        ax.set_ylim([0, bins[_bin-1][1]])
        plt.axis('on')
        plt.savefig(f'{save_to_path}/bin_{_bin}.png')

