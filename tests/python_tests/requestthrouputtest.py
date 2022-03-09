import requests
from datetime import datetime , timedelta
import keyboard , platform , os

url = "https://localhost:8443/hello"
#response = requests.get(url , verify=False)
#print(response.text)
print("Time      Response")
passed = 0
failures = 0
start_time = datetime.now()
while(True):
    response = requests.get(url , verify=False)
    if (response != None):
        passed += 1
        now = datetime.now()
        print(now.strftime("%H:%M:%S") + "     " + response.text)
    else:
        failures +=1
    if keyboard.is_pressed("q"):
        break

elapsed = datetime.now() - start_time


print("\n\n")
print("PlatForm Version" , platform.version())
print("PlatForm Machine" , platform.machine())
print("PlatForm Name " , platform.uname())
print("PlatForm System" , platform.system())
print("PlatForm Processor" , platform.processor())
print("\n\nCPU USAGE")  
os.system("top -p $(pidof net.out) -b  -d1 -n1 | grep -i "'Cpu'" |head -c21|cut -d ' ' -f3|cut -d '%' -f1 > file.csv")
print(os.system("cat file.csv"))
print("\n\n")
print("passed           failures            reqs/sec            failures/s")
print(str(passed) + "       " + "           " + str(failures) + "               " + str(passed + failures / elapsed.seconds) + "               " + str(failures/elapsed.seconds))


