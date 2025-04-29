'''this program need fine adjustment for each run as the website to scrap has a weird html'''

import requests
from bs4 import BeautifulSoup
import pandas as pd
from datetime import datetime, timedelta

# URL
base_url = "https://www.timeanddate.com/weather/@1882560/historic"

# one day at a time
def extract_weather_data(date):
    formatted_date = date.strftime('%Y-%m-%d')
    url = f"{base_url}?month={date.month}&year={date.year}&hd={formatted_date.replace('-', '')}"
    response = requests.get(url)
    soup = BeautifulSoup(response.content, 'html.parser')

    # find the correct table 
    table = soup.find('table', {'id': 'wt-his'})
    
    if not table:
        return None

    # extract data
    rows = table.find_all('tr')
    data = []
    for row in rows[1:]:
        cols = row.find_all('td')
        if len(cols) > 1:  #avoid headers
            time = cols[0].text.strip()
            temperature = cols[1].text.strip()
            weather = cols[2].text.strip()
            wind = cols[3].text.strip()
            humidity = cols[4].text.strip()
            barometer = cols[5].text.strip()
            visibility = cols[6].text.strip() if len(cols) > 6 else 'N/A'
            data.append([formatted_date, time, temperature, weather, wind, humidity, barometer, visibility])
    return data

# collect data for july
all_data = []
for day in range(31):
    date = datetime.now() - timedelta(days=day)
    daily_data = extract_weather_data(date)
    if daily_data:
        all_data.extend(daily_data)

# convert to a df
columns = ['Date', 'Time', 'Temperature', 'Weather', 'Wind', 'Humidity', 'Barometer', 'Visibility']
weather_df = pd.DataFrame(all_data, columns=columns)

# save to csv
file_path = '/Users/myrios/Desktop/NYP/Year 2/dep_data/AMK_weather_Jul20.csv'
weather_df.to_csv(file_path, index=False)

print(f"Data saved to {file_path}")