import random as rd

def genData(filename, N, K, MAX_SZ_C, MAX_SZ_I, MAXC):
    '''
    Random generate data:
        filename: the name of the output file
        N: the number of packs
        K: the number of bins
        MAX_SZ_C: the maximum size of bins
        MAX_SZ_I: the maximum size of packs
        MAXC: the maximum cost of each bin

    '''
    f = open(filename, 'w')
    f.write(str(N) + " " + str(K) + '\n')
    w = [0 for i in range(N)]
    l = [0 for i in range(N)]
    W = [0 for i in range(K)]
    L = [0 for i in range(K)]
    c = [0 for i in range(K)]

    for i in range(N):
        w[i] = rd.randint(1, MAX_SZ_I)
        l[i] = rd.randint(1, MAX_SZ_I)
    
    # The minimum size of each bin always larger than the maximum size of packages
    maxW = max(w)
    maxL = max(l)

    for k in range(K):
        W[k] = rd.randint(maxW, MAX_SZ_C)
        L[k] = rd.randint(maxL, MAX_SZ_C)
        c[k] = rd.randint(MAXC//2, MAXC)

    for i in range(N):
        f.write(str(w[i]) + ' ' + str(l[i]) +'\n')
    for k in range(K):
        f.write(str(W[k]) + ' ' + str(L[k]) + ' '  + str(c[k]) +'\n')
        

# Here we set: 
MAX_SZ_C = 20
MAX_SZ_I = 10
MAXC = 100

for i in range(10, 55):
    genData(f'input_data/00{i}.txt', i, i//3, MAX_SZ_C, MAX_SZ_I, MAXC)

for i in range(90, 351, 30):
    if i < 100:
        genData(f'input_data/00{i}.txt', i, i//3, MAX_SZ_C, MAX_SZ_I, MAXC)
    else:
        genData(f'input_data/0{i}.txt', i, i//3, MAX_SZ_C, MAX_SZ_I, MAXC)

for i in range(550, 1001, 50):
    if i == 1000:
        genData(f'input_data/{i}.txt', i, i//3, MAX_SZ_C, MAX_SZ_I, MAXC)
    else:
        genData(f'input_data/0{i}.txt', i, i//3, MAX_SZ_C, MAX_SZ_I, MAXC)

for i in range(2000, 10001, 1000):
    genData(f'input_data/{i}.txt', i, i//3, MAX_SZ_C, MAX_SZ_I, MAXC)











