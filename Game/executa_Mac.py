import os
import sys
import random
import time
import subprocess

random.seed(time.time())

punt = [0,0,0,0]
guany = [[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]
n = eval(sys.argv[5])

for i in range(0,n):
  output = subprocess.Popen(["./GameMac", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], "-s", str(random.randrange(10000)), \
    "-i", "default.cnf", "-o", "default.res", "-d"], stdout=subprocess.PIPE).communicate()[0]
  llista = eval(output)

  used = [0,0,0,0]

  maxim = 0
  jugador = 1
  for j in range(0,4):
    punt[j] += llista[j]
    if maxim < llista[j]:
        jugador = j
        maxim = llista[j]
  guany[0][jugador] += 1
  used[jugador] = 1

  maxim = 0
  jugador = 1
  for j in range(0,4):
    if maxim < llista[j] and used[j] == 0:
        jugador = j
        maxim = llista[j]
  guany[1][jugador] += 1
  used[jugador] = 1

  maxim = 0
  jugador = 1
  for j in range(0,4):
    if maxim < llista[j] and used[j] == 0:
        jugador = j
        maxim = llista[j]
  guany[2][jugador] += 1
  used[jugador] = 1

  maxim = 0
  jugador = 1
  for j in range(0,4):
    if maxim < llista[j] and used[j] == 0:
        jugador = j
        maxim = llista[j]
  guany[3][jugador] += 1
  used[jugador] = 1

  sys.stdout.write("-")
  sys.stdout.flush()

print ("")

nom = ["","","",""]
mitja = [0.0,0.0,0.0,0.0]
for i in range(0,4):
    nom[i] = sys.argv[i + 1]
    while len(nom[i]) < 12:
        nom[i] += str(" ")
    for j in range(0,4):
        mitja[i] += float(j + 1)*float(guany[j][i])/float(n)

for i in range(0,4):
  print("Jugador "+str(i + 1) + " " + nom[i] + ": " + str(punt[i]//n) + " " + str(guany[0][i]) + " "\
      + str(guany[1][i]) + " " + str(guany[2][i]) + " " + str(guany[3][i]) + " %.2f"  % (mitja[i]))
