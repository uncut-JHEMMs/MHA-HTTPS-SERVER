from faker import Faker
import csv


fake = Faker()


with open('data.csv', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile, delimiter='\t')
    for i in range(100000):
        fname = fake.first_name()
        lname = fake.last_name()
        writer.writerow([i , fname, lname , fname+lname+"@smoothstack.com" ])
    






