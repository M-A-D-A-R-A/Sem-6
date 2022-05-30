

# %% [markdown]
# 1. Compute total patient attendance for all district for all four range group and plot the bar diagram. Set the bar plot parameters for better visualization. 

# %%
# importing library

import sys
import pandas as p
import seaborn as sns
import numpy as np
import matplotlib.pyplot as plt
from statsmodels.graphics.gofplots import qqplot

# %%
# Reading csv files
patient_data = p.read_csv("JK-Allopathic-Outpatient_attendance-May-2019.csv")
patient_data.head()

# %%
df = patient_data[['District','No. of facilities by performance - 1 to 100','No. of facilities by performance - 101 to 500','No. of facilities by performance - 501 to 1000','No. of facilities by performance - >1000']].copy()
df.head()

# %%
# Adding the data on basis of the all four range group for each state
data = df.groupby(['District'], as_index = False).sum()
data.head()

# %% [markdown]
# 1. Compute total patient attendance for all district for all four range group and plot the bar diagram. Set the bar plot parameters for better visualization. 

# %%
x = data['District']
y1 = data['No. of facilities by performance - 1 to 100']
y2 = data['No. of facilities by performance - 101 to 500']
y3 = data['No. of facilities by performance - 501 to 1000']
y4 = data['No. of facilities by performance - >1000']

X_axis = np.arange(len(x))
plt.subplots(figsize=(30,10))
plt.bar(X_axis - 0.3, y1, 0.2, label = '1 to 100')
plt.bar(X_axis - 0.1, y2, 0.2, label = '101 to 500')
plt.bar(X_axis + 0.1, y3, 0.2, label = '501 to 1000')
plt.bar(X_axis + 0.3, y4, 0.2, label = '>1000')

plt.xticks(X_axis,x)
plt.xlabel("Districts")
plt.ylabel("patients")

plt.legend()
plt.show()

# %%
df = patient_data

# %%
df.drop(['District'],axis = 1)

# %%
data= df[['Facility Type','No. of facilities by performance - 1 to 100','No. of facilities by performance - 101 to 500','No. of facilities by performance - 501 to 1000','No. of facilities by performance - >1000']].copy()
data

# %%
df = data.groupby(['Facility Type'], as_index = False).sum()
data = df

# %% [markdown]
# 2. Compute total patient attendance for all district for each Facility Type (DH, CHC and SC) for all four range groups and plot the staked bar diagram of three. Set the bar plot parameters for better visualization. 

# %%
x = data['Facility Type']
y1 = data['No. of facilities by performance - 1 to 100']
y2 = data['No. of facilities by performance - 101 to 500']
y3 = data['No. of facilities by performance - 501 to 1000']
y4 = data['No. of facilities by performance - >1000']

X_axis = np.arange(len(x))
plt.subplots(figsize=(6,6))
plt.bar(X_axis , y1, 0.5, label = '1 to 100')
plt.bar(X_axis, y2, 0.5, label = '101 to 500')
plt.bar(X_axis, y3, 0.5, label = '501 to 1000')
plt.bar(X_axis, y4, 0.5, label = '>1000')

plt.xticks(X_axis,x)
plt.xlabel("Facility Type")
plt.ylabel("patients")

plt.legend()
plt.show()

# %%
df =patient_data
idx = df["District"].isin(["Anantnag","Jammu","Poonch","Reasi","Udhampur"])
df = df[idx]

# %%
data= df[['District','Facility Type','Performance - Overall Average **']].copy()
data

# %% [markdown]
# 4. Plot group bar plot for Performance - Overall Average of different Facility Type (DH, CHC and SC) of Anantnag, Jammu, Poonch, Reasi and Udhampur. 

# %%
ax=sns.barplot(x="Performance - Overall Average **",
           y="District",
           hue="Facility Type",
           data=data)

ax.set_xscale("log")


# %% [markdown]
# 4. Present dot plot for Performance - Maximum of any 20 different district. Performance - Maxi mum for different Facility Type should be combined appropriately using a aggregation function for each district.

# %%
df = patient_data
df.District.unique()

# %%
# ploting the dot plot
df=df[patient_data['District'].isin(['Anantnag', 'Badgam', 'Bandipora', 'Baramula', 'Doda', 'Ganderbal',
       'Jammu', 'Kargil', 'Kathua', 'Kishtwar', 'Kulgam', 'Kupwara',
       'Leh Ladakh', 'Poonch', 'Pulwama', 'Rajouri', 'Ramban', 'Reasi',
       'Samba', 'Shopian'])].groupby("District").sum()
df.reset_index(level=0,inplace=True)
plt.scatter(x=df['Performance - Maximum'],y=df['District'])
plt.xlabel("Performance Maximum")
plt.ylabel("District")

# %% [markdown]
# ## Fifa player profile 

# %%
# reading the data 
football_data = p.read_csv('Fifa_player_football_data.csv')
football_data.head()

# %%
# storing it as temporary variable
df = football_data['Age']
df

# %%
# Setting the ranges
bins = [15,20,25,30,35,40,45,50]
labels= ['15-20','21-25','26-30','31-35','36-40','41-45','46-50']
p.cut(df,bins=bins,labels=labels,right=False)

# %% [markdown]
# 1. Present Age of various football players as histogram and kernel density plots. Set appropriate parameters of the plot. 

# %%
# plotting the graph-histogram
plt.hist(df)
plt.show()

# %%
# plotting the kernel density graph
sns.kdeplot(data=df)

# %% [markdown]
# 2. Present Age of various Football players as Kernel Density plots for each of FC Barcelona, Chelsea, Juventus and Real Madrid Clubs. Set appropriate parameters of the plot. 

# %%
# getting data for all the clubs mentioned in question.
idx=football_data["Club"].isin(['FC Barcelona', 'Chelsea', 'Juventus' , 'Real Madrid'])
df = football_data[idx]
df

# %%
# getting club and age data columns
df_new = df[['Age' , 'Club']].copy()
df_new

# %%
# ploting the graph
sns.kdeplot(df_new['Age'],hue=df_new['Club'])

# %% [markdown]
# 3. Plot Value of players as Stacked Histogram Preferred Foot wise (right and left). 

# %%
# sorting the data according to Preferred foot
foot = football_data['Preferred Foot']
foot

# %%
# getting data by prefered foot
football_data['Preferred Foot'].value_counts()

# %%
Right = 13948
Left = 4211
x = ["foot"]
plt.bar(x,Right)
plt.bar(x,Left)
plt.legend(["Right", "Left"])
plt.show()

# %%
ax = sns.histplot(x=football_data["Value"],hue=football_data["Preferred Foot"],data=football_data)
ax.set_xscale("log")

# %% [markdown]
# 4. Check distribution of International Reputation using Q-Q plot.

# %%
from statsmodels.graphics.gofplots import qqplot

# %%
qqplot(football_data["International Reputation"],line='s')
plt.show()


