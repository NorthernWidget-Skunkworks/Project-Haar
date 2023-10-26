#!/usr/bin/env python
# coding: utf-8

# In[15]:


import pandas as pd
data = pd.read_csv('RHTest2.csv', skiprows = 2, encoding='ISO-8859-1') #Read in data from CSV
data.columns = ["Time","TempA","PresA","RHA","TempB","PresB","RHB"]
data["Time"] = data["Time"]/1000 #Convert to seconds
print(data)


# In[20]:


ax = data.plot(kind = 'line', x = 'Time', y = ['RHA','RHB'], title='Humidity Response Time')
ax.set_ylabel("Relative Humidity [%]")
ax.get_figure().savefig('HumidityResponse.pdf')
ax.get_figure().savefig('HumidityResponse.png')


# In[17]:


from array import array
maxVal = max(data['RHA'])
error = array("f", abs(data['RHA'] - maxVal))


tau = data['Time'][error.index(min(error))]
print((tau)/60) #Print tau result in minutes 

