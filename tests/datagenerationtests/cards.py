import requests
import csv
  
url = "https://localhost:8443/addCard"
with open("../../data_generation/credit_Cards.csv" , 'r' , newline = '') as csvfile: 
	
    reader = csv.reader(csvfile)    
    iterations = 0
    for row in reader:
        row = row[0].split('\t')
        response = requests.post(url , data = {"id": row[0] , "card_number":row[1]} , verify=False)
        iterations += 1
        if iterations == 50:
            break

