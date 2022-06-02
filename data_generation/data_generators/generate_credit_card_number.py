from faker import Faker
import csv


fake = Faker()

with open('credit_Cards.csv' , 'w' , newline='') as csvfile:
    writer = csv.writer(csvfile , delimiter='\t')
    for i in range(50000):
        writer.writerow([i , fake.credit_card_number(card_type='visa16')])
    for i in range(50000):
        writer.writerow([i+50000 ,fake.credit_card_number(card_type='mastercard')])

