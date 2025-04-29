'''another variation of scrap.py'''
# brute-forcing by copy-pasting html when direct link doesn't work as expected

from bs4 import BeautifulSoup
import pandas as pd

# load the HTML file
with open('/Users/myrios/Desktop/NYP/Year 2/weather.html', 'r') as file:
    html_content = file.read()

# parse for BS to read
soup = BeautifulSoup(html_content, 'html.parser')

# find the table
table = soup.find('table')

# get table headers
headers = []
for th in table.find('thead').find_all('th'):
    headers.append(th.text.strip())

# get rows
rows = []
for tr in table.find('tbody').find_all('tr'):
    cells = tr.find_all('td')
    row = [cell.text.strip() for cell in cells]
    rows.append(row)

# convert to df and save to csv
df = pd.DataFrame(rows, columns=headers)
csv_path = '/Users/myrios/Desktop/NYP/Year 2/dep_data/AMK_Weather.csv'
df.to_csv(csv_path, index=False)
print('CSV saved successfully.')