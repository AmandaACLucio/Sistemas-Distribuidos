import os

values_Np_Nc=[1,2,4,8]
sizeN=[1,2,4,8,16,32]
rep = 10

for j in range(rep):
    for N in sizeN:
        for value_th in values_Np_Nc:
            command = "./Semaforo "+str(N)+" "+str(value_th)+" 1"
            os.system(command)
        for i in range(1,len(values_Np_Nc)):
            command = "./Semaforo "+str(N)+" 1 "+str(values_Np_Nc[i])
            os.system(command)