import os
import sys
import random
import time
import subprocess

random.seed(time.time())

punt = [0,0,0,0]
n = eval(sys.argv[2])

for i in range(0,n):
  output = subprocess.Popen(["./Game", sys.argv[1], "Dummy", "Dummy", "Dummy", "-s", str(random.randrange(10000)), \
    "-i", "default.cnf", "-o", "default.res", "-d"], stdout=subprocess.PIPE).communicate()[0]
  llista = eval(output)
  for j in range(0,4):
    punt[j] += llista[j]
  sys.stdout.write("-")
  sys.stdout.flush()

print ""
for i in range(0,4):
  print("Jugador "+str(i)+": " + str(punt[i]//n))

