# %% [markdown]
# Name - Shwetank Singh
# ID - 201951150

# %%
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import math

# %%
stocks=pd.read_csv("Stocks_pe.csv")
stocks

# %%
stocks=stocks[(stocks["P/E"]!=0) & (stocks["MarketCap"]!=0)]
#stocks[stocks["MarketCap"].isnull()==True]
stocks.dropna(inplace=True)

# %%
print(stocks["MarketCap"].mean())
print(stocks["MarketCap"].median())

print(stocks["P/E"].mean())
print(stocks["P/E"].median())

# %%
sns.boxplot(y=stocks["P/E"])

# %%
sns.histplot(stocks["P/E"],kde=True)

# %%
# Weights
stocks["wt"]=stocks["MarketCap"]/stocks["MarketCap"].sum()

# %%
#Weighted Mean
tmp=(stocks["P/E"]*stocks["wt"])/stocks["wt"].sum()
weighted_mean=tmp.sum()
weighted_mean

# %%
# Weighted Median
sort_data = stocks.sort_values(by=["P/E"])

wgtsum=0
for idx, w in sort_data.iterrows():
    wgtsum+=w["wt"]
    if wgtsum>=0.5:
        weighted_median=w["P/E"]
        break
weighted_median

# %%
# Trimmed Mean

trimmed_percentage=0.1
trim_num=math.floor(len(sort_data)*trimmed_percentage)
trim_mean=sort_data["P/E"].iloc[trim_num:len(sort_data)-trim_num].mean()
trim_mean

# %% [markdown]
# Estimates of Variability

# %%
#Standard Deviation
stocks["P/E"].std()

# %%
#Variance
stocks["P/E"].var()

# %%
#Mean abs Deviation
data1=stocks["P/E"]-stocks["P/E"].mean()
data2=data1.abs()
data2.mean()

# %%
#Median absolute Deviation from median
data1=stocks["P/E"]-stocks["P/E"].median()
data2=data1.abs()
data2.median()

# %%
#Range (Order Statistics)

print(sort_data['P/E'].iloc[-1],sort_data['P/E'].iloc[0])
print("Range =",sort_data['P/E'].iloc[-1]-sort_data['P/E'].iloc[0])

# %%
#Percentile (Order Statistics)

th=stocks["P/E"].quantile(0.5)
print(th)
print((stocks["P/E"]<th).sum())

# %%
# Inter Quantile Range

IQR=stocks["P/E"].quantile(0.75)-stocks["P/E"].quantile(0.25)
IQR


