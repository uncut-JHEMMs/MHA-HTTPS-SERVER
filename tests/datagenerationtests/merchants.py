
import requests
import csv

url = "https://localhost:8443/addMerchant"

with open("../../data_generation/final_data/merchants_table.csv" , 'r' , newline = '') as csvfile:
    reader = csv.reader(csvfile)
    iterations = 0
    for row in reader:
        row = row[0].split('\t')
        response = requests.post(url , data = {"id": row[0] , "merchant_name":row[1] , "type":row[2]} , verify=False)
        iterations += 1
        if iterations == 50:
            break
