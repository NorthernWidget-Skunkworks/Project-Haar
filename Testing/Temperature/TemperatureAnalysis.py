#!/usr/bin/env python
# coding: utf-8

# In[56]:


import pandas as pd
import matplotlib.pyplot as plt
from matplotlib import style
data = pd.read_csv('Data.csv', skiprows = 1, encoding='ISO-8859-1') #Read in data from CSV
data.columns = ["Time","TempCtrl","RHCtrl","PresA","TempA","RHA","PresB","TempB","RHB"]
# data["Time"] = data["Time"]/1000 #Convert to seconds
print(data)


# In[57]:


style.use('default')
styles = ['-','--',':']
data.plot(kind = 'line', x = 'Time', y = ['TempCtrl','TempA','TempB'], style=styles, title='Temperature Response Time',xlim=(1000,5000),figsize=(7.5,3.5), grid=True)
plt.ylabel("Temperature [°C]")
plt.xlabel("Time [s]")
plt.legend(["Control", "Open","Closed"], loc='lower right')
plt.tight_layout()
plt.savefig('TempResponse.pdf', bbox_inches="tight")
plt.savefig('TempResponse.png',format='png', dpi=300)


