import os

energies = [0.250,0.230,0.200,0.150,0.100,0.07]  # MeV
spread   = [0.1020,0.0979,0.0913,0.0790,0.0645,0.0540]
thickness = [1.,2.,3.,5.]   # cm


for i, e in enumerate(energies):
    for t in thickness:
        with open("template.txt", 'r') as templatef:
            directory = "/{rThickness}-{bEnergy:03}/".format(rThickness=round(t), bEnergy=round(1000*e))
            os.makedirs(os.getcwd() + directory)
            with open(os.getcwd() + directory + "in.inp", 'w+') as f:
                for line in templatef:
                    line = line.replace("$rThickness$",str(t))
                    line = line.replace("$bEnergy$","{:.6f}".format(e))
                    line = line.replace("$bEnergySpread$","{:.6f}".format(spread[i]))
                    print(line, file=f, end='')
