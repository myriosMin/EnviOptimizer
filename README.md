# 🌍 Envi-Optimizer

**An IoT and Machine Learning powered environmental monitoring system** designed to optimize temperature, humidity, and air quality on manufacturing shop floors for safety, efficiency, and predictive insights.

---

## 🏢 Project Overview

Modern factories generate significant heat, dust, and fluctuating environmental conditions. **Envi-Optimizer** addresses these challenges by:

- Monitoring critical environmental factors (temp, humidity, air quality)
- Storing and analyzing sensor data using a full-stack data pipeline
- Predicting anomalies and identifying optimal indoor conditions
- Presenting insights via a Power BI dashboard for decision-makers

---

## 🧱 System Architecture

```text
[Sensors (Arduino)]
   ⇓   MQTT (every 10 min)
[Node.js Server]
   ⇓   SQL INSERT Statements
[MySQL Database (mydb1)]
   ⇓   Python (Colab)
   ⇓   Power BI (via ODBC)
```

---

## 🤖 Features

- Real-time data collection via Arduino + MQTT
- Cloudless local MySQL database storage
- Weather data scraping with Python (BeautifulSoup)
- KNN clustering for optimal condition identification
- RNN anomaly detection for system alerts
- Dashboard with slicers for interactive insights
- 3D-printed Arduino housing for durability

---

## 🔧 Tools & Technologies

| Category      | Tools Used                            |
| ------------- | ------------------------------------- |
| Hardware      | Arduino R4, Temp/Humidity/Air Sensors |
| Backend       | C++ (Arduino IDE), Node.js            |
| Data Storage  | MySQL (local, table: `t2g2`)          |
| Web Scraping  | Python, BeautifulSoup                 |
| ML/Analysis   | Google Colab, KNN, RNN                |
| Dashboard     | Power BI via ODBC                     |
| Communication | MQTT (Mosquitto Broker)               |

---

## 🔬 Results Summary

- **Temperature**: Weekends saw extreme peaks due to inactive AC
- **Humidity**: High variance on Mondays/Sundays
- **Air Quality**: Peaks during machine usage; improves after rainfall

### 🎯 ML Findings

- **Optimal Cluster**: 24°C, 64% humidity, AQI \~42
- **RNN** detected underperforming humidity control (recommend dehumidifier)

---

## 📄 Reports

Full findings with details can be read in a report format as well as a presentation format. 

---

## 🚀 Recommendations

- Install **dehumidifiers** for better humidity control
- Use **low-capacity AC** on weekends to stabilize temperature
- Deploy **air filtration units** near machinery
- Coat shutter doors with **thermo-resistant polymer** to reduce heat spikes

---

## 🙌 Contributors

[Year 2, Data Engineering Project, Diploma in AI & Data Engineering, Nanyang Polytechnic]
- [**Min Phyo Thura**](https://github.com/myriosMin) (Team Lead, Software)
- [Lim Jin Bin](https://github.com/LimJinBin32) (Dashboard, Report)
- [Alexander Chan](https://github.com/Redbeanchan) (Hardware, Report)  
- [Mohammad Habib](https://github.com/habibmohammad35) (Hardware, Report)

---

## 💎 License

MIT License with Common Clause. Use freely for learning or inspiration! No commercial use is allowed. 

---

Thanks for checking out Envi-Optimizer! Feel free to fork or reach out for collaborations 💪
