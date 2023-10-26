#!/usr/bin/env python
# coding: utf-8

# In[13]:


import pandas as pd
data = pd.read_csv('TempTest2.csv', skiprows = 2, encoding='ISO-8859-1') #Read in data from CSV
data.columns = ["Time","TempA","PresA","RHA","TempB","PresB","RHB"]
data["Time"] = data["Time"]/1000 #Convert to seconds
print(data)


# In[14]:


ax = data.plot(kind = 'line', x = 'Time', y = ['TempA','TempB'], title='Temperature Response Time')
ax.set_ylabel("Temperature [°C]")
ax.get_figure().savefig('TempResponse.pdf')


# In[15]:


from array import array
maxVal = max(data['TempA'])
error = array("f", abs(data['TempA'] - maxVal))


tau = data['Time'][error.index(min(error))]
print((tau)/60) #Print tau result in minutes 

