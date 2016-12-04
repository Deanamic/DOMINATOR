import os
import sys
import random
import time
import subprocess

random.seed(time.time())
todaysdate = time.strftime("%d.%m.%Y %H:%M")
punt = [0,0,0,0]
guany = [0,0,0,0]
os.makedirs("Matches/Match "+sys.argv[1]+" vs "+sys.argv[2]+" "+str(todaysdate))
for i in range(0,5):
  output = subprocess.Popen(["./GameMac", sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], "-s", str(random.randrange(10000)), \
    "-i", "default.cnf", "-o", "Matches/Match "+sys.argv[1]+" vs "+sys.argv[2]+" "+str(todaysdate)+"/Match Number"+str(i)+".res", "-d"], stdout=subprocess.PIPE).communicate()[0]
  llista = eval(output)
  maxim = 0
  jugador = 1
  for j in range(0,4):
    punt[j] += llista[j]
    if maxim < llista[j]:
        jugador = j
        maxim = llista[j]
  guany[jugador] += 1
  sys.stdout.write("-")
  sys.stdout.flush()

print ("")
for i in range(0,4):
  print("Jugador "+str(i)+": " + str(punt[i]//5) + " " + str(guany[i]))
