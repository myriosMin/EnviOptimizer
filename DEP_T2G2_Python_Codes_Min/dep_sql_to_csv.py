import mysql.connector
import pandas as pd

# database credentials
db_config = {
    'user': 'egt209',
    'password': 'egt209',
    'host': '192.168.0.101',
    'database': 'mydb'
}

# query
sql_query = "SELECT * FROM t2g2"

def export_to_csv(db_config, sql_query, csv_file_path):
    '''mysql table to csv connector'''
    try:
        # start a connection
        connection = mysql.connector.connect(**db_config)

        # fetch data using pandas
        df = pd.read_sql(sql_query, connection)

        # export data to CSV
        df.to_csv(csv_file_path, index=False)
        print(f"Data successfully exported to {csv_file_path}")

    except mysql.connector.Error as err:
        # connection error
        print(f"Error: {err}")

    finally:
        if connection.is_connected():
            # close the connection after exporting data
            connection.close()
            print("MySQL connection closed.")

# path to save
csv_file_path = '/Users/myrios/Desktop/NYP/Year 2/dep_data/week16_aug3.csv'

export_to_csv(db_config, sql_query, csv_file_path)