import random as rd

def genData(filename, N, K, MAX_SZ_C, MAX_SZ_I, MAXC):
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
        


for i in range(5, 55):
    genData(f'input_data/{i}.txt', i, i*2, 20, 10, 100)

for i in range(60, 331, 30):
    genData(f'input_data/{i}.txt', i, i*2, 20, 10, 100)

for i in range(350, 801, 50):
    genData(f'input_data/{i}.txt', i, i*2, 20, 10, 100)
for i in range(850, 1001, 50):
    genData(f'input_data/{i}.txt', i, i*2, 20, 10, 100)











