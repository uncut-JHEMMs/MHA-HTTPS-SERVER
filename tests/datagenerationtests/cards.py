import requests
import csv
from xml.etree import ElementTree
  
url = "https://localhost:8443/addCard"
with open("../../data_generation/final_data/credit_cards_table.csv" , 'r' , newline = '') as csvfile: 
	
    reader = csv.reader(csvfile)    
    iterations = 0
    for row in reader:
        row = row[0].split('\t')
        response = requests.post(url , data = {"id": row[0] ,"card_type":row[1] , "card_number":row[2]} , verify=False)

        tree = ElementTree.fromstring(response.content)
        print(tree)
        iterations += 1
        if iterations == 50:
            break

