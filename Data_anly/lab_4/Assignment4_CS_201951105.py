# %% [markdown]
# ## Lab 4

# %%
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

# %%
egg_production=pd.read_csv("Egg_Production_2007_2012.csv")
egg_production.head()

# %%
milk_production=pd.read_csv("Milk_Production_2007_2012.csv")
milk_production.head()

# %%
egg_production.rename(columns = {'2007-08 (In lakh nos.)':'2007-08','2008-09 (In lakh nos.)':'2008-09','2009-10 (In lakh nos.)':'2009-10','2010-11 (In lakh nos.)':'2010-11','2011-12 (In lakh nos.)':'2011-12'}, inplace = True)

# %%
egg_production.columns

# %% [markdown]
# 1. Merge two data into a data frame such that the new data frame has multi-level columns (like
# years under milk/eggs). Try to change the column names if required.

# %%
df =pd.merge(egg_production,milk_production,on="States/Uts",how="inner",suffixes=('_egg', '_milk'))

# %%
columns = [("","States/Uts"),("Eggs","2007-08"),  ("Eggs","2008-09"),  ("Eggs","2009-10"),  ("Eggs","2010-11"),  ("Eggs","2011-12"),  ("Milk","2007-08"), ("Milk","2008-09"), ("Milk","2009-10"), ("Milk","2010-11"), ("Milk","2011-12")]

df.columns = pd.MultiIndex.from_tuples(columns)
df.head()

# %%
df_milk=milk_production[milk_production['States/Uts'].isin(['Gujarat', 'Kerala', 'Andhra Pradesh', 'Uttar Pradesh','Punjab'])]
label=['Gujarat', 'Kerala', 'Andhra Pradesh', 'Uttar Pradesh','Punjab']

# %% [markdown]
# 2. Present the production of milk in Gujarat, Kerala, Andhra Pradesh, Uttar Pradesh and Panjab
# on 2007-2008 as a Pie chart. The pie chart should consist of proportion in percentage and labels
# for each piece.

# %%
print("Milk Production")
fig,axes=plt.subplots(2,3,figsize=(20,10))
df_milk.plot.pie(x='States/Uts',y='2007-08',labels=label, autopct='%.0f%%',ax=axes[0,0],title="2007-08")
df_milk.plot.pie(x='States/Uts',y='2008-09',labels=label, autopct='%.0f%%', ax=axes[0,1],title='2008-09')
df_milk.plot.pie(x='States/Uts',y='2009-10',labels=label, autopct='%.0f%%', ax=axes[0,2],title='2009-10')
df_milk.plot.pie(x='States/Uts',y='2010-11',labels=label, autopct='%.0f%%',ax=axes[1,0],title="2010-11")
df_milk.plot.pie(x='States/Uts',y='2011-12',labels=label, autopct='%.0f%%', ax=axes[1,1],title='2011-12')
axes[1,2].axis('off')

plt.show()

# %% [markdown]
# 3. Plot five pie charts of egg production in Gujarat, Kerala, Andhra Pradesh, Uttar Pradesh and
# Panjab given five years range. Each pie chart should represent the egg production in above five
# states for a given year.

# %%
df_egg=egg_production[egg_production['States/Uts'].isin(['Gujarat', 'Kerala', 'Andhra Pradesh', 'Uttar Pradesh','Punjab'])]

print("Egg Production")
fig,axes=plt.subplots(2,3,figsize=(20,10))
df_egg.plot.pie(x='States/Uts',y='2007-08',labels=label, autopct='%.0f%%',ax=axes[0,0],title="2007-08")
df_egg.plot.pie(x='States/Uts',y='2008-09',labels=label, autopct='%.0f%%', ax=axes[0,1],title='2008-09')
df_egg.plot.pie(x='States/Uts',y='2009-10',labels=label, autopct='%.0f%%', ax=axes[0,2],title='2009-10')
df_egg.plot.pie(x='States/Uts',y='2010-11',labels=label, autopct='%.0f%%',ax=axes[1,0],title="2010-11")
df_egg.plot.pie(x='States/Uts',y='2011-12',labels=label, autopct='%.0f%%', ax=axes[1,1],title='2011-12')
axes[1,2].axis('off')
plt.show()

# %%
egg_production_normalized=egg_production.iloc[:-1]
egg_production_normalized_columns=['2007-08', '2008-09', '2009-10', '2010-11', '2011-12']
for column in egg_production_normalized_columns:
    egg_production_normalized[column] = (egg_production_normalized[column] - egg_production_normalized[column].min()) / (egg_production_normalized[column].max() - egg_production_normalized[column].min())    
  

# %% [markdown]
# 4. Plot Staked Area Chart that represents state wise (i.e. states are in x axis) production of eggs
# (y-axis). There would be five stacked colors and each color represents the production of egg in
# a state. The egg production for each state should be normalized over the year range.

# %%
fig, ax = plt.subplots(figsize=(20,10))
labels=['2007-08', '2008-09', '2009-10', '2010-11', '2011-12']
ax.stackplot(egg_production_normalized['States/Uts'],egg_production_normalized.drop(['States/Uts'],axis=1).T,labels=labels)
plt.xticks(rotation=90)
ax.legend(loc='upper right')
plt.title("Data normalized yearwise")
plt.show()

# %%
egg_production_normalized_statewise=egg_production[:-1]
egg_production_normalized_statewise.iloc[:,1:]=egg_production_normalized_statewise.iloc[:,1:].div(egg_production_normalized_statewise.iloc[:,1:].sum(axis=1), axis=0)

# %%
fig, ax = plt.subplots(figsize=(20,10))
labels=['2007-08', '2008-09', '2009-10', '2010-11', '2011-12']
ax.stackplot(egg_production_normalized_statewise['States/Uts'],egg_production_normalized_statewise.drop(['States/Uts'],axis=1).T,labels=labels)
plt.xticks(rotation=90)
ax.legend(loc='upper right')
plt.title("Data - Statewise")
plt.show()


