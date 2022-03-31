import requests
import csv

url = "https://localhost:8443/addUser"

uid = {"user_name": "Huzaifa" , "id":"12"}

with open("../../data_generation/data.csv" , 'r' , newline = '') as csvfile:
    reader = csv.reader(csvfile)
    iterations = 0
    for row in reader:
        row = row[0].split('\t')
        response = requests.post(url , data = {"id": row[0] , "first__name":row[1] , "last_name":row[2] , "email":row[3] } , verify=False)
        iterations += 1
        if iterations == 50:
            break

