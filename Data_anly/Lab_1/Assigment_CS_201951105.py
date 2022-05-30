# %%
# importing library

import pandas as p
import json
import sys

# %% [markdown]
# Task 1 :- Create two data frames by reading above two files.

# %%
# Reading csv files
district_level_service = p.read_csv("data.csv")
district_level_service.head()

# %%
# Reading JSOn files
data_manufacturing = json.load(open("data.json"))
print(data_manufacturing.keys())


# %%
print(len(data_manufacturing["fields"]))
print(len(data_manufacturing["data"]))
print(data_manufacturing['fields'])

# %%
# Creating colums on bases of feilds and labels

columns = p.DataFrame(data_manufacturing['fields'])['label'].tolist()
print(columns)


# %%
# Creating Data Frames for each district level manufacturing

district_level_manufacturing = p.DataFrame(data_manufacturing['data'],columns=columns)
district_level_manufacturing.head()

# %% [markdown]
# Task 2:- Find out total ”Small” Manufacturing MSME in India.

# %%
# Changing data type

district_level_manufacturing['Lg_Dist_Code']=district_level_manufacturing['Lg_Dist_Code'].astype("int64")
district_level_manufacturing['MICRO']=district_level_manufacturing['MICRO'].astype("int64")
district_level_manufacturing['SMALL']=district_level_manufacturing['SMALL'].astype("int64")
district_level_manufacturing['MEDIUM']=district_level_manufacturing['MEDIUM'].astype("int64")

# %%
district_level_manufacturing.SMALL.sum()

# %% [markdown]
# Task 3:-  Create a dataframe having state wise total number of ”Micro”,”Small” and ”Medium” Services
# MSE (as shown below) and save the results as a CSV file.

# %%
district_level_service_statewise = district_level_service[['MICRO','SMALL','MEDIUM']].groupby(district_level_service.STATE_NAME).sum()
district_level_service_statewise.head()

# %%
# Exporting as CSV file

district_level_service_statewise.to_csv('district_level_service_statewise_output.csv')

# %% [markdown]
# Task 4:- Join the both the data frame based on common STATE NAME, DISTRICT NAME, Lg Dist Code
# and Last Updated. The result should look like below. ”x” and ”y” in below image represent the
# manufacturing MSME and service MSME respectively.

# %%
# Merging two files by taking inner on basis of lables

data_merge = p.merge(district_level_manufacturing,district_level_service,how='inner',on=['STATE_NAME','Lg_Dist_Code','DISTRICT_NAME','Last_Updated'])
data_merge.head()

# %% [markdown]
# Task 5:- Create a Pivot Table having rows STATE NAME and columns Service and Manufacturing
# ”MSME” as show in below. Use ”Sum” to add up district wise number.

# %%
# Adding up the columns to give sum of district wise

data_merge.groupby('STATE_NAME')[['MICRO_x','SMALL_x','MEDIUM_x','MICRO_y','SMALL_y','MEDIUM_y']].sum()


