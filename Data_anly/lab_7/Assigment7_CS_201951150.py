# %% [markdown]
# #                                    Lab Assignment-7
# ### Name - Shwetank Singh
# ### ID - 201951150

# %% [markdown]
# ### Import statements

# %%
import pandas as pd
import seaborn as sns
import math
import matplotlib.pyplot as plt
import numpy as np

# %% [markdown]
# ### Task - 1

# %%
df_1=pd.read_csv(r"All_India_CPI.csv")
df_1 = df_1.fillna(0)
df_1.fillna(0, inplace=True)
df_1.iat[315,20]=0
df_1.iat[291,20]=0
df_1.iat[294,20]=0
df_1['Housing'] = df_1.Housing.astype(float)
df_1_2=df_1.iloc[:, 3:28]
df_1_2

# %%
df_1_2.corr()

# %%
sns.heatmap(df_1_2.corr())

# %% [markdown]
# ### Task - 2

# %%
df_2=pd.read_csv(r"IPL Matches 2008-2020.csv")
df_2= df_2.fillna(0)
df_2.fillna(0, inplace=True)
df_2
df_2_crosstab=pd.crosstab(df_2['toss_winner'],
            df_2['winner'], 
            margins = False)
df_2_crosstab

# %% [markdown]
# ### Task - 3

# %%
df_3=pd.read_csv(r"melb_data.csv")
df_3= df_3.fillna(0)
df_3.fillna(0, inplace=True)

# %%
# find the maximum of each column
maxValues = df_3["Price"].max()
print(maxValues)
maxValues = df_3["Landsize"].max()
print(maxValues)

# %%
data1="Landsize"
data2="Price"
hb=plt.hexbin(df_3[data1], 
           df_3[data2], 
            gridsize = 10,
           cmap ='Greens') 
plt.xlabel(data1)
plt.ylabel(data2)
cb = plt.colorbar(hb)
plt.show()

# %%
sns.kdeplot(data=df_3, x=data1, y=data2,fill=True,levels=5)
plt.xlabel(data1)
plt.ylabel(data2)
plt.show()


