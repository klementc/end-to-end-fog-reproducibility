
import time

start = time.time()
for i in range (20000000):
  print("",end="")
end = time.time()

print("start: "+str(0)+" end: "+str(end-start)+" tot: "+str(end-start))