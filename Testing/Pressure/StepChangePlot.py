#!/usr/bin/env python
# coding: utf-8

# In[29]:


import pandas as pd
data = pd.read_csv('StepChangeData.csv') #Read in data from CSV
data_trimmed = data[data['Time [ms]'] > 18000] #Trim data to points around pressure release 
data_trimmed = data_trimmed[data_trimmed['Time [ms]'] < 24000]
data_trimmed['Time [ms]'] = data_trimmed['Time [ms]'] - min(data_trimmed['Time [ms]']) #Make time relative 
print(data_trimmed)


# In[31]:


ax = data_trimmed.plot(kind = 'line', x = 'Time [ms]', y = ['PressureA [mBar]','PressureB [mBar]'])

ax.set_ylabel("Pressure [mBar]")
# plt.savefig('StepChangePlot.png', format='png', dpi=300)
ax.figure.savefig('StepChangePlot.pdf')

