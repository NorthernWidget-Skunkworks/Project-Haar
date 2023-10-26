#!/usr/bin/env python
# coding: utf-8

# In[2]:


import pandas as pd
data = pd.read_csv('Data.csv') #Read in data from CSV
data_trimmed = data[data['Time [ms]'] > 18000] #Trim data to points around pressure release 
data_trimmed = data_trimmed[data_trimmed['Time [ms]'] < 24000]
data_trimmed['Time [ms]'] = data_trimmed['Time [ms]'] - min(data_trimmed['Time [ms]']) #Make time relative 
print(data_trimmed)


# In[3]:


ax = data_trimmed.plot(kind = 'line', x = 'Time [ms]', y = ['PressureA [mBar]','PressureB [mBar]'])

ax.set_ylabel("Pressure [mBar]")
ax.figure.savefig('PresureResponse.pdf')
ax.figure.savefig('PresureResponse.png')


# In[ ]:




