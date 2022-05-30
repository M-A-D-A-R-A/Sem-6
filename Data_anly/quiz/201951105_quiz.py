# %%
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import statsmodels.api as sm
import pylab


sns.set(rc={'figure.figsize':(10,10)})

# %%
data = pd.read_csv("health.csv")
data.info()

# %%
# 10th column that is ninth index
data.columns[9]

# %%
data.replace('..',np.NaN,inplace=True)
data.isna().sum().sum()

# %%
data.iloc[:,11:]=data.iloc[:,11:].astype("float64")
data[(data["Vaccine"]=="HIB3") & (data["Country Name"]=="India")]

# %%
data_index = data.set_index(['Vaccine', 'Country Name'])
data_index = data_index.stack()
data_index.iloc[51]

# %%
data_index.head()

# %%
data_index = data_index.reset_index()

# %%
data_index.rename(columns={'level_2': 'Year', 0: 'Percentage'}, inplace=True)
data_index

# %%
df.isna().sum().sum()

# %%
df2 =df
data3 = df2.set_index(["Vaccine","Country Name"])
data3.head(53)

# %%
#Q6
df = df.copy()
df[["2009 [YR2009]",'2010 [YR2010]','2011 [YR2011]', '2012 [YR2012]', '2013 [YR2013]','2014 [YR2014]','2015 [YR2015]','2016 [YR2016]','2017 [YR2017]','2018 [YR2018]','2019 [YR2019]']]
hib = df[(df['Vaccine'] == 'HIB3') & (df['Country Name']=='India')]
hib['2015 [YR2015]']

# %%
df2 =df
df2 =df2.drop(['year'])


