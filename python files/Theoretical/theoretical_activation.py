import numpy as np
import pandas as pd
import matplotlib as plt
import json


dir = 'data/'
mb = 1e-3 * 1e-24  # millibarns in cm2

half_life = {'Be7':4590000, 'C10':19.29, 'C11':1220.00, 'O14':70.60, 'O15':122.24}
hour = 3600
beam_time = 60
MeV = 1e-3 # in GeV


def λ(key):
    return np.log(2)/half_life[key]


C12 = {
     70:{'Be7':21*mb,'C10':5*mb,'C11':75*mb,'O14':0*mb,'O15':0},
    100:{'Be7':17*mb,'C10':5*mb,'C11':62*mb,'O14':0*mb,'O15':0},
    150:{'Be7':14*mb,'C10':5*mb,'C11':45*mb,'O14':0*mb,'O15':0},
    200:{'Be7':10*mb,'C10':4*mb,'C11':41*mb,'O14':0*mb,'O15':0},
    230:{'Be7':10*mb,'C10':4*mb,'C11':38*mb,'O14':0*mb,'O15':0},
    250:{'Be7':10*mb,'C10':4*mb,'C11':37*mb,'O14':0*mb,'O15':0}
}

O16 = {
     70:{'Be7':9*mb, 'C10':4*mb,'C11':18*mb,'O14':1*mb,'O15':71*mb},
    100:{'Be7':6*mb, 'C10':6*mb,'C11':15*mb,'O14':1*mb,'O15':61*mb},
    150:{'Be7':8*mb, 'C10':2*mb,'C11':11*mb,'O14':1*mb,'O15':42*mb},
    200:{'Be7':8*mb, 'C10':2*mb,'C11':11*mb,'O14':1*mb,'O15':41*mb},
    230:{'Be7':8*mb, 'C10':2*mb,'C11':10*mb,'O14':1*mb,'O15':40*mb},
    250:{'Be7':8*mb, 'C10':2*mb,'C11':10*mb,'O14':1*mb,'O15':39*mb}
}

elemFraction = {'C12':0.41, 'O16':0.13}  # elemental fractions of Carbon-12 and Oxygen-16
n = 9.7e22                               # number density of target

isotopes = half_life.keys()

thickness = [1]
energies = [70,100,150,200,230,250]
N = 1e11

rows = []

i = 0
for e in energies:
    for t in thickness:

        iso_act = {}
        for iso in isotopes:
            iso_act[iso] = {
            "halfLife":half_life[iso],
            "lifeTime":1/λ(iso),
            "number":int(np.sum(
               [ elemFraction['C12'] * N * C12[e][iso] * n * t,
                 elemFraction['O16'] * N * O16[e][iso] * n * t
               ]))
              }

            dict = {
                'run':i,
                'nEvents':int(1e11),
                'energy':e * MeV,
                'rangeshifterThickness':t,
                'physicsList':"Theoretical",
                'isotopes':iso_act
                }

        rows.append(dict)
        i += 1

with open("theoretical.json", 'w') as f:
    f.write(json.dumps(rows, indent=2))
