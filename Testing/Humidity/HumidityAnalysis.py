#!/usr/bin/env python
# coding: utf-8

# In[21]:


import pandas as pd
import matplotlib.pyplot as plt
from matplotlib import style
data = pd.read_csv('Data.csv', skiprows = 2, encoding='ISO-8859-1') #Read in data from CSV
# data.columns = ["Time","TempA","PresA","RHA","TempB","PresB","RHB"]
data.columns = ["Time","TempCtrl","RHCtrl","PresA","TempA","RHA","PresB","TempB","RHB"]
data["Time"] = data["Time"] #Convert to seconds
print(data)


# In[22]:


style.use('default')
styles = ['-','--',':']
data.plot(kind = 'line', x = 'Time', y = ['RHCtrl','RHA','RHB'], style=styles, title='Humidity Response Time',figsize=(7.5,3.5),xlim=(2000,10000), grid=True)
plt.ylabel("Relative Humidity [%]")
plt.xlabel("Time [s]")
plt.legend(["Control", "Open","Closed"], loc='lower right')
plt.tight_layout()
plt.savefig('HumidityResponse.pdf', bbox_inches="tight")
plt.savefig('HumidityResponse.png',format='png',dpi=300)


