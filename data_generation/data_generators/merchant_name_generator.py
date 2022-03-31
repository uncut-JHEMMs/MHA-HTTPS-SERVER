from faker import Faker
import csv

fake = Faker()

with open('../merchants.csv' , 'r' , newline='') as reading, \
    open('merchants.csv' , 'w' , newline='') as writing:
        csv_reader = csv.reader(reading)
        csv_writer = csv.writer(writing , delimiter='\t')

        for row in csv_reader:
            row.append(fake.company_suffix())

            csv_writer.writerow(row)
