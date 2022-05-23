import os
import csv

values_Np_Nc=[1,2,4,8]
sizeN=[1,2,4,8,16,32]
repeticoes = 10

def Execution_Semafore(repeticoes, values_Np_Nc, sizeN):

    data_file = './Results.csv'
    data_file = os.path.join(os.path.dirname(__file__), data_file)

    if os.path.exists(data_file):
        os.remove(data_file)

    with open(data_file, 'w') as f:
        writer = csv.writer(f)
        writer.writerow(["time", "Np", "Nc", "N"])

    f.close();

    for j in range(repeticoes):
        for N in sizeN:
            for value_th in values_Np_Nc:
                command = "./Semaforo "+str(N)+" "+str(value_th)+" 1"
                os.system(command)
            for i in range(1,len(values_Np_Nc)):
                command = "./Semaforo "+str(N)+" 1 "+str(values_Np_Nc[i])
                os.system(command)

Execution_Semafore(repeticoes, values_Np_Nc, sizeN)